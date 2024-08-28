classdef TestCompare < matlab.unittest.TestCase
    properties
        comm1;
        comm2;
        comm3;
        comm4;
        n = 100;
        k = 10;
    end

    properties (TestParameter)
        method = {'vi', 'nmi', 'splitjoin', 'rand', 'ari'};
        order = {'ascend', 'descend', 'ascend', 'descend', 'descend'};
    end

    methods (TestClassSetup)
        function setupMembership(testCase)
            testCase.comm1 = randi(testCase.k, [1 testCase.n]);
            testCase.comm2 = shuffleMembership(testCase.comm1, ...
                                               ceil(testCase.n / 5));
            testCase.comm3 = shuffleMembership(testCase.comm1, ...
                                               ceil(testCase.n / 2));
            testCase.comm4 = randi(testCase.k, [1 testCase.n]);

            function cOut = shuffleMembership(cIn, k)
                swap = [randperm(length(cIn), k); randperm(length(cIn), k)];
                cOut = cIn;
                cOut(swap(1, :)) = cIn(swap(2, :));
                cOut(swap(2, :)) = cIn(swap(1, :));
            end
        end
    end

    methods (Test, TestTags = {'Unit', 'assertion'})
        function testFailsForMatrices(testCase)
            actual = @() igraph.compare(...
                [testCase.comm1; testCase.comm2], testCase.comm3);
            testCase.verifyError(actual, "igraph:invalidValue");
        end

        function testHandlesDifferentVectorOrientation(testCase)
            expected = igraph.compare(testCase.comm1, testCase.comm2);
            actual = igraph.compare(testCase.comm1, testCase.comm2');

            testCase.verifyEqual(actual, expected);
        end
    end

    methods (Test, TestTags = {'Unit', 'methods'}, ParameterCombination = "sequential")
        function testScoresCommunitiesInExpectedOrder(testCase, method, order)
            s = zeros(1, 4);
            s(1) = igraph.compare(testCase.comm1, testCase.comm1, method);
            s(2) = igraph.compare(testCase.comm1, testCase.comm2, method);
            s(3) = igraph.compare(testCase.comm1, testCase.comm3, method);
            s(4) = igraph.compare(testCase.comm1, testCase.comm4, method);

            testCase.verifyTrue(issorted(s, order));
        end
    end
end

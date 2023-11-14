classdef TestMxPredicate < matlab.unittest.TestCase
    properties
        adj;
        weightedAdj;
    end

    properties (ClassSetupParameter)
        dtype = struct('logical', 'logical', ...
                       'double', 'double');
        useSparse = struct('sparse', true, 'dense', false);
        n = num2cell(10 .^ (1:2));
    end

    methods (TestClassSetup)
        function unWeightedSetup(testCase, n, dtype, useSparse)
            testCase.adj = randomGraph(n, 5, dtype, useSparse);
            testCase.adj = tril(testCase.adj);
            testCase.adj = testCase.adj + testCase.adj' - ...
                diag(diag(testCase.adj));
            if strcmp(dtype, 'logical')
                logical(testCase.adj);
            end
        end

        function weightedSetup(testCase, n, useSparse)
            testCase.weightedAdj = randomGraph(n, 5, 'double', useSparse);
            testCase.weightedAdj = addWeights(testCase.weightedAdj);
            testCase.weightedAdj = tril(testCase.weightedAdj);
            testCase.weightedAdj = testCase.weightedAdj + ...
                testCase.weightedAdj' - diag(diag(testCase.weightedAdj));
        end
    end

    methods (Test, TestTags = {'Unit'})
        function testMatrixIsSquare(testCase)
            actual = testIsSquare(testCase.adj);
            testCase.verifyTrue(actual);

            actual = testIsSquare([testCase.adj testCase.adj]);
            testCase.verifyFalse(actual);
        end

        function testVectorIsVector(testCase)
            actual = testIsVector(1:10);
            testCase.verifyTrue(actual);
        end

        function testMatrixIsNotVector(testCase)
            actual = testIsVector(testCase.adj);
            testCase.verifyFalse(actual);
        end

        function testMatrixIsWeighted(testCase)
            actual = testIsWeighted(testCase.adj);
            testCase.verifyFalse(actual);

            actual = testIsWeighted(testCase.weightedAdj);
            testCase.verifyTrue(actual);
        end

        function testMatrixIsDirected(testCase)
            actual = testIsDirected(testCase.adj);
            testCase.verifyFalse(actual);

            actual = testIsDirected(testCase.adj + ...
                                    randi([0 1], size(testCase.adj), ...
                                          'like', full(testCase.adj)));
            testCase.verifyTrue(actual);
        end

        function testMatrixIsTriU(testCase)
            actual = testIsTriU(testCase.adj);
            testCase.verifyFalse(actual);

            actual = testIsTriU(triu(testCase.adj));
            testCase.verifyTrue(actual);

            actual = testIsTriU(tril(testCase.adj));
            testCase.verifyFalse(actual);
        end

        function testMatrixIsTriL(testCase)
            actual = testIsTriL(testCase.adj);
            testCase.verifyFalse(actual);

            actual = testIsTriL(triu(testCase.adj));
            testCase.verifyFalse(actual);

            actual = testIsTriL(tril(testCase.adj));
            testCase.verifyTrue(actual);
        end

        function testMatrixIsSymmetric(testCase)
            actual = testIsSymmetric(testCase.adj);
            testCase.verifyTrue(actual);

            actual = testIsSymmetric(triu(testCase.adj));
            testCase.verifyFalse(actual);

            actual = testIsSymmetric(tril(testCase.adj));
            testCase.verifyFalse(actual);
        end
    end
end

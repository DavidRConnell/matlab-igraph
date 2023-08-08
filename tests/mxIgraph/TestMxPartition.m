classdef TestMxPartition < matlab.unittest.TestCase
    properties
        membership;
    end

    properties (ClassSetupParameter)
        n = num2cell(10 .^ (1:2));
    end

    methods (TestClassSetup)
        function classSetup(testCase, n)
            testCase.membership = randperm(n);
        end
    end

    methods (Test, TestTags = {'Unit'})
        function testVectorLength(testCase, n)
            actual = testVectorLength(testCase.membership);
            testCase.verifyEqual(actual, n);
        end

        function testReadAndCreatePartition(testCase)
            actual = testReproducePartition(testCase.membership);
            testCase.verifyEqual(actual, testCase.membership);
        end
    end
end

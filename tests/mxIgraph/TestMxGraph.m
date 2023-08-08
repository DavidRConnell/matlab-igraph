classdef TestMxGraph < matlab.unittest.TestCase
    properties
        adj;
        weightedAdj;
        createSparse = true;
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
        end

        function weightedSetup(testCase, n, useSparse)
            testCase.weightedAdj = randomGraph(n, 5, 'double', useSparse);
            testCase.weightedAdj = addWeights(testCase.weightedAdj);
        end
    end

    methods (Test, TestTags = {'Unit', 'unweighted'})
        function testCountNodes(testCase, n)
            actual = testVCount(testCase.adj);
            testCase.verifyEqual(actual, n);
        end

        function testCountEdges(testCase)
            actual = testECount(testCase.adj);
            testCase.verifyEqual(actual, full(sum(testCase.adj > 0, 'all')));
        end

        function testReadAndCreateFullGraph(testCase)
            actual = testReproduceAdj(testCase.adj, ~testCase.createSparse);
            testCase.verifyEqual(actual, full(double(testCase.adj)))
        end

        function testReadAndCreateSparseGraph(testCase)
            actual = testReproduceAdj(testCase.adj, testCase.createSparse);
            testCase.verifyEqual(actual, sparse(double(testCase.adj)))
        end
    end

    methods (Test, TestTags = {'Unit', 'weighted'})
        function testReadAndCreateWeightedFullGraph(testCase)
            actual = testReproduceAdj(testCase.weightedAdj, ~testCase.createSparse);
            testCase.verifyEqual(actual, full(double(testCase.weightedAdj)))
        end

        function testReadAndCreateWeightedSparseGraph(testCase)
            actual = testReproduceAdj(testCase.weightedAdj, testCase.createSparse);
            testCase.verifyEqual(actual, sparse(double(testCase.weightedAdj)))
        end
    end
end

classdef TestMxGraph < matlab.unittest.TestCase
    properties
        adj;
        weightedAdj;
        adjOptions;
    end

    properties (ClassSetupParameter)
        dtype = struct('logical', 'logical', ...
                       'double', 'double');
        makeSparse = struct('sparse', true, 'dense', false);
        n = num2cell(10 .^ (1:2));
    end

    methods (TestClassSetup)
        function unWeightedSetup(testCase, n, dtype, makeSparse)
            testCase.adj = randomGraph(n, 5, dtype, makeSparse);
        end

        function weightedSetup(testCase, n, makeSparse)
            testCase.weightedAdj = randomGraph(n, 5, 'double', makeSparse);
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

        function testReadAndCreateGraph(testCase, dtype, makeSparse)
            testCase.adjOptions.makeSparse = makeSparse;
            testCase.adjOptions.dtype = dtype;
            actual = testReproduceAdj(testCase.adj, testCase.adjOptions);
            testCase.verifyEqual(actual, testCase.adj)
        end
    end

    methods (Test, TestTags = {'Unit', 'weighted'})
        function testReadAndCreateWeightedGraph(testCase, makeSparse)
            testCase.adjOptions.makeSparse = makeSparse;
            testCase.adjOptions.dtype = 'double';
            actual = testReproduceAdj(testCase.weightedAdj, ...
                                      testCase.adjOptions);
            testCase.verifyEqual(actual, testCase.weightedAdj)
        end
    end
end

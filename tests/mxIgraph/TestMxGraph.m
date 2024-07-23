classdef TestMxGraph < matlab.unittest.TestCase
    properties
        adj;
        weightedAdj;
        graphOpts;
    end

    properties (ClassSetupParameter)
        dtype = igraph.args.datatypes();
        repr = igraph.args.representations();
        n = num2cell(10 .^ (1:2));
    end

    methods (TestClassSetup)
        function unWeightedSetup(testCase, n, dtype, repr)
            if strcmp(repr, 'graph')
                % Prevents the Weight attribute from being added to edge table.
                dtype = 'logical';
            end

            testCase.adj = randomGraph(n, 5, dtype, repr);

            if strcmp(repr, 'graph')
                testCase.adj = digraph(testCase.adj);
            end
        end

        function weightedSetup(testCase, n, repr)
            testCase.weightedAdj = randomGraph(n, 5, 'double', repr);
            testCase.weightedAdj = addWeights(testCase.weightedAdj);

            if strcmp(repr, 'graph')
                testCase.weightedAdj = digraph(testCase.weightedAdj);
            end
        end
    end

    methods (Test, TestTags = {'Unit', 'unweighted'})
        function testReadAndCreateGraph(testCase, dtype, repr)
            testCase.graphOpts.repr = repr;
            testCase.graphOpts.dtype = dtype;
            testCase.graphOpts.isdirected = true;
            testCase.graphOpts.isweighted = false;

            actual = testReproduceAdj(testCase.adj, testCase.graphOpts);
            testCase.verifyEqual(actual, testCase.adj)
        end
    end

    methods (Test, TestTags = {'Unit', 'weighted'})
        function testReadAndCreateWeightedGraph(testCase, repr)
            testCase.graphOpts.repr = repr;
            testCase.graphOpts.dtype = 'double';
            testCase.graphOpts.isdirected = true;
            testCase.graphOpts.isweighted = true;
            testCase.graphOpts.weight = 'Weight';

            actual = testReproduceAdj(testCase.weightedAdj, ...
                                      testCase.graphOpts);
            testCase.verifyEqual(actual, testCase.weightedAdj)
        end
    end
end

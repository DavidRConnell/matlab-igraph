classdef TestRNG < matlab.unittest.TestCase
    properties
        seed = [1 2];
        method = "kregular"
    end

    properties (TestParameter)
        generator = {'matlab', 'mt19937', 'glibc2', 'pcg32', 'pcg64'};
    end

    methods
        function verifyApproximatly(testCase, actual, expected)
            eps = 1e-4;
            testCase.verifyGreaterThan(actual, expected - eps);
            testCase.verifyLessThan(actual, expected + eps);
        end
    end

    methods (Test, TestTags = {'Unit'})
        function reproduceRandomGraph(testCase, generator)
            igraph.rng(testCase.seed(1), generator);
            expected1 = igraph.randgame(testCase.method);
            expected2 = igraph.randgame(testCase.method);

            igraph.rng(testCase.seed(1));
            actual1 = igraph.randgame(testCase.method);
            actual2 = igraph.randgame(testCase.method);

            testCase.verifyEqual(actual1, expected1);
            testCase.verifyEqual(actual2, expected2);
        end

        function produceDifferentGraphsWithDifferentSeeds(testCase, generator)
            igraph.rng(testCase.seed(1), generator);
            expected = igraph.randgame(testCase.method);

            igraph.rng(testCase.seed(2));
            actual = igraph.randgame(testCase.method);

            testCase.verifyNotEqual(actual, expected);
        end

        function produceDifferentGraphsWithoutReseeding(testCase, generator)
            igraph.rng(testCase.seed(1), generator);
            expected = igraph.randgame(testCase.method);
            actual = igraph.randgame(testCase.method);

            testCase.verifyNotEqual(actual, expected);
        end
    end

    methods (Test, TestTags = {'Repeatability'})
        % Methods to confirm identical results of stochastic methods across
        % environments.

        function checkRngIsConsistentOnDifferentOses(testCase)
        % Generated using Linux R2023b.
            expected = [0.2919 0.4460 0.6106];
            igraph.rng(testCase.seed(1), "matlab");

            g = igraph.randgame("preference", nNodes = 100, mixingParam=0.35);
            igraph.cluster(g, "labelPropagation");
            actual = rand(1, 3);

            testCase.verifyApproximatly(actual, expected);
        end

        function checkReproducibleGraphs(testCase, generator);
        % Generated using Linux R2023b.
            expected = struct('matlab', [4 6 7 3 4 10 5 8 9 6 7 9 8 10 10], ...
                              'mt19937', [4 8 10 4 7 9 5 7 8 9 9 10 7 8 10], ...
                              'glibc2', [2 5 7 3 5 4 8 9 10 7 7 9 10 9 10], ...
                              'pcg32', [3 5 9 4 5 9 7 9 6 10 7 8 10 8 10], ...
                              'pcg64', [4 6 8 5 9 10 5 6 8 9 10 7 7 9 10]);

            igraph.rng(testCase.seed(1), generator);

            actual = igraph.randgame(testCase.method);
            actual = actual.Edges.EndNodes(:, 2)';

            testCase.verifyEqual(actual, expected.(generator));
        end
    end
end

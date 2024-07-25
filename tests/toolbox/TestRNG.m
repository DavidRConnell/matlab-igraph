classdef TestRNG < matlab.unittest.TestCase
    properties
        seed = [1 2];
    end

    properties (TestParameter)
        generator = {'matlab', 'mt19937', 'glibc2', 'pcg32', 'pcg64'};
    end

    methods (Test, TestTags = {'Unit'})
        function reproduceRandomGraph(testCase, generator)
            % Random choice.
            method = "kregular";

            igraph.rng(testCase.seed(1), generator);
            expected1 = igraph.randgame(method);
            expected2 = igraph.randgame(method);

            igraph.rng(testCase.seed(1));
            actual1 = igraph.randgame(method);
            actual2 = igraph.randgame(method);

            testCase.verifyEqual(actual1, expected1);
            testCase.verifyEqual(actual2, expected2);
        end

        function produceDifferentGraphsWithDifferentSeeds(testCase, generator)
            method = "kregular";

            igraph.rng(testCase.seed(1), generator);
            expected = igraph.randgame(method);

            igraph.rng(testCase.seed(2));
            actual = igraph.randgame(method);

            testCase.verifyNotEqual(actual, expected);
        end

        function produceDifferentGraphsWithoutReseeding(testCase, generator)
            method = "kregular";

            igraph.rng(testCase.seed(1), generator);
            expected = igraph.randgame(method);
            actual = igraph.randgame(method);

            testCase.verifyNotEqual(actual, expected);
        end
    end
end

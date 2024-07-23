classdef TestIO < matlab.unittest.TestCase
    properties
        adj = rand(10);
        p = 0.5;
        location = "iocache";
    end

    properties (TestParameter)
        method = {'mat', 'edgelist', 'ncol', 'lgl', 'graphml', ...
                  'gml'};
        isweighted = struct('weighted', true, 'unweighted', false);
        isdirected = struct('directed', true, 'undirected', false);
    end

    methods (TestClassSetup)
        function setupTestDirectory(testCase)
            mkdir(testCase.location);
            testCase.addTeardown(@() rmdir(testCase.location, "s"));
        end
    end

    methods
        function TF = weightsAllowed(~, method)
            TF = ~strcmp(method, 'edgelist');
        end

        function TF = directedAllowed(~, method)
            TF = ~any(strcmp(method, {'ncol', 'lgl'}));
        end

        function ext = methodExt(~, method)
            ext = method;
            switch lower(method)
              case 'edgelist'
                ext = 'txt';
              case 'graphdb'
                ext = 'zip';
              case 'pajek'
                ext = 'net';
              case 'leda'
                ext = 'gw';
            end
        end

        function name = filename(testCase, method)
            name = "graph." + testCase.methodExt(method);
            name = fullfile(testCase.location, name);
        end

        function adj = getAdj(testCase, isweighted, isdirected)
            adj = testCase.adj;
            if ~isweighted
                adj = adj > testCase.p;
            end

            if ~isdirected
                adj = tril(adj);
            end
        end

        function verifyApproximatly(testCase, actual, expected)
            eps = 1e-8;
            testCase.verifyGreaterThan(actual, expected - eps);
            testCase.verifyLessThan(actual, expected + eps);
        end
    end

    methods (Test, TestTags={'Unit'})
        function reproduceAdj(testCase, method, isweighted, isdirected)
            testCase.assumeFalse(isweighted && ~testCase.weightsAllowed(method), ...
                                "Method cannot handle weights, skipping.")
            testCase.assumeFalse(isdirected && ~testCase.directedAllowed(method), ...
                                "Method cannot handle directed graphs, skipping.")

            if isweighted
                dtype = 'double';
            else
                dtype = 'logical';
            end

            name = testCase.filename(method);
            expected = testCase.getAdj(isweighted, isdirected);
            igraph.save(name, expected, 'overwrite', true);
            actual = igraph.load(name, isweighted=isweighted, ...
                                 isdirected=isdirected, repr="full", ...
                                 dtype=dtype);

            if any(strcmp(method, {'ncol', 'lgl'}))
                % These methods do not track node id order so use a weaker
                % test of equivalence.
                testCase.verifyTrue(igraph.isisomorphic(actual, expected));
            elseif isweighted
                testCase.verifyApproximatly(actual, expected);
            else
                testCase.verifyEqual(actual, expected);
            end
        end
    end
end

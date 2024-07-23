function TF = issubisomorphic(g1, g2, g1Opts, g2Opts)
%ISISOMORPHIC test if g2 is isomorphic to a subgraph of g1
%   TF = ISSUBISOMORPHIC(GRAPH1, GRAPH2) returns true if GRAPH2 is isomorphic
%   with a subgraph of GRAPH1. Throws an error if GRAPH1 is not bigger than
%   GRAPH2.
%
%   TF = ISSUBISOMORPHIC(GRAPH1, GRAPH2, G1ISDIRECTED, G2ISDIRECTED) override
%   the guesses for whether GRAPH1 and GRAPH2 are directed. If they are not
%   both the same (both directed or both undirected) an error is triggered.
%
%   See also IGRAPH.ISISOMORPHIC.

    arguments
        g1 {igutils.mustBeGraph}
        g2 {igutils.mustBeGraph}
        g1Opts.g1isdirected (1, 1) logical = igraph.isdirected(g1);
        g2Opts.g2isdirected (1, 1) logical = igraph.isdirected(g2);
    end

    g1Opts.isdirected = g1Opts.g1isdirected;
    g2Opts.isdirected = g2Opts.g2isdirected;
    if g1Opts.isdirected ~= g2Opts.isdirected
        eid = "igraph:inconsistentProperties";
        msg = "Graphs must both be directed or both be undirected to " + ...
              "test if isomorphic.";
        error(eid, msg);
    end

    if length(g2) >= length(g1)
        eid = "igraph:notASubgraph";
        msg = "Graph 2 must be smaller than graph 1.";
        error(eid, msg);
    end

    TF = mexIgraphDispatcher(mfilename(), g1, g2, g1Opts, g2Opts);
end

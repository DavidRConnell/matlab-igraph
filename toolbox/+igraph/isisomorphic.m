function TF = isisomorphic(g1, g2, g1Opts, g2Opts)
%ISISOMORPHIC test if two graphs are isomorphic
%   TF = ISISOMORPHIC(GRAPH1, GRAPH2) returns true if GRAPH1 and GRAPH2 are the
%       equivalent when ignoring the order of nodes.
%
%   TF = ISISOMORPHIC(GRAPH1, GRAPH2, G1ISDIRECTED, G2ISDIRECTED) override the
%       guesses for whether GRAPH1 and GRAPH2 are directed. If they are not
%       both the same (both directed or both undirected) an error is triggered.
%
%   See also IGRAPH.ISSUBISOMORPHIC.

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

    TF = mexIgraphDispatcher(mfilename(), g1, g2, g1Opts, g2Opts);
end

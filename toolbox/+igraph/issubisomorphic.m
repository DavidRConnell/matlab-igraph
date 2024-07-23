function TF = issubisomorphic(g1, g2, graphOpts)
%ISISOMORPHIC test if g2 is isomorphic to a subgraph of g1
%   TF = ISSUBISOMORPHIC(GRAPH1, GRAPH2) returns true if GRAPH2 is isomorphic
%   with a subgraph of GRAPH1. Throws an error if GRAPH1 is not bigger than
%   GRAPH2.
%
%   TF = ISSUBISOMORPHIC(GRAPH1, GRAPH2, 'isdirected', TF) override the guesses
%   for whether GRAPH1 and GRAPH2 are directed. By default IGRAPH.ISDIRECTED
%   tests both graphs. If one is directed the value is set to true otherwise
%   false.
%
%   See also IGRAPH.ISISOMORPHIC.

    arguments
        g1 {igutils.mustBeGraph}
        g2 {igutils.mustBeGraph}
        graphOpts.isdirected (1, 1) logical = ...
            igraph.isdirected(g1) || igraph.isdirected(g2);
    end

    if igraph.numnodes(g2) >= igraph.numnodes(g1)
        eid = "igraph:notASubgraph";
        msg = "Graph 2 must be smaller than graph 1.";
        error(eid, msg);
    end

    TF = mexIgraphDispatcher(mfilename(), g1, g2, graphOpts);
end

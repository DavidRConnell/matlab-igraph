function TF = issubisomorphic(adj1, adj2, adj1Options, adj2Options)
%ISISOMORPHIC test if adj2 is isomorphic to a subgraph of adj1
%   TF = ISSUBISOMORPHIC(ADJ1, ADJ2) returns true if ADJ2 is isomorphic with
%       a subgraph of ADJ1. Throws an error if ADJ1 is not bigger than ADJ2.
%
%   TF = ISSUBISOMORPHIC(ADJ1, ADJ2, ADJ1ISDIRECTED, ADJ2ISDIRECTED) override
%       the guesses for whether ADJ1 and ADJ2 are directed. If they are not
%       both the same (both directed or both undirected) an error is triggered.

    arguments
        adj1 {mustBeAdj}
        adj2 {mustBeAdj}
        adj1Options.adj1isdirected (1, 1) logical = igraph.isdirected(adj1);
        adj2Options.adj2isdirected (1, 1) logical = igraph.isdirected(adj2);
    end

    adj1Options.isdirected = adj1Options.adj1isdirected;
    adj2Options.isdirected = adj2Options.adj2isdirected;
    if adj1Options.isdirected ~= adj2Options.isdirected
       eid = "igraph:inconsistentProperties";
       msg = "Graphs must both be directed or both be undirected to " + ...
             "test if isomorphic.";
       error(eid, msg);
    end

    if length(adj2) >= length(adj1)
        eid = "igraph:notASubgraph";
        msg = "Graph 2 must be smaller than graph 1.";
        error(eid, msg);
    end

    TF = mexIgraphDispatcher(mfilename(), adj1, adj2, adj1Options, adj2Options);
end

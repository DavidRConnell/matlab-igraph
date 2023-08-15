function TF = issubisomorphic(adj1, adj2, adjOptions)
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
        adjOptions.adj1isdirected (1, 1) logical = igraph.isdirected(adj1);
        adjOptions.adj2isdirected (1, 1) logical = igraph.isdirected(adj2);
    end

    if adjOptions.adj1isdirected ~= adjOptions.adj2isdirected
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

    TF = mexIgraphIsSubIsomorphic(adj1, adj2, ...
                                  adjOptions.adj1isdirected, ...
                                  adjOptions.adj2isdirected);
end

function TF = isisomorphic(adj1, adj2, adjOptions)
%ISISOMORPHIC test if adj1 and adj2 are isomorphic
%   TF = ISISOMORPHIC(ADJ1, ADJ2) returns true if ADJ1 and ADJ2 are the
%       equivalent when ignoring the order of nodes.
%
%   TF = ISISOMORPHIC(ADJ1, ADJ2, ADJ1ISDIRECTED, ADJ2ISDIRECTED) override the
%       guesses for whether ADJ1 and ADJ2 are directed. If they are not both
%       the same (both directed or both undirected) an error is triggered.

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

    TF = mexIgraphIsIsomorphic(adj1, adj2, ...
                               adjOptions.adj1isdirected, ...
                               adjOptions.adj2isdirected);
end

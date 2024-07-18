function TF = isisomorphic(adj1, adj2, adj1Options, adj2Options)
%ISISOMORPHIC test if adj1 and adj2 are isomorphic
%   TF = ISISOMORPHIC(ADJ1, ADJ2) returns true if ADJ1 and ADJ2 are the
%       equivalent when ignoring the order of nodes.
%
%   TF = ISISOMORPHIC(ADJ1, ADJ2, ADJ1ISDIRECTED, ADJ2ISDIRECTED) override the
%       guesses for whether ADJ1 and ADJ2 are directed. If they are not both
%       the same (both directed or both undirected) an error is triggered.

    arguments
        adj1 {igraph.args.mustBeGraph}
        adj2 {igraph.args.mustBeGraph}
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

    TF = mexIgraphDispatcher(mfilename(), adj1, adj2, adj1Options, adj2Options);
end

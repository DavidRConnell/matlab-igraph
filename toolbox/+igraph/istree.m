function TF = istree(adj, opts)
%ISTREE test if the adjacency is a tree graph
%   TF = ISTREE(ADJ) checks if the adjacency matrix ADJ is a tree graph.
%   See also igraph.treeRoot
    arguments
        adj {mustBeAdj};
        opts.isdirected (1, 1) logical;
        opts.mode (1, :) char {mustBeMemberi(opts.mode, ...
                                             {'in', 'out', 'all'})} = 'all';
    end

    if ~isoptionset(opts, "isdirected")
       opts.isdirected = ~issymmetric(adj) || opts.mode ~= "all";
    end

    opts.mode = lower(opts.mode);
    TF = mexIgraphIsTree(adj, opts.isdirected, opts.mode, false);
end

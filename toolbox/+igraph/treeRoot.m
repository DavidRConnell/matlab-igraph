function root = treeRoot(adj, opts)
%TREEROOT find the root node of the tree graph.
%   ROOT = TREEROOT(ADJ) return the vertex id of the tree root.
%   If adj is not a tree, returns 0.
%
%   See also igraph.istree.
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
    root = mexIgraphIsTree(adj, opts.isdirected, opts.mode, true);
end
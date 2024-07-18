function root = treeRoot(graph, opts)
%TREEROOT find the root node of the tree graph.
%   ROOT = TREEROOT(GRAPH) return the vertex id of the tree root.
%   If graph is not a tree, returns 0.
%
%   See also igraph.istree.

    arguments
        graph {igraph.args.mustBeGraph};
        opts.isdirected (1, 1) logical;
        opts.mode (1, :) char {igraph.args.mustBeMode} = 'all';
    end

    if ~igraph.args.isoptionset(opts, "isdirected")
       opts.isdirected = ~issymmetric(graph) || opts.mode ~= "all";
    end

    opts.mode = lower(opts.mode);
    opts.findRoot = true;
    root = mexIgraphDispatcher("isTree", graph, opts);
end

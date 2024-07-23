function TF = istree(graph, opts)
%ISTREE test if the graph is a tree graph
%   TF = ISTREE(GRAPH) checks if GRAPH is a tree graph.
%
%   See also IGRAPH.TREEROOT.

    arguments
        graph {igutils.mustBeGraph};
        opts.isdirected (1, 1) logical;
        opts.mode (1, :) char {igutils.mustBeMode} = 'all';
    end

    if ~igutils.isoptionset(opts, "isdirected")
       opts.isdirected = ~issymmetric(graph) || opts.mode ~= "all";
    end

    opts.mode = lower(opts.mode);
    opts.findRoot = false;
    TF = mexIgraphDispatcher(mfilename(), graph, opts);
end

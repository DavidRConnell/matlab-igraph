function values = centrality(graph, method, opts)
%CENTRALITY calculate centrality measure in a graph
%   VALUES = CENTRALITY(GRAPH, METHOD) calculate the centrality for all nodes
%      in GRAPH using METHOD. METHOD can be one of 'closeness', 'harmonic',
%      'betweenness', 'pagerank', 'burt' (equivalently 'constraint').
%   VALUES = CENTRALITY(GRAPH, METHOD, ..., 'VIDS', NODES) select a subset of
%      nodes in the graph to calculate centrality on.
%   VALUES = CENTRALITY(GRAPH, METHOD, ..., 'directed', TF) if true, treat the
%      graph is directed (defaults to IGRAPH.ISDIRECTED).
%
%   Method specific arguments:
%       'normalized' logical for 'closeness', 'harmonic'; whether to return
%           the mean (true) or sum (false) of the results (default true).
%       'damping' value between 0 and 1, the 'pagerank' damping factor
%           (default 0.85).
%
%       NOTE: Method specific arguments will be silently ignored if supplied
%       but not needed for the given method.
%
%   See also IGRAPH.ISDIRECTED.

    arguments
       graph {igraph.args.mustBeGraph};
       method (1, :) char ...
           {igraph.args.mustBeMemberi(method, ...
                                      {'closeness', 'harmonic', 'betweenness', ...
                                       'pagerank', 'burt', 'constraint'})};
       opts.vids (1, :) {mustBePositive, mustBeInteger} = 1:length(graph);
       opts.mode (1, :) char {igraph.args.mustBeMode} = 'all';
       opts.directed (1, 1) logical = igraph.isdirected(graph);
       opts.normalized (1, 1) logical = true;
       opts.damping (1, 1) {mustBeInRange(opts.damping, 0, 1)} = 0.85;
    end

    method = lower(method);
    if strcmp(method, 'constraint')
        method = 'burt';
    end

    opts.vids = opts.vids - 1;
    values = mexIgraphDispatcher(mfilename(), graph, method, opts);
end

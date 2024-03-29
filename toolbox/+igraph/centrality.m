function values = centrality(adj, method, opts)
%CENTRALITY calculate centrality measure in a graph
%   VALUES = CENTRALITY(ADJ, METHOD) calculate the centrality for all nodes
%      in ADJ using METHOD. METHOD can be one of 'closeness', 'harmonic',
%      'betweenness', 'pagerank', 'burt' (equivalently 'constraint').
%   VALUES = CENTRALITY(ADJ, METHOD, ..., 'VIDS', NODES) select a subset of
%      nodes in the graph to calculate centrality on.
%   VALUES = CENTRALITY(ADJ, METHOD, ..., 'directed', TF) if true, treat the
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
%   See also igraph.isdirected.

    arguments
       adj {mustBeAdj};
       method (1, :) char ...
           {mustBeMemberi(method, ...
                          {'closeness', 'harmonic', 'betweenness', ...
                           'pagerank', 'burt', 'constraint'})};
       opts.vids (1, :) {mustBePositive, mustBeInteger} = 1:length(adj);
       opts.mode (1, :) char {mustBeMode} = 'all';
       opts.directed (1, 1) logical = igraph.isdirected(adj);
       opts.normalized (1, 1) logical = true;
       opts.damping (1, 1) {mustBeInRange(opts.damping, 0, 1)} = 0.85;
    end

    method = lower(method);
    if strcmp(method, 'constraint')
        method = 'burt';
    end

    opts.vids = opts.vids - 1;
    values = mexIgraphDispatcher(mfilename(), adj, method, opts);
end

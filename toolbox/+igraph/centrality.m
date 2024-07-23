function values = centrality(graph, method, graphOpts, methodOpts, attribute)
%CENTRALITY calculate centrality measure in a graph
%   VALUES = CENTRALITY(GRAPH, METHOD) calculate the centrality for all nodes
%   in GRAPH using METHOD. METHOD can be one of 'closeness', 'harmonic',
%   'betweenness', 'pagerank', 'burt' (equivalently 'constraint').
%
%   VALUES = CENTRALITY(GRAPH, METHOD, ..., 'vids', NODES) select a subset of
%   nodes in the graph to calculate centrality on.
%
%   VALUES = CENTRALITY(GRAPH, METHOD, ..., 'directed', TF) if true, treat the
%   graph is directed (defaults to IGRAPH.ISDIRECTED).
%
%   GRAPH = CENTRALITY(GRAPH, METHOD, 'result', NAME) if GRAPH can hold
%   attributes, setting RESULT will determine the name of a node attribute to
%   store the results such that GRAPH.Nodes.NAME will be a vector of centrality
%   values. Note: The original GRAPH is not modified.
%
%   Method specific arguments:
%      'normalized'   logical for 'closeness', 'harmonic'; whether to return
%                     the mean (true) or sum (false) of the results (default
%                     true).
%
%      'damping'      value between 0 and 1, the 'pagerank' damping factor
%                     (default 0.85).
%
%      NOTE: Method specific arguments will be silently ignored if supplied
%      but not needed for the given method.
%
%   See also IGRAPH.ISDIRECTED.

    arguments
       graph {igutils.mustBeGraph};
       method (1, :) char ...
           {igutils.mustBeMemberi(method, ...
                                  {'closeness', 'harmonic', 'betweenness', ...
                                   'pagerank', 'burt', 'constraint'})};
       graphOpts.isdirected = igraph.isdirected(graph);
       methodOpts.vids (1, :) {mustBePositive, mustBeInteger} = ...
           1:igraph.numnodes(graph);
       methodOpts.mode (1, :) char {igutils.mustBeMode} = 'all';
       methodOpts.normalized (1, 1) logical = true;
       methodOpts.damping (1, 1) ...
           {mustBeInRange(methodOpts.damping, 0, 1)} = 0.85;
       attribute.results (1, :) char ...
           {igutils.mustHoldNodeAttr(graph, attribute.results)} = '';
    end

    method = lower(method);
    if strcmp(method, 'constraint')
        method = 'burt';
    end

    if ~isempty(attribute.results) && ...
            length(methodOpts.vids) < igraph.numnodes(graph)
        error("Cannot store results in graph as node attributes " + ...
              "when calculating centrality for only a subset of " + ...
              "nodes (i.e. when vids is not a vector of all node ids.)");
    end

    values = mexIgraphDispatcher(mfilename(), graph, method, graphOpts, ...
                                 methodOpts);

    if ~isempty(attribute.results)
        graph.Nodes.(attribute.results) = values';
        values = graph;
    end
end

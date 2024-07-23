function membership = cluster(graph, method, graphOpts, methodOpts)
%CLUSTER perform community detection on a graph
%   MEMBERSHIP = CLUSTER(GRAPH, METHOD) use METHOD to find a community
%       structure for the graph. See below for method specific options.
%
%   The allowed methods are: 'optimal', 'SpinGlass', 'LeadingEigenvector',
%   'walktrap', 'EdgeBetweenness', 'FastGreedy', 'multilevel', 'louvain',
%   'leiden', 'FluidCommunities', 'LabelPropagation' and 'infomap'.
%
%   Method names are case insensitive.
%
%   Methods 'optimal', 'FastGreedy', 'Multilevel' (or equivalently 'louvain'),
%   and 'leiden' all attempt to find the community structure of the graph that
%   maximizes Newman's modularity. Optimal finds the true maximal structure but
%   scales up poorly. The other three approximate the calculation to speed up
%   the process but do not guarantee they will find the optimal structure.
%   Additionally, Leiden can optimize with the constant Potts model (default)
%   instead of modularity which may be a better metric to optimize off of (see:
%   Santo Fortunato & Marc Barthélemy (2007) Resolution Limit in Community
%   Detection, Proceedings of the National Academy of Sciences. and V. A.
%   Traag, P. Van Dooren & Y. Nesterov (2011) Narrow Scope for
%   Resolution-Limit-Free Community Detection, Physical Review E.).
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'optimal') Find the community structure that
%       maximizes Newman's modularity. This algorithm is slow for larger graphs
%       and likely unfeasible for graphs beyond ~100 nodes. See
%       igraph.optimalModularity to find only the modularity.
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'SpinGlass', ...) Use the spinglass clustering
%       algorithm. Additional arguments:
%
%        Name             Description
%       --------------------------------------------------------------------
%        'nSpins'         Number of spins to perform (default 4). This is
%                         equivalently the maximum number of clusters (but the
%                         final number of clusters may be less).
%        'parallel'       Whether to perform spins in parallel (default true
%                         if all weights are positive otherwise false). Cannot
%                         be used with negative edge weights.
%        'tempStart'      The temperature at the start (default 1).
%        'tempEnd'        The temperature to stop at (default 0.01).
%        'coolingFactor'  Factor to determine how quickly the system cools
%                         (default 0.99).
%        'updateRule'     The null model to use. Can be 'simple' or 'config'
%                         (default 'simple'). Simple uses a simple random
%                         graph. Config uses a random graph with the same
%                         degree distribution as the adjacency matrix.
%        'resolution'     Higher resolution means breaking into smaller
%                         clusters (default 0.8).
%        'negResolution'  Resolution parameter for negative weights, if there
%                         are any (default 0.1).
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'LeadingEigenvector') Not implemented.
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'walktrap') A random walker based community
%       detection algorithm.
%
%        Name      Description
%       --------------------------------------------------------------------
%        'nSteps'  The length of random walks (default 4);
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'edgebetweenness') Community detection based on
%       separating communities by removing heavily traveled edges until
%       isolated islands forms. This assumes communities are well separated
%       except for a few hub edges that allow for cross-community
%       communication.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'isweighted'  Whether to treat the graphs as weighted or not (defaults
%                      result of IGRAPH.ISWEIGHTED).
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'fastgreedy') A fast approximation of the
%       community structure that maximizes Newman's modularity.
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'multilevel')
%   MEMBERSHIP = CLUSTER(GRAPH, 'louvain') Run the 'multilevel' community
%       detection algorithm, often referred to as 'Louvain'.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'resolution'  Larger resolutions favor smaller communities (default 1,
%                      the original multilevel definition).
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'leiden') A community detection algorithm based
%       off multilevel to improve some known weaknesses in the multilevel
%       algorithm. By default it uses the Constant Potts Model (CPM) instead of
%       modularity for optimizing. If unsure, Leiden is a good choice for an
%       optimization based algorithm.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'resolution'  Larger resolutions favor smaller communities (default 1,
%        'randomness'  How much randomness to use in the refinement stage (beta
%                      in the original paper). Default 0.01.
%        'nIterations' The number of iterations to use. By default runs until
%                      convergence. Some graphs may not converge leading to
%                      infinite loops, in which a cases this value should be
%                      set.
%        'metric'      The metric for optimization, either 'CPM' (default) or
%                      'modularity'.
%        'initial'     The starting community structure. If unset, start with
%                      each node in its own community. Community labels should
%                      start with 1.
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'fluidcommunities') A community detection
%       algorithm based on simulating interacting fluids flowing through the
%       graph. Edge weights will be ignored for this method.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'nCommunities' Number of communities to define (default 4). This value
%                       should be changed to something sensible for the graph.
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'labelpropagation') Cluster nodes based on the
%       labels of their neighbors.
%
%        Name      Description
%       --------------------------------------------------------------------
%        'mode'    Which direction labels should travel. Does not impact
%                  undirected graphs. For directed graphs, upstream igraph
%                  suggests leaving as 'all' (default, treat as undirected)
%                  unless you understand the consequences of using a directed
%                  network. Can also be 'out' for labels traveling in the
%                  direction of edges or 'in' for labels going in the opposite
%                  direction.
%        'initial' The initial labels for each node (defaults to each node
%                  being the sole member of it's own label). Labels start at 1.
%        'fixed'   A mask defining which members should be fixed in their in
%                  their original community. Should be a logical of length
%                  equal to the number of nodes in the graph. Defaults to all
%                  false (all nodes can be relabeled freely).
%
%   MEMBERSHIP = CLUSTER(GRAPH, 'infomap') The infomap community detection
%       algorithm.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nTrials'     Number of attempts to partition the network (default 1).
%        'nodeWeights' Weights of nodes, used as the likelihood of a random
%                      walker jumps to a given node when it gets stuck.
%                      Defaults to uniform likelihood.
%
%   See also igraph.modularity, igraph.compare.

    arguments
        graph {igraph.args.mustBeGraph};
        method (1, :) char ...
            {igraph.args.mustBeMemberi(method, ...
                                       {'optimal', 'spinglass', 'leadingeigenvector', ...
                                        'walktrap', 'edgebetweenness', 'fastgreedy', ...
                                        'multilevel', 'louvain', 'leiden', ...
                                        'fluidcommunities', 'labelpropagation', ...
                                        'infomap'})};

        graphOpts.isdirected (1, 1) logical = igraph.isdirected(graph);
        methodOpts.nSpins;
        methodOpts.parallel;
        methodOpts.tempStart;
        methodOpts.tempEnd;
        methodOpts.coolingFactor;
        methodOpts.updateRule;
        methodOpts.resolution;
        methodOpts.negResolution;
        methodOpts.initial;
        methodOpts.maxSteps;
        methodOpts.nSteps;
        methodOpts.isweighted;
        methodOpts.randomness;
        methodOpts.nIterations;
        methodOpts.metric;
        methodOpts.fixed;
        methodOpts.mode;
        methodOpts.nTrials;
        methodOpts.nodeWeights;
    end

    method = lower(method);
    if strcmp(method, 'louvain')
        method = 'multilevel';
    end

    methodOpts = namedargs2cell(methodOpts);
    switch method
      case 'optimal'
        methodOpts = parseNullOptions(graph, methodOpts{:});
      case 'spinglass'
        methodOpts = parseSpinglassOptions(graph, methodOpts{:});
      case 'leadingeigenvector'
        % Currently crashing Matlab with an memalloc error.
        error("igraph:notImplemented", ...
              "LeadingEigenvector currently not implemented.");
        methodOpts = parseLeadingEigenvectorOptions(graph, methodOpts{:});
      case 'walktrap'
        methodOpts = parseWalktrapOptions(graph, methodOpts{:});
      case 'edgebetweenness'
        methodOpts = parseEdgeBetweenness(graph, methodOpts{:});
      case 'fastgreedy'
        methodOpts = parseNullOptions(graph, methodOpts{:});
      case 'multilevel'
        methodOpts = parseMultilevelOptions(graph, methodOpts{:});
      case 'leiden'
        methodOpts = parseLeidenOptions(graph, methodOpts{:});
      case 'fluidcommunities'
        methodOpts = parseFluidCommunitiesOptions(graph, methodOpts{:});
      case 'labelpropagation'
        methodOpts = parseLabelPropagationOptions(graph, methodOpts{:});
      case 'infomap'
        methodOpts = parseInfomapOptions(graph, methodOpts{:});
    end

    membership = mexIgraphDispatcher(mfilename(), graph, method, graphOpts, ...
                                     methodOpts);
end

function opts = parseNullOptions(~)
    arguments
        ~
    end

    opts = struct();
end

function opts = parseSpinglassOptions(graph, opts)
    arguments
        graph %#ok<INUSA>
        opts.nSpins (1, 1) {mustBePositive, mustBeInteger} = 4;
        opts.parallel (1, 1) logical = min(graph) > 0;
        opts.tempStart (1, 1) {mustBeNumeric} = 1;
        opts.tempEnd (1, 1) {mustBeNumeric} = 0.01;
        opts.coolingFactor (1, 1) {mustBeNumeric} = 0.99;
        opts.updateRule (1, :) ...
            {igraph.args.mustBeMemberi(opts.updateRule, ...
                                       {'simple', 'config'})} = 'simple';
        opts.resolution (1, 1) {mustBeNumeric} = 0.8;
        opts.negResolution (1, 1) {mustBeNumeric} = 0.1;
    end

    opts.updateRule = lower(opts.updateRule);
end

function opts = parseLeadingEigenvectorOptions(graph, opts)
    arguments
        graph %#ok<INUSA>
        opts.maxSteps (1, 1) {mustBePositive, mustBeInteger} = 5;
        opts.initial (1, :) ...
            {igraph.args.mustBePartition} = 1:igraph.numnodes(graph)
    end
end

function opts = parseWalktrapOptions(~, opts)
    arguments
        ~
        opts.nSteps (1, 1) {mustBePositive, mustBeInteger} = 4;
    end
end

function opts = parseEdgeBetweenness(graph, opts)
    arguments
        graph %#ok<INUSA>
        opts.isweighted (1, 1) logical = igraph.isweighted(graph);
    end
end

function opts = parseMultilevelOptions(~, opts)
    arguments
        ~
        opts.resolution (1, 1) {mustBeNonnegative} = 1;
    end
end

function opts = parseLeidenOptions(graph, opts)
    arguments
        graph  %#ok<INUSA>
        opts.resolution (1, 1) {mustBeNumeric} = 1;
        opts.randomness (1, 1) {mustBePositive} = 0.01;
        opts.nIterations (1, 1) {mustBeInteger} = -1;
        opts.metric (1, :) char ...
            {igraph.args.mustBeMemberi(opts.metric, ...
                                       {'modularity', 'cpm', ...
                                        'constantpottsmodel'})} = 'cpm';
        opts.initial (1, :) ...
            {igraph.args.mustBePartition} = 1:igraph.numnodes(graph);
    end

    opts.metric = lower(opts.metric);
    if strcmp(opts.metric, 'constantpottsmodel')
        opts.metric = 'cpm';
    end
end

function opts = parseFluidCommunitiesOptions(~, opts)
    arguments
        ~
        opts.nCommunities (1, 1) {mustBePositive, mustBeInteger} = 1;
    end
end

function opts = parseLabelPropagationOptions(graph, opts)
    arguments
        graph %#ok<INUSA>
        opts.mode (1, :) char {igraph.args.mustBeMode} = 'all';
        opts.initial (1, :) ...
            {igraph.args.mustBePartition} = 1:igraph.numnodes(graph);
        opts.fixed (1, :) logical = false(1, igraph.numnodes(graph));
    end

    if length(opts.fixed) ~= length(opts.initial)
        throwAsCaller(MException("Igraph:wrongLength", ...
                                 "Option fixed must match " + ...
                                 "length of option initial"));
    end
end

function opts = parseInfomapOptions(graph, opts)
    arguments
        graph
        opts.nTrials (1, 1) {mustBePositive,mustBeInteger} = 1;
        opts.nodeWeights (1, :) {mustBePositive} = ...
            ones(1, igraph.numnodes(graph));
    end

    if length(opts.nodeWeights) ~= igraph.numnodes(graph)
        throwAsCaller(MException("Igraph:wrongLength", ...
                                 "The length of node weights must be" + ...
                                 " equal to the number of nodes in the" + ...
                                 " adjacency matrix."));
    end
end

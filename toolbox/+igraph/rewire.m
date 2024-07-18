function graph = rewire(graph, graphOpts, methodOpts)
%REWIRE alter a graph by rewiring its edges
%   GRAPH = REWIRE(GRAPH) rewire the GRAPH while preserving it's degree.
%   GRAPH = REWIRE(GRAPH, "PROBABILITY", P) rewire an edge with probability P.
%   GRAPH = REWIRE(GRAPH, "NREWIRES", N) rewire N edges.
%   GRAPH = REWIRE(GRAPH, "PRESERVEDEGREE", TF) If true (default) preserve the
%         degree (in and out) of each node. Requires mode to be 'all'. If
%         false, when rewiring, edges are taken from one node pair and move to
%         another at random.
%   GRAPH = REWIRE(GRAPH, "MODE", MODE) Whether to rewire both ends ('all',
%         default) only 'in' node or only 'out' node. Must be 'all' if
%         preserving degrees.
%   GRAPH = REWIRE(GRAPH, "LOOPS", TF) Whether to allow modifying existing loops
%         and creating new loops.
%
%   GRAPH = GAME(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) provide additional
%       output name-value pairs. The output defaults to the input's values.
%       If the input is a sparse logical the output will be a sparse logical.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'makeSparse'   Whether to return a sparse or full matrix.
%        'dtype'        The data type to use, either 'double' or 'logical'.
%        'isdirected'   Whether the resulting GRAPH should be directed or not.
%
%   See also IGRAPH.GENERATE, IGRAPH.RANDGAME, IGRAPH.FAMOUS,
%   IGRAPH.CORRELATEDPAIR.

    arguments
        graph {igraph.args.mustBeGraph};
        graphOpts.makeSparse (1, 1) logical = issparse(graph);
        graphOpts.dtype (1, :) char ...
            {igraph.args.mustBeMemberi(graphOpts.dtype, {'logical', 'double'})};
        methodOpts.isdirected (1, 1) logical = igraph.isdirected(graph);
        methodOpts.probability (1, 1) ...
            {mustBeInRange(methodOpts.probability, 0, 1)} = 0;
        methodOpts.nRewires (1, 1) {mustBeNonnegative, mustBeInteger} = 0;
        methodOpts.preserveDegree (1, 1) logical = true;
        methodOpts.mode (1, :) char {igraph.args.mustBeMode} = 'all';
        methodOpts.loops (1, 1) logical = false;
    end

    if methodOpts.preserveDegree && ~strcmp(methodOpts.mode, 'all')
        error("Igraph:overConstrained", "Cannot preserve degree without " + ...
              "modifying both ends of edges.");
    end

    if methodOpts.probability > 0 && methodOpts.nRewires > 0
        error("Igraph:overConstrained", "Cannot set both the " + ...
              "probability of a rewire and the number of rewires.");
    end

    if methodOpts.probability == 0 && methodOpts.nRewires == 0
        methodOpts.probability = 0.75;
    end

    if methodOpts.nRewires > nnz(graph)
        error("Igraph:tooManyRewires", "Cannot have more rewires than " + ...
              "edges.\nGraph only has %d edges requested %d rewires.", ...
              nnz(graph), methodOpts.nRewires);
    end

    if methodOpts.preserveDegree && methodOpts.nRewires == 0
        methodOpts.nRewires = ceil(methodOpts.probability * nnz(graph));
    end

    if ~methodOpts.preserveDegree && methodOpts.probability == 0
        methodOpts.probability = methodOpts.nRewires / nnz(graph);
    end

    if ~igraph.args.isoptionset(graphOpts, 'dtype')
        if islogical(graph)
            graphOpts.dtype = 'logical';
        else
            graphOpts.dtype = 'double';
        end
    end
    graphOpts.dtype = lower(graphOpts.dtype);

    graph = mexIgraphDispatcher(mfilename(), graph, graphOpts, methodOpts);
end

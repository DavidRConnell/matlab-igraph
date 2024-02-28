function adj = rewire(adj, adjOptions, methodOptions)
%REWIRE alter an adjacency matrix by rewiring its edges
%   ADJ = REWIRE(ADJ) rewire the ADJ while preserving it's degree.
%   ADJ = REWIRE(ADJ, "PROBABILITY", P) rewire an edge with probability P.
%   ADJ = REWIRE(ADJ, "NREWIRES", N) rewire N edges.
%   ADJ = REWIRE(ADJ, "PRESERVEDEGREE", TF) If true (default) preserve the
%         degree (in and out) of each node. Requires mode to be 'all'. If
%         false, when rewiring, edges are taken from one node pair and move to
%         another at random.
%   ADJ = REWIRE(ADJ, "MODE", MODE) Whether to rewire both ends ('all',
%         default) only 'in' node or only 'out' node. Must be 'all' if
%         preserving degrees.
%   ADJ = REWIRE(ADJ, "LOOPS", TF) Whether to allow modifying existing loops
%         and creating new loops.
%
%   ADJ = GAME(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) provide additional
%       output name-value pairs. The output defaults to the input's values.
%       If the input is a sparse logical the output will be a sparse logical.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'makeSparse'   Whether to return a sparse or full matrix.
%        'dtype'        The data type to use, either 'double' or 'logical'.
%        'isdirected'   Whether the resulting ADJ should be directed or not.
%
%   See also: igraph.generate, igraph.randgame, igraph.famous,
%             igraph.correlatedPair.

    arguments
        adj {mustBeAdj};
        adjOptions.makeSparse (1, 1) logical = issparse(adj);
        adjOptions.dtype (1, :) char ...
            {mustBeMemberi(adjOptions.dtype, {'logical', 'double'})};
        methodOptions.isdirected (1, 1) logical = igraph.isdirected(adj);
        methodOptions.probability (1, 1) ...
            {mustBeInRange(methodOptions.probability, 0, 1)} = 0;
        methodOptions.nRewires (1, 1) {mustBeNonnegative, mustBeInteger} = 0;
        methodOptions.preserveDegree (1, 1) logical = true;
        methodOptions.mode (1, :) char {mustBeMode} = 'all';
        methodOptions.loops (1, 1) logical = false;
    end

    if methodOptions.preserveDegree && ~strcmp(methodOptions.mode, 'all')
        error("Igraph:overConstrained", "Cannot preserve degree without " + ...
              "modifying both ends of edges.");
    end

    if methodOptions.probability > 0 && methodOptions.nRewires > 0
        error("Igraph:overConstrained", "Cannot set both the " + ...
              "probability of a rewire and the number of rewires.");
    end

    if methodOptions.probability == 0 && methodOptions.nRewires == 0
        methodOptions.probability = 0.75;
    end

    if methodOptions.nRewires > nnz(adj)
        error("Igraph:tooManyRewires", "Cannot have more rewires than " + ...
              "edges.\nGraph only has %d edges requested %d rewires.", ...
              nnz(adj), methodOptions.nRewires);
    end

    if methodOptions.preserveDegree && methodOptions.nRewires == 0
        methodOptions.nRewires = ceil(methodOptions.probability * nnz(adj));
    end

    if ~methodOptions.preserveDegree && methodOptions.probability == 0
        methodOptions.probability = methodOptions.nRewires / nnz(adj);
    end

    if ~isoptionset(adjOptions, 'dtype')
        if islogical(adj)
            adjOptions.dtype = 'logical';
        else
            adjOptions.dtype = 'double';
        end
    end
    adjOptions.dtype = lower(adjOptions.dtype);

    adj = mexIgraphDispatcher(mfilename(), adj, adjOptions, methodOptions);
end

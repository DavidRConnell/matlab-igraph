function opts = parseOptionsStaticFitness(opts)
    arguments
        opts.fitness (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.outFitness (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.inFitness (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.nEdges (1, 1) {mustBeInteger, mustBePositive} = 20;
        opts.loops (1, 1) logical = false;
    end

    if isoptionset(opts, "fitness") && ...
            (isoptionset(opts, "outFitness") || isoptionset(opts, "inFitness"))
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "fitness should be set for undirected " + ...
                                 "graphs. Both inFitness and outFitness " + ...
                                 "should be set for directed graphs. " + ...
                                 "Cannot set both fitness and outFitness " + ...
                                 "or inFitness."));
    end

    if (isoptionset(opts, "outFitness") && ~isoptionset(opts, "inFitness")) || ...
            (isoptionset(opts, "inFitness") && ~isoptionset(opts, "outFitness"))
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "fitness should be set for undirected " + ...
                                 "graphs. Both inFitness and outFitness " + ...
                                 "should be set for directed graphs. " + ...
                                 "If outFitness is set inFitness must " + ...
                                 "also be set."));
    end

    if isoptionset(opts, "outFitness")
        if length(opts.outFitness) ~= length(opts.inFitness)
            throwAsCaller(MException("Igraph:badArgument", ...
                                     "outFitness and inFitness must have " + ...
                                     "the same length."))
        end
    end

    if ~isoptionset(opts, "inFitness")
        opts.inFitness = [];
    end

    % opts.fitness just for user interface. Internally use outFitness like
    % igraph.
    if isoptionset(opts, "fitness")
        opts.outFitness = opts.fitness;
    end

    if ~isoptionset(opts, "outFitness")
        opts.outFitness = ones(1, 10) * 3;
    end
end

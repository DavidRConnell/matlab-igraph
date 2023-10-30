function opts = parseOptionsDegSeq(opts)
    arguments
        opts.degree (1, :) {mustBeNonnegative, mustBeInteger};
        opts.outDegree (1, :) {mustBeNonnegative, mustBeInteger};
        opts.inDegree (1, :) {mustBeNonnegative, mustBeInteger};
    end

    if isoptionset(opts, "degree") && ...
            (isoptionset(opts, "outDegree") || isoptionset(opts, "inDegree"))
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "degree should be set for undirected " + ...
                                 "graphs. Both inDegree and outDegree " + ...
                                 "should be set for directed graphs. " + ...
                                 "Cannot set both degree and outDegree " + ...
                                 "or inDegree."));
    end

    if (isoptionset(opts, "outDegree") && ~isoptionset(opts, "inDegree")) || ...
            (isoptionset(opts, "inDegree") && ~isoptionset(opts, "outDegree"))
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "degree should be set for undirected " + ...
                                 "graphs. Both inDegree and outDegree " + ...
                                 "should be set for directed graphs. " + ...
                                 "If outDegree is set inDegree must " + ...
                                 "also be set."));
    end

    if isoptionset(opts, "outDegree")
        if length(opts.outDegree) ~= length(opts.inDegree)
            throwAsCaller(MException("Igraph:badArgument", ...
                                     "outDegree and inDegree must have " + ...
                                     "the same length."))
        end
    end

    if ~isoptionset(opts, "inDegree")
        opts.inDegree = [];
    end

    % opts.degree just for user interface. Internally use outDegree like igraph.
    if isoptionset(opts, "degree")
        opts.outDegree = opts.degree;
    end

    if ~(isoptionset(opts, "degree") || isoptionset(opts, "outDegree"))
        opts.outDegree = ones(1, 10) * 3;
    end
end

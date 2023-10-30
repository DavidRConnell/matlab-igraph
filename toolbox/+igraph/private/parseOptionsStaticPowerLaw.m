function opts = parseOptionsStaticPowerLaw(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive} = 10;
        opts.nEdges (1, 1) {mustBeInteger, mustBePositive} = 20;
        opts.exponent (1, 1) {mustBeNumeric};
        opts.outExponent (1, 1) {mustBeNumeric};
        opts.inExponent (1, 1) {mustBeNumeric};
        opts.loops (1, 1) logical = false;
        opts.finiteSizeCorrection (1, 1) logical = false;
    end

    if isoptionset(opts, "exponent") && ...
            (isoptionset(opts, "outExponent") || ...
             isoptionset(opts, "inExponent"))
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "exponent should be set for undirected " + ...
                                 "graphs. Both inExponent and outExponent " + ...
                                 "should be set for directed graphs. " + ...
                                 "Cannot set both exponent and " + ...
                                 "outExponent or inExponent."));
    end

    if (isoptionset(opts, "outExponent") && ...
        ~isoptionset(opts, "inExponent")) || ...
        (isoptionset(opts, "inExponent") && ...
         ~isoptionset(opts, "outExponent"))
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "exponent should be set for undirected " + ...
                                 "graphs. Both inExponent and outExponent " + ...
                                 "should be set for directed graphs. " + ...
                                 "If outExponent is set inExponent must " + ...
                                 "also be set."));
    end

    if ~isoptionset(opts, "inExponent")
        opts.inExponent = -1;
    end

    % opts.exponent just for user interface. Internally use outExponent like
    % igraph.
    if isoptionset(opts, "exponent")
        opts.outExponent = opts.exponent;
    end

    if ~isoptionset(opts, "outExponent")
        opts.outExponent = 2;
    end

    if opts.inExponent >= 0 && opts.inExponent < 2
        throwAsCaller(MException("Igraph:badValue", ...
                                 "If inExponent is set it must be at least 2."));

    end

    if opts.outExponent >= 0 && opts.outExponent < 2
        throwAsCaller(MException("Igraph:badValue", ...
                                 "exponent or outExponent must be at least 2."));

    end

end

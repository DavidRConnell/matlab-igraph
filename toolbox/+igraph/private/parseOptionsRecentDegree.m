function opts = parseOptionsRecentDegree(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger};
        opts.degreeExp (1, 1) {mustBeNumeric} = 1;
        opts.timeWindow (1, 1) {mustBePositive, mustBeInteger} = 3;
        opts.edgesPerStep (1, :) {mustBeInteger, mustBePositive} = 3;
        opts.outPreference (1, 1) logical = false;
        opts.zeroAppeal (1, 1) {mustBeNonnegative, ...
                                mustBeLessThan(opts.zeroAppeal, 1)} = 0.1;
        opts.isdirected (1, 1) logical = false;
    end

    if ~isoptionset(opts, "nNodes")
        if length(opts.edgesPerStep) > 1
            opts.nNodes = length(opts.edgesPerStep);
        else
            opts.nNodes = 10;
        end
    end

    if length(opts.edgesPerStep) > 1 && ...
            length(opts.edgesPerStep) ~= opts.nNodes
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Edges Per time step must be a scalar " + ...
                                 "or a vector of length nNodes."));
    end

    if length(opts.edgesPerStep) > 1
        opts.outSeq = opts.edgesPerStep;
        opts.edgesPerStep = 1; % Ignored when outSeq not empty.
    else
        opts.outSeq = [];
    end
end

function opts = parseOptionsPreference(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive};
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBePositive} = 4;
        opts.nTypes (1, 1) {mustBeInteger, mustBePositive};
        opts.typeDistribution (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.blockSizes (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.preference (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.isdirected (1, 1) logical;
        opts.loops (1, 1) logical = false;
    end

    if isoptionset(opts, "typeDistribution") && isoptionset(opts, "blockSizes")
        throwAsCaller(MException("Igraph:overConstrained", ...
                                 "typeDistribution and blockSizes " + ...
                                 "cannot both be set."));
    end

    opts.fixedSizes = false;
    if isoptionset(opts, "blockSizes")
        if isoptionset(opts, "nNodes") && ...
                ~(opts.nNodes == sum(opts.blockSizes))
            throwAsCaller(MException("Igraph:overConstrained", ...
                                     "nTypes must be equal to the sum of " + ...
                                     "blockSizes. Leave nTypes blank " + ...
                                     "to calculate based on blockSizes."));
        else
            opts.nNodes = sum(opts.blockSizes);
        end
        opts.fixedSizes = true;
        opts.typeDistribution = opts.blockSizes;
    end

    if ~isoptionset(opts, "nNodes")
        opts.nNodes = 10;
    end

    if ~isoptionset(opts, "nTypes")
        if ~(isoptionset(opts, "typeDistribution") || ...
             isoptionset(opts, "preference"))
            opts.nTypes = 4;
        elseif isoptionset(opts, "typeDistribution")
            opts.nTypes = length(opts.typeDistribution);
        else
            opts.nTypes = length(opts.preference);
        end
    end

    if ~isoptionset(opts, "typeDistribution")
       opts.typeDistribution = ones(1, opts.nTypes) / opts.nTypes;
    end

    if ~isoptionset(opts, "preference")
       opts.preference = ones(opts.nTypes, opts.nTypes)  / opts.nTypes;
    end

    if length(opts.typeDistribution) ~= opts.nTypes
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Type distribution must be of length nTypes"));
    end

    if length(opts.preference) ~= opts.nTypes || ...
            (size(opts.preference, 1) ~= size(opts.preference, 2))
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Preference must be a square matrix " + ...
                                 "with sides of length nTypes"));
    end

    if ~isoptionset(opts, "isdirected")
        opts.isdirected = ~issymmetric(opts.preference);
    end

    if ~opts.isdirected && ~issymmetric(opts.preference)
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Preference matrix must be symmetric " + ...
                                 "if graph is undirected"));
    end
end

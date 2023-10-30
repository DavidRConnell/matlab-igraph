function opts = parseOptionsCallawayTraits(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBeNonnegative} = 10;
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBePositive} = 4;
        opts.nTypes (1, :) {mustBeInteger, mustBePositive};
        opts.typeDistribution (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.preference (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.isdirected (1, 1) logical;
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

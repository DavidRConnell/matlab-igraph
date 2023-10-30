function opts = parseOptionsAsymmetricPreference(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive} = 10;
        opts.nOutTypes (1, 1) {mustBeInteger, mustBePositive};
        opts.nInTypes (1, 1) {mustBeInteger, mustBePositive};
        opts.typeDistribution (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.preference (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.loops (1, 1) logical = false;
    end

    if ~isoptionset(opts, "nOutTypes")
        if ~(isoptionset(opts, "typeDistribution") || ...
             isoptionset(opts, "preference"))
            opts.nOutTypes = 4;
        elseif isoptionset(opts, "typeDistribution")
            opts.nOutTypes = size(opts.typeDistribution, 1);
        else
            opts.nOutTypes = size(opts.preference, 1);
        end
    end

    if ~isoptionset(opts, "nInTypes")
        if ~(isoptionset(opts, "typeDistribution") || ...
             isoptionset(opts, "preference"))
            opts.nInTypes = 4;
        elseif isoptionset(opts, "typeDistribution")
            opts.nInTypes = size(opts.typeDistribution, 2);
        else
            opts.nInTypes = size(opts.preference, 2);
        end
    end

    if ~isoptionset(opts, "typeDistribution")
        opts.typeDistribution = ones(opts.nOutTypes, opts.nInTypes) / ...
            (opts.nOutTypes * opts.nInTypes);
    end

    if ~isoptionset(opts, "preference")
        opts.preference = ones(opts.nOutTypes, opts.nInTypes)  / ...
            (opts.nOutTypes * opts.nInTypes);
    end

    if (size(opts.typeDistribution, 1) ~= opts.nOutTypes) || ...
            (size(opts.typeDistribution, 2) ~= opts.nInTypes)
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Type distribution must be a matrix " + ...
                                 "of size nOutTypes x nInTypes"));
    end

    if (size(opts.preference, 1) ~= opts.nOutTypes) || ...
            (size(opts.preference, 2) ~= opts.nInTypes)
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Preference must be a matrix of size " + ...
                                 "nOutTypes x nInTypes"));
    end
end

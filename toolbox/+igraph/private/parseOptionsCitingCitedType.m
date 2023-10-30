function opts = parseOptionsCitingCitedType(opts)
    arguments
        opts.edgesPerStep (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.types (1, :) {mustBePositive, mustBeInteger} = 1:10;
        opts.preference (:, :) {mustBeNumeric, mustBeSquare};
        opts.isdirected (1, 1) logical = false;
    end

    opts.nNodes = length(opts.types);

    if ~isoptionset(opts, "preference")
        opts.preference = ones(max(opts.types), max(opts.types));
    end

    if length(opts.preference) ~= max(opts.types)
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Preference must be square with side " + ...
                                 "lengths equal to the number of types " + ...
                                 "(i.e. max(types))."));
    end

    opts.types = opts.types - 1;
end

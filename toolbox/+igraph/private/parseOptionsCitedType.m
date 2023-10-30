function opts = parseOptionsCitedType(opts)
    arguments
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBeNonnegative} = 3;
        opts.types (1, :) {mustBeInteger, mustBePositive} = 1:10;
        opts.preference (1, :) {mustBeNonnegative};
        opts.isdirected (1, 1) logical = false;
    end

    opts.nNodes = length(opts.types);

    if ~isoptionset(opts, "preference")
        opts.preference = ones(1, max(opts.types));
    end

    if length(opts.preference) ~= max(opts.types)
        throwAsCaller(MException("Igraph:wrongSize", ...
                                 "Size of preference must be the number " + ...
                                 "of types (i.e. max(types))."));
    end

    opts.types = opts.types - 1;
end

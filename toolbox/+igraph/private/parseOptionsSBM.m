function opts = parseOptionsSBM(opts)
    arguments
        opts.blockSizes (1, :) {mustBePositive, mustBeInteger} = 2 * ones(1, 5);
        opts.preference (:, :) ...
            {mustBeInRange(opts.preference, 0, 1), mustBeSquare};
        opts.isdirected (1, 1) logical = false;
        opts.loops (1, 1) logical = false;
    end

    if ~isoptionset(opts, "preference")
        opts.preference = ones(length(opts.blockSizes)) / ...
            (length(opts.blockSizes) ^ 2);
    end

    if length(opts.preference) ~= length(opts.blockSizes)
        throwAsCaller(MException("Igraph:badArgument", ...
                                 "Preference must be square with side " + ...
                                 "lengths equal to the number of groups " + ...
                                 "(i.e. length(blockSizes))."));
    end

    opts.nNodes = sum(opts.blockSizes);
end

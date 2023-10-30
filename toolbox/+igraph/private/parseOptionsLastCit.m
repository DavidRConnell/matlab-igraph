function opts = parseOptionsLastCit(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBeNonnegative} = 10;
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBeNonnegative} = 3;
        opts.agingBins (1, 1) {mustBeInteger, mustBePositive};
        opts.preference (1, :) {mustBeInteger, mustBeNonnegative};
        opts.isdirected (1, 1) logical = false;
    end

    if ~isoptionset(opts, "agingBins")
        opts.agingBins = opts.nNodes;
    end

    if ~isoptionset(opts, "preference")
        opts.preference = ones(1, opts.agingBins + 1);
    end

    if length(opts.preference) ~= opts.agingBins + 1
        throwAsCaller(MException("Igraph:wrongSize", ...
                                 "preference must be length agingBins + 1."));
    end
end

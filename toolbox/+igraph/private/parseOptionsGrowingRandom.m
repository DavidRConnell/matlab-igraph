function opts = parseOptionsGrowingRandom(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive} = 10;
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBePositive} = 4;
        opts.isdirected (1, 1) logical = false;
        opts.citation (1, 1) logical = false;
    end
end

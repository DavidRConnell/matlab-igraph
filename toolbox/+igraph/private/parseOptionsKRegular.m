function opts = parseOptionsKRegular(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.degree (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.isdirected (1, 1) logical = false;
    end
end

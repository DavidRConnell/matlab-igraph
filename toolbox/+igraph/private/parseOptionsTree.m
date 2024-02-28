function opts = parseOptionsTree(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.isdirected (1, 1) logical = false;
        opts.method (1, :) char;
    end
end

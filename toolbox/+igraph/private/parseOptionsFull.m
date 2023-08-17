function opts = parseOptionsFull(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.isdirected (1, 1) logical = false;
        opts.loops (1, 1) logical = false;
    end
end

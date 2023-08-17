function opts = parseOptionsCirculant(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.shifts {mustBeNonnegative, mustBeVector, mustBeInteger} = [1];
        opts.isdirected (1, 1) logical = false;
    end
end

function opts = parseOptionsGrg(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.radius (1, 1) {mustBeNonnegative} = 5;
        opts.torus (1, 1) logical = false;
    end
end

function opts = parseOptionsWattsStrogatz(opts)
    arguments
        opts.dim (1, 1) {mustBeNonnegative, mustBeInteger} = 2;
        opts.size (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
        opts.radius (1, 1) {mustBeNonnegative, mustBeInteger} = 1;
        opts.probability (1, 1) ...
            {mustBeNonnegative, ...
             mustBeLessThanOrEqual(opts.probability, 1)} = 0.2;
        opts.loops (1, 1) logical = false;
    end
end

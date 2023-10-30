function opts = parseOptionsForestFire(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.forwardProbability (1, 1) ...
            {mustBeInRange(opts.forwardProbability, 0, 1)} = 0.5;
        opts.backwardFactor (1, 1) {mustBeNonnegative, mustBeNumeric} = 1;
        opts.nAmbassadors (1, 1) {mustBeInteger, mustBePositive} = 3;
        opts.isdirected (1, 1) logical = false;
    end
end

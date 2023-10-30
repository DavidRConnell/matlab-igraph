function opts = parseOptionsHSBM(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 20;
        opts.blockSizes (1, 1) {mustBeNonnegative, mustBeInteger} = 5;
        opts.rho (1, :) {mustBeInRange(opts.rho, 0, 1)} = [2 2 1] / 5;
        opts.preference (:, :) {mustBeNonNegative, mustBeSquare};
        opts.pOut (1, 1) {mustBeInRange(opts.pOut, 0, 1)} = 0.3;
    end

    if ~isoptionset(opts, "preference")
        opts.preference = ones(length(opts.rho));
    end
end

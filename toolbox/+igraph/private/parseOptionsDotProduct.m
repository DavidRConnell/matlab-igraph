function opts = parseOptionsDotProduct(opts)
    arguments
        opts.positions (:, :) {mustBeNumeric} = ones(3, 10) / 3;
        opts.isdirected (1, 1) logical = false;
    end
end

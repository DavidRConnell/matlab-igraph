function opts = parseOptionsStar(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.mode (1, :) char {mustBeMemberi(opts.mode, ...
                                             {'out', 'in', 'mutual', ...
                                              'undirected'})} = 'undirected';
        opts.center (1, 1) {mustBePositive, mustBeInteger} = 1;
    end

    if opts.center > opts.nNodes
        error("Invalid value for 'center' argument. Value must be less " + ...
              "than 'nNodes'.");
    end

    opts.center = opts.center - 1;
end

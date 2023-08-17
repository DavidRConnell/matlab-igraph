function opts = parseOptionsRegularTree(opts)
    arguments
        opts.height (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
        opts.degree (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.mode (1, :) char {mustBeMemberi(opts.mode, ...
                                             {'out', 'in', 'undirected'})} = ...
                                             'undirected';
    end

    opts.mode = lower(opts.mode);
end

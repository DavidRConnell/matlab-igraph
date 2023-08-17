function opts = parseOptionsKaryTree(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.children (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.mode (1, :) char {mustBeMemberi(opts.mode, ...
                                             {'out', 'in', 'undirected'})} = ...
                                             'undirected';
    end

    opts.mode = lower(opts.mode);
end

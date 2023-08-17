function opts = parseOptionsKautz(opts)
    arguments
        opts.nLetters (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.stringLength (1, 1) {mustBeNonnegative, mustBeInteger} = 2;
    end
end

function opts = parseOptionsPetersen(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 5;
        opts.shift (1, 1) {mustBeNonnegative, mustBeInteger} = 2;
    end

    if opts.nNodes < 3
       error("nNodes must be at least 3.");
    end

    if opts.shift > (opts.nNodes / 2)
        error("shift must be less than nNodes / 2.\n\n" + ...
              "nNodes = %d, shift = %d, shift must be less than %g", ...
              opts.nNodes, opts.shift, opts.nNodes / 2);
    end
end

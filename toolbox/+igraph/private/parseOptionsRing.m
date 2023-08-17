function opts = parseOptionsRing(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.isdirected (1, 1) logical;
        opts.ismutual (1, 1) logical = false;
        opts.iscircular (1, 1) logical = false;
    end

    if ~isoptionset(opts, 'isdirected')
        if opts.ismutual
            opts.isdirected = true;
        else
            opts.isdirected = false;
        end
    end

    if opts.ismutual && ~opts.isdirected
        error("Igraph:generateRing", ...
              "Cannot create mutual edges when graph is not directed. " + ...
             "ismutual can only be true when isdirected is true.")
    end
end

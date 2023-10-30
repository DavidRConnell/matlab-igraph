function opts = parseOptionsErdosRenyi(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.nEdges (1, 1) {mustBeNonnegative, mustBeInteger};
        opts.probability (1, 1) ...
            {mustBeNonnegative, mustBeLessThanOrEqual(opts.probability, 1)};
        opts.isdirected (1, 1) logical = false;
        opts.loops (1, 1) logical = false;
    end

    if (~isoptionset(opts, "nEdges")) && (~isoptionset(opts, "probability"))
        opts.nEdges = 10;
    elseif (isoptionset(opts, "nEdges") && (isoptionset(opts, "probability")))
        throwAsCaller(MException("Igraph:badOptions", ...
                                 "Cannot set both probability and " + ...
                                 "number of edges."));
    end
end

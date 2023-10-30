function opts = parseOptionsBarabasi(method, opts)
    arguments
        method;
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger};
        opts.power (1, 1) {mustBeNonnegative} = 1;
        opts.nConnections (1, :) {mustBeNonnegative, mustBeInteger} = 1;
        opts.outPreference (1, 1) logical = false;
        opts.attractiveness (1, 1) {mustBeNumeric} = 1;
        opts.isdirected (1, 1) logical;
        opts.startFrom {mustBeAdj} = [];
    end

    if isoptionset(opts, "nNodes")
        if (length(opts.nConnections) > 1) && ...
           ((length(opts.nConnections)) ~= opts.nNodes)
            eid = "Igraph:invalidVectorLength";
            msg = "If out sequence is not a scalar it must have " + ...
                  "length equal to the number of nodes.";
           throwAsCaller(MException(eid, msg));
        end
    else
        if length(opts.nConnections) == 1
            opts.nNodes = 10;
        else
            opts.nNodes = length(opts.nConnections);
        end
    end

    if method == "barabasibag" && opts.power ~= 1
        eid = "Igraph:badPower";
        throwAsCaller(MException(eid, ...
                                 "Power must be 1 when using BarabasiBag."));
    end

    if method == "barabasibag" && opts.attractiveness ~= 1
        eid = "Igraph:badAttractiveness";
        throwAsCaller(MException(eid, "Attractiveness must be 1 when " + ...
                                  "using BarabasiBag."));
    end

    if (~opts.outPreference) && (opts.attractiveness <= 0)
        eid = "igraph:badAttractiveness";
        throwAsCaller(MException(eid, "Attractiveness must be positive " + ...
                                 "when out preference is false."))
    end

    if ~isoptionset(opts, "isdirected")
       if isempty(opts.startFrom)
           opts.isdirected = false;
       else
           opts.isdirected = igraph.isdirected(opts.startFrom);
       end
    end
end

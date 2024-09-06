function mustBePartition(membership)
%MUSTBEPARTITION assert the argument is a valid membership vector

    eid = "igraph:invalidVertixVector";
    msg = "";

    if ~isnumeric(membership) && ~islogical(membership)
        msg = "Membership vector must be numeric or logical";
    end

    if min(membership) < 1
        msg = "Node IDs must start at 1";
    end

    if ~allfinite(membership) || ~all(membership == floor(membership), 'all')
        msg = "All nodes IDs must be integers"
    end

    if ~isreal(membership)
        msg = "Membership vector must be real";
    end

    if ~strcmp(msg, "")
        msg = "Value must be a membership vector\n\n" + msg;
        throwAsCaller(MException(eid, msg));
    end
end

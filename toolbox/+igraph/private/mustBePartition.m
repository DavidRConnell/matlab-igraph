function mustBePartition(membership)
%MUSTBEPARTITION assert the argument is a valid membership vector

    eid = "Igraph:NotMembershipVector";
    msg = "";
    if ~((isnumeric(membership) || islogical(membership)))
        msg = "Membership vector must be numeric or logical";
    end

    if ~(isreal(membership))
        msg = "Membership vector must be real";
    end

    if ~(isvector(membership))
        msg = "Membership vector must be a vector";
    end

    if ~strcmp(msg, "")
        msg = "Value must be a membership vector\n\n" + msg;
        throwAsCaller(MException(eid, msg));
    end
end

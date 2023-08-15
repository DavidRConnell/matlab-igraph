function mustBeAdj(adj)
%MUSTBEADJ assert the argument is a valid adjacency matrix

    eid = "Igraph:NotAdj";
    msg = "";
    if ~((isnumeric(adj) || islogical(adj)))
        msg = "Adjacency matrix must be numeric or logical";
    end

    if ~(isreal(adj))
        msg = "Adjacency matrix must be real";
    end

    if ~(ismatrix(adj))
        msg = "Adjacency matrix must be a matrix";
    end

    if ~strcmp(msg, "")
        msg = "Value must be an adjacency matrix\n\n" + msg;
        throwAsCaller(MException(eid, msg));
    end
end

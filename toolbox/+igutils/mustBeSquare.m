function mustBeSquare(mat)
%MUSTBESQUARE assert the matrix is square

    if ~(size(mat, 1) == size(mat, 2))
        eid = "Igraph:NotSquare";
        msg = "Matrix must be square.";
        throwAsCaller(MException(eid, msg));
    end
end

function mustBeGraph(graph)
%MUSTBEGRAPH assert the argument is a valid graph

    eid = "Igraph:NotGraph";
    msg = "";

    if isa(graph, "graph") || isa(graph, "digraph")
        return
    end

    if ~((isnumeric(graph) || islogical(graph)))
        msg = "Adjacency matrix must be numeric or logical";
    end

    if ~(isreal(graph))
        msg = "Adjacency matrix must be real";
    end

    if ~(ismatrix(graph))
        msg = "Adjacency matrix must be a matrix";
    end

    if ~strcmp(msg, "")
        msg = "Value must be a graph or adjacency matrix\n\n" + msg;
        throwAsCaller(MException(eid, msg));
    end
end

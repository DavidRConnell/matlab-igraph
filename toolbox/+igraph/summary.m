function summary(graph)
%SUMMARY display information about a graph
%   SUMMARY(GRAPH) provide a brief summary about GRAPH.

    arguments
        graph {igutils.mustBeGraph}
    end

    if igraph.isweighted(graph)
        weighted = "Weighted";
    else
        weighted = "Unweighted";
    end

    if igraph.isdirected(graph)
        directed = "Directed";
    else
        directed = "Undirected";
    end

    nNodes = igraph.numnodes(graph);
    nEdges = igraph.numedges(graph);
    edgeDensity = igraph.edgeDensity(graph);

    fprintf("\n    %s and %s igraph with:\n\n" + ...
            "     %d nodes\n     %d edges\n     %0.2g edge density\n\n", ...
            weighted, directed, nNodes, nEdges, edgeDensity);
end

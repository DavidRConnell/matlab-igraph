function n = numedges(graph)
%NUMEDGES count the number of edges in a graph
%   N = NUMEDGES(GRAPH) return the number of edges in GRAPH independent of the
%   graph's representation.
%
%   See also IGRAPH.NUMNODES.

    arguments
        graph {igutils.mustBeGraph};
    end

    if igutils.isgraph(graph)
        n = graph.numedges();
    elseif issymmetric(graph)
        n = nnz(graph);
        nDiag = sum(diag(graph) ~= 0);
        n = ((n - nDiag) / 2) + nDiag;
    else
        n = nnz(graph);
    end
end

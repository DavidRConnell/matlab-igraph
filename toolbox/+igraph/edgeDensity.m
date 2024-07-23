function d = edgeDensity(graph)
%EDGEDENSITY calculate the edge density of a graph
%   D = EDGEDENSITY(GRAPH) calculates the number of edges present in GRAPH
%   divided by the max number of edges GRAPH could hold. In the case of an
%   undirected graph, number of edges are the unique edges. If the input is a
%   multigraph, the edge density will be incorrect sense there is no limit to
%   the number of edges that could be stored.
%
%   See also IGRAPH.NUMEDGES.

    arguments
        graph {igraph.args.mustBeGraph};
    end

    nNodes = igraph.numnodes(graph);
    if igraph.isdirected(graph)
        possibleEdges = nNodes ^ 2;
    else
        possibleEdges = nNodes + (nNodes * (nNodes - 1) / 2);
    end

    d = igraph.numedges(graph) / possibleEdges;
end

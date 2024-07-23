function n = numnodes(graph)
%NUMNODES count the number of nodes in a graph
%   N = NUMNODES(GRAPH) return the number of nodes in GRAPH independent of the
%   graph's representation.
%
%   See also IGRAPH.NUMEDGES.

    arguments
        graph {igraph.args.mustBeGraph};
    end

    if igraph.args.isgraph(graph)
        n = graph.numnodes();
    else
        n = size(graph, 2);
    end
end

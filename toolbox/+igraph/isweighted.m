function TF = isweighted(graph)
%ISWEIGHTED guess if a graph is weighted
%   TF = ISWEIGHTED(GRAPH) return true if GRAPH contains values other than 0
%   and 1.
%
%   See also IGRAPH.ISDIRECTED.

    if igutils.isgraph(graph)
        TF = igutils.hasEdgeAttr(graph);
    else
        TF = sum(graph == 1 | graph == 0, 'all') ~= numel(graph);
    end
end

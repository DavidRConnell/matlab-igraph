function TF = isweighted(graph)
%ISWEIGHTED guess if a graph is weighted
%   TF = ISWEIGHTED(GRAPH) return true if GRAPH contains values other than 0
%   and 1.
%
%   See also IGRAPH.ISDIRECTED.

    TF = sum(graph == 1 | graph == 0, 'all') ~= numel(graph);
end

function TF = isdirected(graph)
%ISDIRECTED guess if a graph is directed.
%   TF = ISDIRECTED(GRAPH) returns false if the GRAPH is symmetric or a upper
%   or lower triangular matrix, true otherwise.
%
%   See also IGRAPH.ISWEIGHTED.

    % NOTE: Using igraph wrapper instead of matlab builtins because tril/triu
    % do not work with logicals.
    TF = mexIgraphIsDirected(graph);
end

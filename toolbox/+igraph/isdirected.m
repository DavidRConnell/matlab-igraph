function TF = isdirected(adj)
%ISDIRECTED guess if a graph is directed.
%   TF = ISDIRECTED(ADJ) returns false if the ADJ is symmetric or a upper or lower
%   triangular matrix, true otherwise.
%
%   See also igraph.isweighted

    % NOTE: Using igraph wrapper instead of matlab builtins because tril/triu
    % do not work with logicals.
    TF = mexIgraphIsDirected(adj);
end

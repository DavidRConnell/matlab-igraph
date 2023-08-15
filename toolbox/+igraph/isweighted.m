function TF = isweighted(adj)
%ISWEIGHTED guess if a graph is weighted
%   TF = ISWEIGHTED(ADJ) return true if ADJ contains values other than 0 and 1.
%
%   See also igraph.isdirected

    TF = sum(adj == 1 | adj == 0, 'all') ~= numel(adj);
end

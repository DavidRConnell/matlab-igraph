function adj = rewire(adj, method, adjOptions, methodOptions)
% USE: igraph_rewire_edges, igraph_rewire_directed_edges, igraph_connect_neighborhood
% (https://igraph.org/c/doc/igraph-Operators.html#miscellaneous-operators),
%
% Look closer at rewire_edges and rewired_directed_edges, may want to use a
% optional like in, out, both to combine them into a single function.

    arguments
        adj {mustBeAdj};
        method {mustBeMemberi(method, ...
                              {'probability', 'correlated', 'trial', ...
                               'neighborhood'})}
        adjOptions.makeSparse (1, 1) logical = issparse(adj);
        adjOptions.dtype mustBeMemberi(adjOptions.dtype, {'logical', 'double'});
    end

    method = lower(method);
    adjOptions.dtype = lower(adjOptions.dtype);

    if ~isoptionset(adjOptions, 'dtype')
        if islogical(adj)
            adjOptions.dtype = 'logical';
        else
            adjOptions.dtype = 'double';
        end
    end

    switch method
        case 'probability'
    end
end

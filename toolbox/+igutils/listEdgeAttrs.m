function attrs = listEdgeAttrs(graph)
%LISTEDGEATTRS list the graph's edge attributes
%
%   See also IGUTILS.HASEDGEATTR.

    attrs = graph.Edges.Properties.VariableNames;
    attrs = attrs(~strcmp(attrs, 'EndNodes'));
end

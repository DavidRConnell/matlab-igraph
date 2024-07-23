function attrs = listEdgeAttrs(graph)
%LISTEDGEATTRS list the graph's edge attributes

    attrs = graph.Edges.Properties.VariableNames;
    attrs = attrs(~strcmp(attrs, 'EndNodes'));
end

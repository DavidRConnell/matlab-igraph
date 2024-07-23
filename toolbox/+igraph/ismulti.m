function TF = ismulti(graph)
%ISMULTI check if graph is a multigraph.

    TF = false;
    if (isa(graph, 'graph') || isa(graph, 'digraph'))
        TF = ismultigraph(graph);
    end
end

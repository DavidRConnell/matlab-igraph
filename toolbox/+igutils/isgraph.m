function TF = isgraph(g)
%ISGRAPH determines if argument is either an object of graph or digraph class

    TF = isa(g, "graph") || isa(g, "digraph");
end

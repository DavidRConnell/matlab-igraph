function tf = hasEdgeAttr(graph, name)
%HASEDGEATTR determine if the graph has a specific edge attribute
%   TF = HASEDGEATTR(GRAPH) return true if graph has any edge attributes.
%   TF = HASEDGEATTR(GRAPH, NAME) return true if graph has an edge attribute
%   NAME.

    arguments
        graph
        name (1, :) char = '';
    end

    if ~igraph.args.isgraph(graph)
        throwAsCaller(MException("igraph:notGraphClass", ...
                                 "Attempted to get an edge attribute " + ...
                                 "from a graph representation that " + ...
                                 "cannot hold attributes. Only the " + ...
                                 "MATLAB classes 'graph' and 'digraph " + ...
                                 "can store attributes.\n\nThis error" + ...
                                 "may have been caused by passing " + ...
                                 "a string as a function argument " + ...
                                 "expecting an edge attribute, if " + ...
                                 "this is the case, you need to pass " + ...
                                 "a vector to the function."));
    end

    edgeAttrs = igraph.args.listEdgeAttrs(graph);
    if isempty(edgeAttrs)
        tf = false;
    elseif isempty(name)
        tf = true;
    else
        tf = any(strcmp(edgeAttrs, name));
    end
end

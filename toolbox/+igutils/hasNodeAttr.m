function tf = hasNodeAttr(graph, name)
%HASNODEATTR determine if the graph has a specific node attribute
%   TF = HASNODEATTR(GRAPH) return true if graph has any node attributes.
%   TF = HASNODEATTR(GRAPH, NAME) return true if graph has a node attribute
%   NAME.

    arguments
        graph
        name (1, :) char = '';
    end

    if ~igutils.isgraph(graph)
        throwAsCaller(MException("igraph:notGraphClass", ...
                                 "Attempted to get a node attribute " + ...
                                 "from a graph representation that " + ...
                                 "cannot hold attributes. Only the " + ...
                                 "MATLAB classes 'graph' and 'digraph " + ...
                                 "can store attributes.\n\nThis error" + ...
                                 "may have been caused by passing " + ...
                                 "a string as a function argument " + ...
                                 "expecting a node attribute, if " + ...
                                 "this is the case, you need to pass " + ...
                                 "a vector to the function."));
    end

    nodeAttrs = graph.Nodes.Properties.VariableNames;
    if isempty(nodeAttrs)
        tf = false;
    elseif isempty(name)
        tf = true;
    else
        tf = any(strcmp(nodeAttrs, name));
    end
end

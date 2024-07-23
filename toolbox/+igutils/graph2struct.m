function s = graph2struct(graph)
%GRAPH2STRUCT converts a graph type to a structure
%   This is intended for use inside mxIgraph files. It is in a public location
%   only to ensure it is available regardless of where the mex file that
%   depends on it is located.
%
%   No guarantee this will collect all pertinent information from the graph
%   type.

    nodes = table2struct_i(graph.Nodes);
    edges = table2struct_i(graph.Edges);

    s = struct('Nodes', nodes, 'Edges', edges);
end

% Builtin table2struct produce an array of structures. Would rather have a
% structure of arrays.
function s = table2struct_i(t)
    s.n = size(t, 1);
    for var = t.Properties.VariableNames
        s.(var{1}) = t.(var{1});
    end
end

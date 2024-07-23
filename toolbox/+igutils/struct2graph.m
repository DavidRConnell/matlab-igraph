function g = struct2graph(s, isdirected)
%STRUCT2GRAPH converts a structure to a graph type
%   Structure should have fields 'Nodes' and 'Edges' which should be structures
%   that provide number of nodes/edges in a field n, the nodes and edge data as
%   well as other fields for any attributes to track.
%
%   This is intended for use inside mxIgraph files. It is in a public location
%   only to ensure it is available regardless of where the MATLAB file that
%   calls a mex function is located. Should be regarded as private.

    s.Nodes = toTable(s.Nodes);
    s.Edges = toTable(s.Edges);

    if isdirected
        g = digraph(s.Edges, s.Nodes);
    else
        g = graph(s.Edges, s.Nodes);
    end
end

function t = toTable(s)
    nfields = length(fieldnames(s));
    if nfields == 1
        t = array2table(zeros(s.n, 0));
    else
        s = rmfield(s, 'n');
        t = struct2table(s);
    end
end

function values = validateEdgeAttr(graph, attr)
%VALIDATEEDGEATTR check for edge attribute and return it's vector
%   VALUES = VALIDATEEDGEATTR(GRAPH, ATTR) validates ATTR is an edge attribute
%   for GRAPH and if so returns its value.
%
%   ATTR can be either the name of an edge attribute in GRAPH or the values of
%   an attribute. If ATTR is a name validation will fail if ATTR is not the one
%   of GRAPH's edge attributes determined by IGUTILS.HASEDGEATTR. If ATTR is
%   the values of the attribute, the number of rows in ATTR must match the
%   number of edges in GRAPH.
%
%   When ATTR is a name and validation succeeds VALUES will be the attribute
%   pulled from GRAPH. When ATTR is the values they will be returned unaltered
%   if validation is successful.
%
%   See also IGUTILS.VALIDATENODEATTR.

    eid = '';
    if size(attr, 2) > 1
        if size(attr, 1) ~= igraph.numedges(graph)
            eid = 'igraph:wrongLength';
            msg = "The number of edge attributes does not match the " + ...
                  "number of edges in graph.";
        end

        values = attr;
    elseif isstring(attr) || ischar(attr)
        % hasEdgeAttr will error in the case graph is not a representation that
        % holds attributes so don't need explicit check here.
        if ~igutils.hasEdgeAttr(graph, attr)
            eid = 'igraph:missingAttribute';
            msg = sprintf("The provided graph does not contain an edge " + ...
                  "attribute named '%s'.", attr);
        end

        values = graph.Edges.(attr)';
    else
        values = repmat(attr, [1, igraph.numedges(graph)]);
    end

    if ~isempty(eid)
        throwAsCaller(MException(eid, msg));
    end
end

function values = validateNodeAttr(graph, attr)
%VALIDATENODEATTR check for node attribute and return it's vector
%   VALUES = VALIDATENODEATTR(GRAPH, ATTR) validates ATTR is a node attribute
%   for GRAPH and if so returns its value.
%
%   ATTR can be either the name of a node attribute in GRAPH or the values of
%   an attribute. If ATTR is a name validation will fail if ATTR is not the one
%   of GRAPH's node attributes determined by IGUTILS.HASNODEATTR. If ATTR is
%   the values of the attribute, the number of rows in ATTR must match the
%   number of nodes in GRAPH.
%
%   When ATTR is a name and validation succeeds VALUES will be the attribute
%   pulled from GRAPH. When ATTR is the values they will be returned unaltered
%   if validation is successful.
%
%   See also IGUTILS.VALIDATEEDGEATTR.

    eid = '';
    if size(attr, 2) > 1
        if size(attr, 2) ~= igraph.numnodes(graph)
            eid = 'igraph:wrongLength';
            msg = "The number of node attributes does not match the " + ...
                  "number of nodes in graph.";
        end

        values = attr;
    elseif isstring(attr) || ischar(attr)
        % hasNodeAttr will error in the case graph is not a representation that
        % holds attributes so don't need explicit check here.
        if ~igutils.hasNodeAttr(graph, attr)
            eid = 'igraph:missingAttribute';
            msg = sprintf("The provided graph does not contain a node " + ...
                          "attribute named '%s'.", attr);
        end

        values = graph.Nodes.(attr)';
    else
        values = repmat(attr, [1, igraph.numnodes(graph)]);
    end

    if ~isempty(eid)
        throwAsCaller(MException(eid, msg));
    end
end

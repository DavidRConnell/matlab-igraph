function mustHoldEdgeAttr(graph, name)
%MUSTHOLDEDGEATTR argument validater for new edge attributes
%   MUSTHOLDEDGEATTR(GRAPH, NAME) throws an error if GRAPH cannot hold
%   attributes or if GRAPH already has an edge attribute NAME.
%
%   MUSTHOLDEDGATTR will always return successfully if name is an empty
%   string.
%
%   See also IGUTILS.MUSTHOLDNODEATTR.

    if isempty(name)
        return
    end

    eid = '';
    if strcmp(name, "EndNodes");
        eid = 'igraph:reservedAttribute';
        msg = "Cannot use the reserved edge attribute name 'EndNodes'."
    elseif igutils.hasEdgeAttr(graph, name)
        eid = 'igraph:duplicateName';
        msg = sprintf("The provided graph already has an edge " + ...
                      "attribute with the name '%s', remove the old " + ...
                      "attribute or choose a different name.", name);
    end

    if ~isempty(eid);
        throwAsCaller(MException(eid, msg));
    end
end

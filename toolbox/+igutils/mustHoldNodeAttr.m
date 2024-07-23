function mustHoldNodeAttr(graph, name)
%MUSTHOLDNODEATTR argument validater for new node attributes
%   MUSTHOLDNODEATTR(GRAPH, NAME) throws an error if GRAPH cannot hold
%   attributes or if GRAPH already has a node attribute NAME.
%
%   MUSTHOLDNODEATTR will always return successfully if name is an empty
%   string.
%
%   See also IGUTILS.MUSTHOLDEDGEATTR.

    if isempty(name)
        return
    end

    eid = '';
    if igutils.hasNodeAttr(graph, name)
        eid = 'igraph:duplicateName';
        msg = sprintf("The provided graph already has a node " + ...
                      "attribute with the name '%s', remove the old " + ...
                      "attribute or choose a different name.", name);
    end

    if ~isempty(eid);
        throwAsCaller(MException(eid, msg));
    end
end

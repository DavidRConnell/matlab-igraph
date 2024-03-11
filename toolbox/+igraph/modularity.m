function q = modularity(adj, membership, options)
%MODULARITY calculate the modularity of a graph under a partition
%   Q = MODULARITY(ADJ, MEMBERSHIP) calculates the Newman modularity for the
%       given ADJ with structure provided by MEMBERSHIP.
%
%   Q = MODULARITY(..., 'PARAM1', VAL1, 'PARAM2', VAL2) Additionally accepts
%       the optional name value pairs RESOLUTION and ISDIRECTED. RESOLUTION
%       modifies the modularity equation to prefer larger or smaller
%       communities. By default RESOLUTION is 1, which gives the original
%       behavior.
%
%       ISDIRECTED determines whether the graph should be treated as directed
%       or not. Defaults to the results of IGRAPH.ISDIRECTED.
%
%   See also igraph.optimalModularity, igraph.cluster.

    arguments
        adj {mustBeAdj};
        membership {mustBePartition};
        options.resolution (1, 1) {mustBeNonnegative} = 1;
        options.isdirected (1, 1) logical = igraph.isdirected(adj);
    end

    q = mexIgraphDispatcher(mfilename(), adj, membership, options);
end

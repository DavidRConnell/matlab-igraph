function q = modularity(graph, membership, graphOpts, methodOpts)
%MODULARITY calculate the modularity of a graph under a partition
%   Q = MODULARITY(GRAPH, MEMBERSHIP) calculates the Newman modularity for the
%   given GRAPH with structure provided by MEMBERSHIP.
%
%   Q = MODULARITY(..., 'PARAM1', VAL1, 'PARAM2', VAL2) Additionally accepts
%   the optional name value pairs 'resolution' and 'isdirected'. 'resolution'
%   modifies the modularity equation to prefer larger or smaller communities.
%   By default RESOLUTION is 1, which gives the original behavior.
%
%   'isdirected' determines whether the graph should be treated as directed or
%   not. Defaults to the results of IGRAPH.ISDIRECTED.
%
%   See also IGRAPH.OPTIMALMODULARITY, IGRAPH.CLUSTER.

    arguments
        graph {igutils.mustBeGraph};
        membership {igutils.mustBePartition};
        graphOpts.isdirected (1, 1) logical = igraph.isdirected(graph);
        methodOpts.resolution (1, 1) {mustBeNonnegative} = 1;
    end

    q = mexIgraphDispatcher(mfilename(), graph, membership, graphOpts, ...
                            methodOpts);
end

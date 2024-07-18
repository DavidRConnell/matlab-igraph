function q = optimalModularity(graph)
%OPTIMALMODULARITY return the maximum modularity for the graph
%   Q = OPTIMALMODULARITY(GRAPH) finds the community structure that maximizes
%       the modularity of the graph then returns the resulting modularity.
%
%   This is a slow algorithm with exponential time-complexity. According to the
%   igraph documentation, you probably don't want to run on a graph with more
%   than a few hundred nodes (if that).
%
%   This method is useful for comparing the results of other community
%   detection algorithms to the "optimal" (specifically in modularity)
%   membership structure.
%
%   To get the community structure that maximizes modularity use
%   IGRAPH.CLUSTER.
%
%   See also IGRAPH.MODULARITY, IGRAPH.CLUSTER.

    arguments
        graph {igraph.args.mustBeGraph};
    end

    q = mexIgraphDispatcher(mfilename(), graph);
end

function [adj, memb] = benchmark(adjOptions, methodOptions)
%BENCHMARK generate a benchmark graph for testing clustering algorithms.
%   ADJ = BENCHMARK() create a benchmark graph using the LFR method.
%
%   [ADJ, MEMBERSHIP] = BENCHMARK() return the original community structure
%       used to generate the benchmark. As the mixing (see below) constant
%       increases, the MEMBERSHIP will reflect the current community structure
%       less.
%
%   ... = BENCHMARK('PARAM1', VAL1, 'PARAM2', VAL2, ...) BENCHMARK accepts
%       optional parameters related to the return type of the graph as well as
%       method specific options.
%
%   Returned adjacency matrix options:
%
%        Name           Description
%       --------------------------------------------------------------------
%        'makeSparse'   Whether to return a sparse (default) or full matrix.
%        'dtype'        The data type to use, either 'double' (default) or
%                       'logical'.
%
%   Method options:
%       Currently the only method implemented is the LFR benchmark (defined
%       in: Andrea Lancichinetti, Santo Fortunato & Filippo Radicchi (2008)
%       Benchmark Graphs for Testing Community Detection Algorithms, Physical
%       Review E.). This samples the node degrees and community sizes from a
%       power law distribution and places nodes in communities at random. The
%       maximum degree is set to the number of nodes - 1 and the minimum degree
%       is set such that the expected degree is as close as possible to the
%       requested mean degree. Nodes are then wired at random to satisfy the
%       mixing parameter, which defines the proportion of inter-community
%       connections. (When the mixing parameter is 0, there is no mixing
%       between communities.)
%
%        Name                Description
%       --------------------------------------------------------------------
%        'method'            Currently only 'LFR' benchmarks are implemented.
%        'nNodes'            Number of nodes (defaults to 100).
%        'mixing'            The mixing parameter. This is the proportion
%                            (between 0 and 1) of edges that connect a node to
%                            nodes outside of its community. A value of 0 gives
%                            perfect community structure. (Default 0.1.)
%        'meanDegree'        The average degree of nodes. Node degrees are
%                            sampled from a power law distribution. (Default
%                            10.)
%        'degreeExponent'    The exponent (alpha) of the power law
%                            distribution used to sample node degrees. Must be
%                            > 1 since the mean of a power law distribution
%                            with alpha == 1 is not well defined. As the value
%                            gets larger, must values will be near the minimum
%                            degree. (Default 2.)
%        'communityExponent' Alpha for the community size power law
%                            distribution. (Default 3.)
%        'dryRun'            If set to true, instead of creating the
%                            benchmarks, print the node degree vector and
%                            community size vector (default false).
%
%   See also igraph.randgame, igraph.rewire, igraph.rng, igraph.generate.

    arguments
        adjOptions.makeSparse (1, 1) logical = true;
        adjOptions.dtype (1, :) char ...
            {mustBeMemberi(adjOptions.dtype, {'logical', 'double'})} = 'double';
        methodOptions.method (1, :) char ...
            {mustBeMemberi(methodOptions.method, {'lfr'})} = 'lfr';
        methodOptions.nNodes (1, 1) {mustBeNonnegative} = 100;
        methodOptions.mixing (1, 1) ...
            {mustBeInRange(methodOptions.mixing, 0, 1)} = 0.1;
        methodOptions.meanDegree (1, 1) {mustBePositive} = 10;
        methodOptions.degreeExponent (1, 1) ...
            {mustBeGreaterThan(methodOptions.degreeExponent, 1)} = 3;
        methodOptions.communityExponent (1, 1) ...
            {mustBeGreaterThan(methodOptions.communityExponent, 1)} = 2;
        methodOptions.dryRun (1, 1) logical = false;
    end

    if methodOptions.meanDegree >= (methodOptions.nNodes - 1)
        error("igraph:badValue", "Mean degree cannot be greater than " + ...
              "the number of nodes in the graph.")
    end

    if nargout == 2
        [adj, memb] = mexIgraphDispatcher(mfilename(), ...
                                          adjOptions, methodOptions);
    else
        adj = mexIgraphDispatcher(mfilename(), adjOptions, methodOptions);
    end
end

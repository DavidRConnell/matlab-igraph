function varargout = correlatedPair(x, opts)
%CORRELATEDPAIR create a pair of correlated graphs
%   ADJ2 = CORRELATEDPAIR(ADJ1, ...) create a new graph that is correlated to
%       ADJ1.
%   [ADJ1, ADJ2] = CORRELATEDPAIR(NNODES, ...) create two graphs with NNODES
%       nodes each that are correlated.
%
%   If the number of outputs is 1, assumes the first parameter is an adjacency
%   matrix to use in creating the second adjacency matrix. If there are two
%   outputs, assumes the first parameter is an integer number of nodes.
%
%   ... = CORRELATEDPAIR(..., "PARAM1", VAL1, "PARAM2", VAL2, ...)
%
%       If an adjacency matrix is the first argument, the default values for
%       options that impact the type of adjacency matrix returned will be to
%       match the input adjacency matrix, otherwise the defaults are those in
%       the table below. For ISDIRECTED the igraph.isdirected test is used
%       which is a best guess based off the characteristics of the adjacency
%       matrix. If you want a symmetric adjacency matrix to be treated as
%       directed, explicitly set the ISDIRECTED option.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'makeSparse'  Whether to return a sparse (default) or full matrix.
%        'dtype'       The data type to use, either 'double' (default) or
%                      'logical'.
%        'correlation' The (Pearson) correlation between the two graphs
%                      (default 0.5).
%        'density'     The probability any given edge exists (i.e. the goal
%                      edge density). Default is 0.25 if both adjacency
%                      matrices are being created otherwise, it's the density
%                      of the old graph.
%        'isdirected'  Whether the new graphs should be directed (default
%                      false).
%
%   See also igraph.rng, igraph.randgame, igraph.rewire.

    arguments
        x
        opts.makeSparse (1, 1) logical;
        opts.dtype (1, :) char;
        opts.correlation (1, 1) ...
            {mustBeInRange(opts.correlation, -1, 1)} = 0.25;
        opts.density (1, 1) {mustBeInRange(opts.density, 0, 1)};
        opts.isdirected (1, 1) logical;
    end

    opts = namedargs2cell(opts);
    if nargout < 2
        adj2 = correlateWith(x, opts{:});
        varargout = {adj2};
    elseif nargout == 2
        [adj1, adj2] = generatePair(x, opts{:});
        varargout = {adj1, adj2};
    else
        error("Igraph:tooManyOutputs", "correlatedPair returns two or " + ...
              "less values, %d requested.", nargout);
    end
end

function adj2 = correlateWith(adj, adjOptions, opts)
    arguments
        adj = {mustBeAdj}
        adjOptions.makeSparse = issparse(adj);
        adjOptions.dtype;
        opts.correlation;
        opts.density = nnz(adj + adj') / numel(adj);
        opts.isdirected = igraph.isdirected(adj);
    end

    if ~isoptionset(adjOptions, "dtype")
        if islogical(adj)
            adjOptions.dtype = 'logical';
        else
            adjOptions.dtype = 'double';
        end
    end

    adj2 = mexIgraphCorrelatedPairs('correlate', adjOptions.makeSparse, ...
                                    adjOptions.dtype, adj, ...
                                    opts.correlation, opts.density, ...
                                    opts.isdirected);
end

function [adj1, adj2] = generatePair(nNodes, adjOptions, opts)
    arguments
        nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        adjOptions.makeSparse = true;
        adjOptions.dtype = 'double';
        opts.correlation;
        opts.density = 0.5;
        opts.isdirected = false;
    end

    [adj1, adj2] = mexIgraphCorrelatedPairs('generate', ...
                                            adjOptions.makeSparse, ...
                                            adjOptions.dtype, nNodes, ...
                                            opts.correlation, opts.density, ...
                                            opts.isdirected);
end

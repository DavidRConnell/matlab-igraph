function varargout = correlatedPair(x, adjOptions, methodOptions)
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
        adjOptions.makeSparse (1, 1) logical;
        adjOptions.dtype (1, :) char;
        methodOptions.correlation (1, 1) ...
            {mustBeInRange(methodOptions.correlation, -1, 1)} = 0.25;
        methodOptions.density (1, 1) {mustBeInRange(methodOptions.density, 0, 1)};
        methodOptions.isdirected (1, 1) logical;
    end

    adjOptions = namedargs2cell(adjOptions);
    methodOptions = namedargs2cell(methodOptions);
    if nargout < 2
        adj2 = correlateWith(x, adjOptions{:}, methodOptions{:});
        varargout = {adj2};
    elseif nargout == 2
        [adj1, adj2] = generatePair(x, adjOptions{:}, methodOptions{:});
        varargout = {adj1, adj2};
    else
        error("Igraph:tooManyOutputs", "correlatedPair returns two or " + ...
              "less values, %d requested.", nargout);
    end
end

function adj2 = correlateWith(adj, adjOptions, methodOptions)
    arguments
        adj = {mustBeAdj}
        adjOptions.makeSparse = issparse(adj);
        adjOptions.dtype;
        methodOptions.correlation;
        methodOptions.density = nnz(adj + adj') / numel(adj);
        methodOptions.isdirected = igraph.isdirected(adj);
    end

    if ~isoptionset(adjOptions, "dtype")
        if islogical(adj)
            adjOptions.dtype = 'logical';
        else
            adjOptions.dtype = 'double';
        end
    end

    adj2 = mexIgraphDispatcher(mfunctionname(), adj, adjOptions, ...
                               methodOptions);
end

function [adj1, adj2] = generatePair(nNodes, adjOptions, methodOptions)
    arguments
        nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        adjOptions.makeSparse = true;
        adjOptions.dtype = 'double';
        methodOptions.correlation;
        methodOptions.density = 0.5;
        methodOptions.isdirected = false;
    end

    [adj1, adj2] = mexIgraphDispatcher(mfunctionname(), adjOptions, ...
                                       methodOptions);
end

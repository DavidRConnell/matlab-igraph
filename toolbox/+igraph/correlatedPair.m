function varargout = correlatedPair(x, graphOpts, methodOpts)
%CORRELATEDPAIR create a pair of correlated graphs
%   G2 = CORRELATEDPAIR(G1, ...) create a new graph that is correlated to
%       G1.
%   [G1, G2] = CORRELATEDPAIR(NNODES, ...) create two graphs with NNODES
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
%   See also IGRAPH.RNG, IGRAPH.RANDGAME, IGRAPH.REWIRE.

    arguments
        x
        graphOpts.makeSparse (1, 1) logical;
        graphOpts.dtype (1, :) char;
        methodOpts.correlation (1, 1) ...
            {mustBeInRange(methodOpts.correlation, -1, 1)} = 0.25;
        methodOpts.density (1, 1) {mustBeInRange(methodOpts.density, 0, 1)};
        methodOpts.isdirected (1, 1) logical;
    end

    graphOpts = namedargs2cell(graphOpts);
    methodOpts = namedargs2cell(methodOpts);
    if nargout < 2
        g2 = correlateWith(x, graphOpts{:}, methodOpts{:});
        varargout = {g2};
    elseif nargout == 2
        [g1, g2] = generatePair(x, graphOpts{:}, methodOpts{:});
        varargout = {g1, g2};
    else
        error("Igraph:tooManyOutputs", "correlatedPair returns two or " + ...
              "less values, %d requested.", nargout);
    end
end

function g2 = correlateWith(g1, graphOpts, methodOpts)
    arguments
        g1 = {igraph.args.mustBeGraph}
        graphOpts.makeSparse = issparse(g1);
        graphOpts.dtype;
        methodOpts.correlation;
        methodOpts.density = nnz(g1 + g1') / numel(g1);
        methodOpts.isdirected = igraph.isdirected(g1);
    end

    if ~igraph.args.isoptionset(graphOpts, "dtype")
        if islogical(g1)
            graphOpts.dtype = 'logical';
        else
            graphOpts.dtype = 'double';
        end
    end

    g2 = mexIgraphDispatcher(mfunctionname(), g1, graphOpts, ...
                               methodOpts);
end

function [g1, g2] = generatePair(nNodes, graphOpts, methodOpts)
    arguments
        nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        graphOpts.makeSparse = true;
        graphOpts.dtype = 'double';
        methodOpts.correlation;
        methodOpts.density = 0.5;
        methodOpts.isdirected = false;
    end

    [g1, g2] = mexIgraphDispatcher(mfunctionname(), graphOpts, ...
                                       methodOpts);
end

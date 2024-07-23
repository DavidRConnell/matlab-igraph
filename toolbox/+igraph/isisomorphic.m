function TF = isisomorphic(g1, g2, graphOpts)
%ISISOMORPHIC test if two graphs are isomorphic
%   TF = ISISOMORPHIC(GRAPH1, GRAPH2) returns true if GRAPH1 and GRAPH2 are the
%   equivalent when ignoring the order of nodes.
%
%   TF = ISISOMORPHIC(GRAPH1, GRAPH2, 'isdirected', TF) override the guesses
%   for whether GRAPH1 and GRAPH2 are directed. By default IGRAPH.ISDIRECTED
%   tests both graphs. If one is directed the value is set to true otherwise
%   false.
%
%   See also IGRAPH.ISSUBISOMORPHIC.

    arguments
        g1 {igutils.mustBeGraph}
        g2 {igutils.mustBeGraph}
        graphOpts.isdirected (1, 1) logical = ...
            igraph.isdirected(g1) || igraph.isdirected(g2);
    end

    TF = mexIgraphDispatcher(mfilename(), g1, g2, graphOpts);
end

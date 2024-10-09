function pos = layout(graph, method, graphOpts, methodOpts)
%LAYOUT position graph vertices for plotting
%   POS = LAYOUT(GRAPH, METHOD) use METHOD to generate coordinates for all
%   nodes in GRAPH. Layout will often be created indirectly by IGRAPH.PLOT, but
%   calling LAYOUT directly can be useful for reusing the same layout or
%   manually modifying a layout.
%
%   POS = LAYOUT(GRAPH, METHOD, 'isdirected', TF) explicitly state if the graph
%   should be treated as directed or not. Defaults to result of
%   IGRAPH.ISDIRECTED.
%
%   Methods include regular layouts:
%       'random', 'circle', 'star', 'grid', 'graphopt', 'bipartite',
%       'FruchtermanReingold' ('fr'), 'KamadaKawai' ('kk'), 'gem',
%       'DavidsonHarel' ('dh'), 'mds', and 'lgl'.
%
%   Additionally there are layouts for tree graphs:
%       'Reingoldtilford' ('rt').
%
%   Trying to use a tree layout for a non-tree graph (that fails
%   IGRAPH.ISTREE) will trigger an error.
%
%   Not all methods take into consider edges or edge weights. Methods that uses
%   edge weights are: 'FruchtermanReingold' and 'KamadaKawai'.
%
%   Some layouts have additional optional arguments specified below:
%
%   POS = LAYOUT(GRAPH, 'random') Places nodes at random along a 2d plane.
%   POS = LAYOUT(GRAPH, 'circle', ...) Uniform place nodes along a circle.
%
%         Name    Description
%       --------------------------------------------------------------------
%        'order' A vector of node indices (starting at 1) specifying the order
%                nodes should be plotted. Defaults to 1:nNodes(GRAPH). The
%                length can be less than the number of nodes in which case
%                nodes excluded from order will be placed at (0, 0).
%
%   POS = LAYOUT(GRAPH, 'star', ...) Form star with a node in the center.
%
%         Name    Description
%       --------------------------------------------------------------------
%        'center' The index of the node to put in the center (default 1).
%        'order'  A vector of node indices (including the center) specifying
%                 the order nodes should be plotted. Defaults to
%                 1:nNodes(GRAPH).
%
%   POS = LAYOUT(GRAPH, 'grid', ...) Place nodes in a grid.
%
%         Name    Description
%       --------------------------------------------------------------------
%        'width' The number of nodes per row. Default of 0 means
%                ceil(sqrt(nNodes)).
%
%   POS = LAYOUT(GRAPH, 'graphopt', ...) Use the graphopt algorithm.
%
%         Name             Description
%       --------------------------------------------------------------------
%        'nIterations'    Number of iterations (default 500).
%        'charge'         Node charge for calculating repulsion (default 0.001).
%        'mass'           Node mass for spring forces (default 30).
%        'springLength'   Length of springs (default 0).
%        'springConstant' Spring constant (default 1).
%        'stepMax'        Maximum movement along a single axis per iteration
%                         (default 5).
%        'initial'        The initial layout to start the simulation
%                         (default [], let the algorithm decide).
%
%   POS = LAYOUT(GRAPH, 'bipartite', ...) A layout for displaying bipartite
%   graphs.
%
%         Name             Description
%       --------------------------------------------------------------------
%        'types'         Logical vector of nodes expressing which nodes are in
%                        each type.
%        'hgap'          Minimum horizontal gap (default 1).
%        'vgap'          Minimum vertical gap (default 5).
%        'maxIterations' Maximum number of iterations to perform (default 100).
%
%   POS = LAYOUT(GRAPH, 'FruchtermanReingold', ...)
%   POS = LAYOUT(GRAPH, 'fr', ...) Use the Fruchterman-Reingold algorithm.
%
%         Name          Description
%       --------------------------------------------------------------------
%        'nIterations' Number of iterations (default 500).
%        'startTemp'   Initial maximum movement per step (decreases over
%                      iterations). Default 10.
%        'grid'        Whether to use the faster grid version or not (default
%                      chooses grid for large graphs).
%        'initial'     The initial layout to start the algorithm (default []).
%
%   POS = LAYOUT(GRAPH, 'KamadaKawai', ...)
%   POS = LAYOUT(GRAPH, 'kk', ...) Use the Kamada-Kawai algorithm
%
%         Name            Description
%       --------------------------------------------------------------------
%        'maxIterations' Maximum number of iterations (default 10 * nNodes).
%        'epsilon'       The minimum change needed to continue iterating
%                        (default 0, always run MAXITERATIONS iterations).
%        'constant'      Node attraction constant (default nNodes).
%        'initial'       The initial layout to start the algorithm (default []).
%
%   POS = LAYOUT(GRAPH, 'gem', ...) Use the GEM algorithm.
%
%         Name            Description
%       --------------------------------------------------------------------
%        'maxIterations' Maximum number of iterations (default 40 * nNodes ^ 2).
%        'tempMax'       Maximum local temperature (default nNodes).
%        'tempMin'       The global temperature that will cause the simulation
%                        to stop (default 0.1).
%        'tempInit'      Initial local temperatures (default sqrt(nNodes)).
%        'initial'       The initial layout to start the algorithm (default []).
%
%   POS = LAYOUT(GRAPH, 'DavidsonHarel', ...)
%   POS = LAYOUT(GRAPH, 'dh', ...) The Davidson-Harel algorithm.
%
%         Name                 Description
%       --------------------------------------------------------------------
%        'maxIterations'      Maximum number of iterations (default 10).
%        'fineIterations'     Number of tuning iterations (default
%                             max(10, log2(nNodes))).
%        'coolingFactor'      Colling factor (default 0.75).
%        'weightNodeDist'     Weight for node--node distances (default 1).
%        'weightBorder'       Weight for border distance (default 0).
%        'weightEdgeLen'      Weight for edge lengths (default density / 10).
%        'weightEdgeCross'    Weight for edge crossing (default 1 -
%                             sqrt(density)).
%        'weightNodeEdgeDist' Weight for node--edge distances (default 1 -
%                             (density / 5)).
%        'initial'            The initial layout to start the algorithm
%                             (default []).
%
%   POS = LAYOUT(GRAPH, 'mds', ...) Use multidimensional scaling to position
%   nodes.
%
%         Name        Description
%       --------------------------------------------------------------------
%        'distance'  The nNodes x nNodes distance matrix. Distance functions
%                    should be undirected (default [], in which case the
%                    shortest path lengths will be used).
%
%   POS = LAYOUT(GRAPH, 'lgl', ...) The large graph layout (LGL) force based
%   simulation.
%
%         Name            Description
%       --------------------------------------------------------------------
%        'maxIterations' Max number of iterations (default 150).
%        'stepMax'       Maximum distance allowed for a node to move per step
%                        (default nNodes).
%        'area'          Size of the area nodes are placed (default nNodes ^ 2).
%        'coolExp'       Cooling exponent (default 1.5).
%        'repulseRadius' The radius of repulsion (default nNodes ^ 3).
%        'cellSize'      Size of the grid cells (default sqrt(nNodes)).
%        'root'          The root node (default randi([1, nNodes(graph)])).
%
%   POS = LAYOUT(GRAPH, 'ReingoldTilford', ...)
%   POS = LAYOUT(GRAPH, 'rt', ...) Use the Reingold-Tilford tree layout
%   algorithm.
%
%         Name         Description
%       --------------------------------------------------------------------
%        'mode'       Determines which edges to use for building the tree,
%                     'all' (default), 'out', or 'in'.
%        'root'       The index of the root node or a vector of root nodes. By
%                     default, it attempts to select a root node automatically.
%        'circular'   Whether to plot vertices in a circle.
%
% See also IGRAPH.PLOT, IGRAPH.RNG.

    arguments
        graph {igutils.mustBeGraph};
        method (1, :) char ...
            {igutils.mustBeMemberi(method, ...
                                   {'random', 'circle', 'star', 'grid', ...
                                    'graphopt', 'bipartite', ...
                                    'fruchtermanreingold', 'fr', ...
                                    'kamadakawai', 'kk', 'gem', ...
                                    'davidsonharel', 'dh', 'mds', ...
                                    'lgl', 'reingoldtilford', 'rt', ...
                                    'reingoldtilfordcircular'})};

        graphOpts.?igutils.GraphInProps;
        methodOpts.order;
        methodOpts.center;
        methodOpts.width;
        methodOpts.nIterations;
        methodOpts.charge;
        methodOpts.mass;
        methodOpts.springLenth;
        methodOpts.springConstant;
        methodOpts.stepMax;
        methodOpts.initial;
        methodOpts.types;
        methodOpts.hgap;
        methodOpts.vgap;
        methodOpts.maxIterations;
        methodOpts.startTemp;
        methodOpts.grid;
        methodOpts.epsilon;
        methodOpts.constant;
        methodOpts.tempMax;
        methodOpts.tempMin;
        methodOpts.tempInit;
        methodOpts.fineIterations;
        methodOpts.coolingFactor;
        methodOpts.weightNodeDist;
        methodOpts.weightBorder;
        methodOpts.weightEdgeLen;
        methodOpts.weightEdgeCross;
        methodOpts.weightNodeEdgeDist;
        methodOpts.area;
        methodOpts.coolExp;
        methodOpts.repulseRadius;
        methodOpts.cellSize;
        methodOpts.root;
        methodOpts.mode;
        methodOpts.circular
    end

    graphOpts = namedargs2cell(graphOpts);
    graphOpts = igutils.setGraphInProps(graph, graphOpts{:});

    method = lower(method);
    if strcmp(method, 'fr')
        method = 'fruchtermanreingold';
    elseif strcmp(method, 'kk')
        method = 'kamadakawai';
    elseif strcmp(method, 'dh')
        method = 'davidsonharel';
    elseif strcmp(method, 'rt')
        method = 'reingoldtilford';
    end

    treeMethods = {'reingoldtilford'};

    if any(strcmp(treeMethods, method)) && ~igraph.istree(graph)
        error("igraph:notATree", "Requested a tree layout for a graph " + ...
              "that is not a tree.");
    end

    methodOpts = namedargs2cell(methodOpts);
    switch method
      case 'random'
        methodOpts = parseNullOptions(graph, methodOpts{:});
      case 'circle'
        methodOpts = parseCircleOptions(graph, methodOpts{:});
      case 'star'
        methodOpts = parseStarOptions(graph, methodOpts{:});
      case 'grid'
        methodOpts = parseGridOptions(graph, methodOpts{:});
      case 'graphopt'
        methodOpts = parseGraphoptOptions(graph, methodOpts{:});
      case 'bipartite'
        methodOpts = parseBipartiteOptions(graph, methodOpts{:});
      case 'fruchtermanreingold'
        methodOpts = parseFruchtermanReingoldOptions(graph, methodOpts{:});
      case 'kamadakawai'
        methodOpts = parseKamadaKawaiOptions(graph, methodOpts{:});
      case 'gem'
        methodOpts = parseGemOptions(graph, methodOpts{:});
      case 'davidsonharel'
        methodOpts = parseDavidsonharelOptions(graph, methodOpts{:});
      case 'mds'
        methodOpts = parseMdsOptions(graph, methodOpts{:});
      case 'lgl'
        methodOpts = parseLglOptions(graph, methodOpts{:});
      case 'reingoldtilford'
        methodOpts = parseReingoldTilfordOptions(graph, methodOpts{:});
        if methodOpts.circular
            method = 'reingoldtilfordcircular';
        end
        methodOpts = rmfield(methodOpts, 'circular');
      otherwise
        error("The method ""%s"" has not been implemented.\n\n" + ...
              "Please report an issue or submit a pull request to github.", ...
              method);
    end

    pos = mexIgraphDispatcher(mfilename(), graph, method, graphOpts, ...
                              methodOpts);
end

function opts = parseNullOptions(~)
    arguments
        ~
    end

    opts = struct();
end

function opts = parseCircleOptions(graph, opts)
    arguments
        graph; %#ok<INUSA>
        opts.order (1, :) {mustBePositive, mustBeInteger} = ...
            1:igraph.numnodes(graph);
    end
end

function opts = parseStarOptions(graph, opts)
    arguments
        graph; %#ok<INUSA>
        opts.center (1, 1) {mustBePositive, mustBeInteger} = 1;
        opts.order (1, :) {mustBePositive, mustBeInteger} = ...
            1:igraph.numnodes(graph);
    end
end

function opts = parseGridOptions(graph, opts)
    arguments
        graph;
        opts.width (1, 1) {mustBeNonnegative, mustBeInteger} = 0;
    end

    if opts.width > igraph.numnodes(graph)
        throwAsCaller(MException("igraph:outOfBounds", ...
                                 "Width must be smaller than or equal " + ...
                                 "to the number of nodes, %d", ...
                                 igraph.numnodes(graph)));
    end
end

function opts = parseGraphoptOptions(graph, opts)
    arguments
        graph;
        opts.nIterations (1, 1) {mustBePositive, mustBeInteger} = 500;
        opts.charge (1, 1) double = 0.001;
        opts.mass (1, 1) double {mustBePositive} = 30;
        opts.springLength (1, 1) double {mustBeNonnegative} = 0;
        opts.springConstant (1, 1) double = 1;
        opts.stepMax (1, 1) double {mustBePositive} = 5;
        opts.initial (:, 2) double = [];
    end

    if opts.initial
        if size(opts.initial, 1) ~= igraph.numnodes(graph)
            throwAsCaller(MException("igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end
end

function opts = parseBipartiteOptions(graph, opts)
    arguments
        graph;
        opts.types (1, :) logical;
        opts.hgap (1, 1) {mustBeNonnegative} = 1;
        opts.vgap (1, 1) {mustBeNonnegative} = 5;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = 100;
    end

    if length(opts.types) ~= igraph.numnodes(graph)
        throwAsCaller(MException("igraph:wrongSize", ...
                                 "Type vector must have exactly one " + ...
                                 "element for every node."));
    end
end

function opts = parseFruchtermanReingoldOptions(graph, opts)
    arguments
        graph;
        opts.nIterations (1, 1) {mustBePositive, mustBeInteger} = 500;
        opts.startTemp (1, 1) {mustBeNonnegative} = 10;
        opts.initial (:, 2) double = [];
        opts.grid (1, 1) logical;
    end

    if opts.initial
        if size(opts.initial, 1) ~= igraph.numnodes(graph)
            throwAsCaller(MException("igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end

    if ~igutils.isoptionset(opts, 'grid')
        opts.grid = 'auto';
    elseif opts.grid
        opts.grid = 'grid';
    else
        opts.grid = 'nogrid';
    end
end

function opts = parseKamadaKawaiOptions(graph, opts)
    arguments
        graph;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = ...
            10 * igraph.numnodes(graph);
        opts.epsilon (1, 1) {mustBeNonnegative} = 0;
        opts.constant (1, 1) {mustBeNonnegative} = igraph.numnodes(graph);
        opts.initial (:, 2) double = [];
    end

    if opts.initial
        if size(opts.initial, 1) ~= igraph.numnodes(graph)
            throwAsCaller(MException("igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end
end

function opts = parseGemOptions(graph, opts)
    arguments
        graph;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = ...
            (40 * (igraph.numnodes(graph) ^ 2));
        opts.tempMax (1, 1) double = igraph.numnodes(graph);
        opts.tempMin (1, 1) double = 0.1;
        opts.tempInit (1, 1) double = sqrt(igraph.numnodes(graph));
        opts.initial (:, 2) double = [];
    end

    if opts.initial
        if size(opts.initial, 1) ~= igraph.numnodes(graph)
            throwAsCaller(MException("igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end
end

function opts = parseDavidsonharelOptions(graph, opts)
    arguments
        graph;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = 10;
        opts.fineIterations (1, 1) {mustBePositive, mustBeInteger} = ...
            max(10, log2(igraph.numnodes(graph)));
        opts.coolingFactor (1, 1) double = 0.75;
        opts.initial (:, 2) double = [];
        opts.weightNodeDist (1, 1) double = 1;
        opts.weightBorder (1, 1) double = 0;
        opts.weightEdgeLen (1, 1) double;
        opts.weightEdgeCross (1, 1) double;
        opts.weightNodeEdgeDist (1, 1) double;
    end

    if opts.initial
        if size(opts.initial, 1) ~= igraph.numnodes(graph)
            throwAsCaller(MException("igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end

    if ~igutils.isoptionset(opts, 'weightEdgeLen') || ...
            ~igutils.isoptionset(opts, 'weightEdgeCross') || ...
            ~igutils.isoptionset(opts, 'weightNodeEdgeDist')
        density = igraph.edgeDensity(graph);
    end

    if ~igutils.isoptionset(opts, 'weightEdgeLen')
        opts.weightEdgeLen = density / 10;
    end

    if ~igutils.isoptionset(opts, 'weightEdgeCross')
        opts.weightEdgeCross = 1 - sqrt(density);
    end

    if ~igutils.isoptionset(opts, 'weightNodeEdgeDist')
        opts.weightNodeEdgeDist = 1 - (density / 5);
    end
end

function opts = parseMdsOptions(~, opts)
    arguments
        ~
        opts.distance (:, :) double {igutils.mustBeSquare} = [];
    end

    if ~isempty(opts.distance) && ...
            (length(opts.distance) ~= igraph.numnodes(graph))
        throwAsCaller(MException("igraph:wrongSize", ...
                                 "Distance matrix must have element " + ...
                                 "for every pair of nodes (i.e. a " + ...
                                 "nNodes X nNodes matrix)."));
    end
end

function opts = parseLglOptions(graph, opts)
    arguments
        graph; %#ok<INUSA>
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = 150;
        opts.stepMax (1, 1) {mustBeNonnegative} = igraph.numnodes(graph);
        opts.area (1, 1) {mustBeNonnegative} = igraph.numnodes(graph) ^ 2;
        opts.coolExp (1, 1) double = 1.5;
        opts.repulseRadius (1, 1) {mustBePositive} = igraph.numnodes(graph) ^ 3;
        opts.cellSize (1, 1) {mustBePositive} = sqrt(igraph.numnodes(graph));
        opts.root (1, 1) {mustBePositive} = randi([1, igraph.numnodes(graph)]);
    end
end

function opts = parseReingoldTilfordOptions(~, opts)
    arguments
        ~
        opts.mode (1, :) char {igutils.mustBeMode} = 'all';
        opts.root (1, :) {mustBePositive, mustBeInteger} = [];
        opts.circular (1, 1) logical = false;
    end
end

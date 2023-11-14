function pos = layout(adj, method, opts, methodOpts)
%LAYOUT position graph vertices for plotting
%   POS = LAYOUT(ADJ, METHOD) use METHOD to generate coordinates for all nodes
%       in ADJ. Layout will often be created indirectly by igraph.plot, but
%       calling LAYOUT directly can be useful for reusing the same layout or
%       manually modifying a layout.
%   POS = LAYOUT(ADJ, METHOD, "ISDIRECTED", TF) explicitly state if the graph
%       should be treated as directed or not. Defaults to result of
%       IGRAPH.ISDIRECTED.
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
%   IGRAPH.ISTREE), will trigger an error.
%
%   Not all methods take into consider edges or edge weights. Methods that uses
%   edge weights are: 'FruchtermanReingold' and 'KamadaKawai'.
%
%   Some layouts have additional optional arguments specified below:
%
%   POS = LAYOUT(ADJ, 'random') Places nodes at random along a 2d plane.
%   POS = LAYOUT(ADJ, 'circle', ...) Uniform place nodes along a circle.
%
%        Name    Description
%       --------------------------------------------------------------------
%        'order' A vector of node indices (starting at 1) specifying the order
%                nodes should be plotted. Defaults to 1:length(ADJ). The length
%                can be less than the number of nodes in which case nodes
%                excluded from order will be placed at (0, 0).
%
%   POS = LAYOUT(ADJ, 'star', ...) Form star with a node in the center.
%
%        Name    Description
%       --------------------------------------------------------------------
%        'center' The index of the node to put in the center (default 1).
%        'order'  A vector of node indices (including the center) specifying
%                 the order nodes should be plotted. Defaults to 1:length(ADJ).
%
%   POS = LAYOUT(ADJ, 'grid', ...) Place nodes in a grid.
%
%        Name    Description
%       --------------------------------------------------------------------
%        'width' The number of nodes per row. Default of 0 means
%                ceil(sqrt(nNodes)).
%
%   POS = LAYOUT(ADJ, 'graphopt', ...) Use the graphopt algorithm.
%
%        Name             Description
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
%   POS = LAYOUT(ADJ, 'bipartite', ...) A layout for displaying bipartite
%       graphs.
%
%        Name             Description
%       --------------------------------------------------------------------
%        'types'         Logical vector of nodes expressing which nodes are in
%                        each type.
%        'hgap'          Minimum horizontal gap (default 1).
%        'vgap'          Minimum vertical gap (default 5).
%        'maxIterations' Maximum number of iterations to perform (default 100).
%
%   POS = LAYOUT(ADJ, 'FruchtermanReingold', ...)
%   POS = LAYOUT(ADJ, 'fr', ...) Use the Fruchterman-Reingold algorithm.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nIterations' Number of iterations (default 500).
%        'startTemp'   Initial maximum movement per step (decreases over
%                      iterations). Default 10.
%        'grid'        Whether to use the faster grid version or not (default
%                      chooses grid for large graphs).
%        'initial'     The initial layout to start the algorithm (default []).
%
%   POS = LAYOUT(ADJ, 'KamadaKawai', ...)
%   POS = LAYOUT(ADJ, 'kk', ...) Use the Kamada-Kawai algorithm
%
%        Name            Description
%       --------------------------------------------------------------------
%        'maxIterations' Maximum number of iterations (default 10 * nNodes).
%        'epsilon'       The minimum change needed to continue iterating
%                        (default 0, always run MAXITERATIONS iterations).
%        'constant'      Node attraction constant (default nNodes).
%        'initial'       The initial layout to start the algorithm (default []).
%
%   POS = LAYOUT(ADJ, 'gem', ...) Use the GEM algorithm.
%
%        Name            Description
%       --------------------------------------------------------------------
%        'maxIterations' Maximum number of iterations (default 40 * nNodes ^ 2).
%        'tempMax'       Maximum local temperature (default nNodes).
%        'tempMin'       The global temperature that will cause the simulation
%                        to stop (default 0.1).
%        'tempInit'      Initial local temperatures (default sqrt(nNodes)).
%        'initial'       The initial layout to start the algorithm (default []).
%
%   POS = LAYOUT(ADJ, 'DavidsonHarel', ...)
%   POS = LAYOUT(ADJ, 'dh', ...) The Davidson-Harel algorithm.
%
%        Name                 Description
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
%   POS = LAYOUT(ADJ, 'mds', ...) Use multidimensional scaling to position
%       nodes.
%
%        Name        Description
%       --------------------------------------------------------------------
%        'distance'  The nNodes x nNodes distance matrix. Distance functions
%                    should be undirected (default [], in which case the
%                    shortest path lengths will be used).
%
%   POS = LAYOUT(ADJ, 'lgl', ...) The large graph layout (LGL) force based
%       simulation.
%
%        Name            Description
%       --------------------------------------------------------------------
%        'maxIterations' Max number of iterations (default 150).
%        'stepMax'       Maximum distance allowed for a node to move per step
%                        (default nNodes).
%        'area'          Size of the area nodes are placed (default nNodes ^ 2).
%        'coolExp'       Cooling exponent (default 1.5).
%        'repulseRadius' The radius of repulsion (default nNodes ^ 3).
%        'cellSize'      Size of the grid cells (default sqrt(nNodes)).
%        'root'          The root node (default randi([1, length(adj)])).
%
%   POS = LAYOUT(ADJ, 'ReingoldTilford', ...)
%   POS = LAYOUT(ADJ, 'rt', ...) Use the Reingold-Tilford tree layout
%       algorithm.
%
%        Name         Description
%       --------------------------------------------------------------------
%        'mode'       Determines which edges to use for building the tree,
%                     'all' (default), 'out', or 'in'.
%        'root'       The index of the root node or a vector of root nodes. By
%                     default, it attempts to select a root node automatically.
%        'circular' Whether to plot vertices in a circle.
%
% See also igraph.layout3d, igraph.plot, igraph.rng

    arguments
        adj {mustBeAdj};
        method (1, :) char ...
            {mustBeMemberi(method, {'random', 'circle', 'star', 'grid', ...
                                    'graphopt', 'bipartite', ...
                                    'fruchtermanreingold', 'fr', ...
                                    'kamadakawai', 'kk', 'gem', ...
                                    'davidsonharel', 'dh', 'mds', ...
                                    'lgl', 'reingoldtilford', 'rt', ...
                                     'reingoldtilfordcircular'})};

        opts.directed (1, 1) logical = igraph.isdirected(adj);
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

    if any(contains(treeMethods, method)) && ~igraph.istree(adj)
        error("igraph:notATree", "Requested a tree layout for a graph " + ...
              "that is not a tree.");
    end

    methodOpts = namedargs2cell(methodOpts);
    layoutWrapper = @(varargin) ...
        mexIgraphLayout(method, adj, opts.isdirected, varargin{:});

    switch method
      case 'random'
        methodOpts = parseNullOptions(adj, methodOpts{:});
      case 'circle'
        methodOpts = parseCircleOptions(adj, methodOpts{:});
      case 'star'
        methodOpts = parseStarOptions(adj, methodOpts{:});
      case 'grid'
        methodOpts = parseGridOptions(adj, methodOpts{:});
      case 'graphopt'
        methodOpts = parseGraphoptOptions(adj, methodOpts{:});
      case 'bipartite'
        methodOpts = parseBipartiteOptions(adj, methodOpts{:});
      case 'fruchtermanreingold'
        methodOpts = parseFruchtermanReingoldOptions(adj, methodOpts{:});
      case 'kamadakawai'
        methodOpts = parseKamadaKawaiOptions(adj, methodOpts{:});
      case 'gem'
        methodOpts = parseGemOptions(adj, methodOpts{:});
      case 'davidsonharel'
        methodOpts = parseDavidsonharelOptions(adj, methodOpts{:});
      case 'mds'
        methodOpts = parseMdsOptions(adj, methodOpts{:});
      case 'lgl'
        methodOpts = parseLglOptions(adj, methodOpts{:});
      case 'reingoldtilford'
        methodOpts = parseReingoldTilfordOptions(adj, methodOpts{:});
        if methodOpts.circular
          method = 'reingoldtilfordcircular';
        end
        methodOpts = rmfield(methodOpts, 'circular');
      otherwise
        error("The method ""%s"" has not been implemented.\n\n" + ...
              "Please report an issue or submit a pull request to github.", ...
              method);
    end

    methodOpts = namedargs2cell(methodOpts);
    pos = mexIgraphLayout(method, adj, methodOpts{2:2:end});
end

function opts = parseNullOptions(~)
    arguments
        ~
    end

    opts = struct();
end

function opts = parseCircleOptions(adj, opts)
    arguments
        adj;
        opts.order (1, :) {mustBePositive, mustBeInteger} = 1:length(adj);
    end
end

function opts = parseStarOptions(adj, opts)
    arguments
        adj;
        opts.center (1, 1) {mustBePositive, mustBeInteger} = 1;
        opts.order (1, :) {mustBePositive, mustBeInteger} = 1:length(adj);
    end
end

function opts = parseGridOptions(adj, opts)
    arguments
        adj;
        opts.width (1, 1) {mustBeNonnegative, mustBeInteger} = 0;
    end

    if opts.width > length(adj)
        throwAsCallor(MException("igraph:outOfBounds", ...
                                 "Width must be smaller than or equal " + ...
                                 "to the number of nodes, %d", length(adj)));
    end
end

function opts = parseGraphoptOptions(adj, opts)
    arguments
        adj;
        opts.nIterations (1, 1) {mustBePositive, mustBeInteger} = 500;
        opts.charge (1, 1) double = 0.001;
        opts.mass (1, 1) double {mustBePositive} = 30;
        opts.springLength (1, 1) double {mustBeNonnegative} = 0;
        opts.springConstant (1, 1) double = 1;
        opts.stepMax (1, 1) double {mustBePositive} = 5;
        opts.initial (:, 2) double = [];
    end

    if opts.initial
        if size(opts.initial, 1) ~= length(adj)
            throwAsCaller(MException("Igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end
end

function opts = parseBipartiteOptions(adj, opts)
    arguments
        adj;
        opts.types (1, :) logical;
        opts.hgap (1, 1) {mustBeNonnegative} = 1;
        opts.vgap (1, 1) {mustBeNonnegative} = 5;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = 100;
    end

    if length(opts.types) ~= length(adj)
        throwAsCaller(MException("Igraph:wrongSize", ...
                                 "Type vector must have exactly one " + ...
                                 "element for every node."));
    end
end

function opts = parseFruchtermanReingoldOptions(adj, opts)
    arguments
        adj;
        opts.nIterations (1, 1) {mustBePositive, mustBeInteger} = 500;
        opts.startTemp (1, 1) {mustBeNonnegative} = 10;
        opts.initial (:, 2) double = [];
        opts.grid (1, 1) logical;
    end

    if opts.initial
        if size(opts.initial, 1) ~= length(adj)
            throwAsCaller(MException("Igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end

    if ~isoptionset(opts, 'grid')
        opts.grid = 'auto';
    elseif opts.grid
        opts.grid = 'grid';
    else
        opts.grid = 'nogrid';
    end
end

function opts = parseKamadaKawaiOptions(adj, opts)
    arguments
        adj;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = ...
            10 * length(adj);
        opts.epsilon (1, 1) {mustBeNonnegative} = 0;
        opts.constant (1, 1) {mustBeNonnegative} = length(adj);
        opts.initial (:, 2) double = [];
    end

    if opts.initial
        if size(opts.initial, 1) ~= length(adj)
            throwAsCaller(MException("Igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end
end

function opts = parseGemOptions(adj, opts)
    arguments
        adj;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = ...
            (40 * (length(adj) ^ 2));
        opts.tempMax (1, 1) double = length(adj);
        opts.tempMin (1, 1) double = 0.1;
        opts.tempInit (1, 1) double = sqrt(length(adj));
        opts.initial (:, 2) double = [];
    end

    if opts.initial
        if size(opts.initial, 1) ~= length(adj)
            throwAsCaller(MException("Igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end
end

function opts = parseDavidsonharelOptions(adj, opts)
    arguments
        adj;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = 10;
        opts.fineIterations (1, 1) {mustBePositive, mustBeInteger} = ...
            max(10, log2(length(adj)));
        opts.coolingFactor (1, 1) double = 0.75;
        opts.initial (:, 2) double = [];
        opts.weightNodeDist (1, 1) double = 1;
        opts.weightBorder (1, 1) double = 0;
        opts.weightEdgeLen (1, 1) double;
        opts.weightEdgeCross (1, 1) double;
        opts.weightNodeEdgeDist (1, 1) double;
    end

    if opts.initial
        if size(opts.initial, 1) ~= length(adj)
            throwAsCaller(MException("Igraph:wrongSize", ...
                                     "Initial must have exactly one " + ...
                                     "element for every node."));
        end
    end

    if ~isoptionset(opts, 'weightEdgeLen') || ...
            ~isoptionset(opts, 'weightEdgeCross') || ...
            ~isoptionset(opts, 'weightNodeEdgeDist')
        density = nnz(adj + adj') / (length(adj) ^ 2);
    end

    if ~isoptionset(opts, 'weightEdgeLen')
        opts.weightEdgeLen = density / 10;
    end

    if ~isoptionset(opts, 'weightEdgeCross')
        opts.weightEdgeCross = 1 - sqrt(density);
    end

    if ~isoptionset(opts, 'weightNodeEdgeDist')
        opts.weightNodeEdgeDist = 1 - (density / 5);
    end
end

function opts = parseMdsOptions(~, opts)
    arguments
        ~;
        opts.distance (:, :) double {mustBeSquare} = [];
    end

    if ~isempty(opts.distance) && (length(opts.distance) ~= length(adj))
        throwAsCaller(MException("Igraph:wrongSize", ...
                                 "Distance matrix must have element " + ...
                                 "for every pair of nodes (i.e. a " + ...
                                 "nNodes X nNodes matrix)."));
    end
end

function opts = parseLglOptions(adj, opts)
    arguments
        adj;
        opts.maxIterations (1, 1) {mustBePositive, mustBeInteger} = 150;
        opts.stepMax (1, 1) {mustBeNonnegative} = length(adj);
        opts.area (1, 1) {mustBeNonnegative} = length(adj) ^ 2;
        opts.coolExp (1, 1) double = 1.5;
        opts.repulseRadius (1, 1) {mustBePositive} = length(adj) ^ 3;
        opts.cellsize (1, 1) {mustBePositive} = sqrt(length(adj));
        opts.root (1, 1) {mustBePositive} = randi([1, length(adj)]);
    end
end

function opts = parseReingoldTilfordOptions(adj, opts)
    arguments
        adj;
        opts.mode (1, :) char {mustBeMode} = 'all';
        opts.root (1, :) {mustBePositive, mustBeInteger} = [];
        opts.circular (1, 1) logical = false;
    end
end

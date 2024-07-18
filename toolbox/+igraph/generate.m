function graph = generate(method, graphOpts, methodOpts)
%GENERATE create a graph with a deterministic generator
%   GRAPH = GENERATE(METHOD) use METHOD to generate a deterministic graph. Each
%       method has its own set of optional arguments, see below for more
%       information on a specific method. All resulting graphs are unweighted.
%       METHOD can be one of the following:
%
%       'star', 'wheel', 'ring', 'KaryTree', 'RegularTree', 'full',
%       'citation', 'prufer', 'atlas', 'DeBruijn', 'kautz', 'circulant',
%       'petersen'.
%
%       Method names are matched case insensitively so 'karytree', 'KaryTree',
%       'karyTree', etc are all acceptable.
%
%   GRAPH = GENERATE(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) in addition to
%       method specific parameters, there are parameters for specifying how the
%       resulting GRAPH is represented common to all methods. These follow the
%       same conventions as the rest of matlab-igraph.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'makeSparse'   Whether to return a sparse (default) or full matrix.
%        'dtype'        The data type to use, either 'double' (default) or
%                       'logical'.
%
%   GRAPH = GENERATE('star', ...) Creates a graph where every node connects to
%       only the center node.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'nNodes'   Number of nodes (defaults to 10).
%        'mode'     One of {'out', 'in', 'mutual', 'undirected' (default)}.
%        'center'   The index of the center node (defaults to 1).
%
%   GRAPH = GENERATE('wheel', ...) Creates a circular graph with a center node
%       all other edges connect to (i.e. a wheel with spokes). See also ring
%       and star generators.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'nNodes'   Number of nodes (defaults to 10).
%        'mode'     One of {'out', 'in', 'mutual', 'undirected' (default)}.
%        'center'   The index of the center node (defaults to 1).
%
%   GRAPH = GENERATE('ring', ...) Creates a circular graph where every node has
%       an edge with the next node (and consequently the previous node). If
%       circular the final node connects to the first node. See also circulant
%       and petersen generators.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nNodes'      Number of nodes (defaults to 10).
%        'isdirected'  Whether to consider the graph directed or undirected
%                      (default unless ismutual is true).
%        'ismutual'    Whether to make graph symmetric or not (default)
%                      must be directed.
%        'iscircular'  Whether to close the ring or not (default).
%
%   GRAPH = GENERATE('kary_tree', ...) Creates a tree graph where every node has
%       the same number of children.
%
%        Name        Description
%       --------------------------------------------------------------------
%        'nNodes'    Number of nodes (defaults to 10).
%        'children'  The number of children for each nodes (defaults to 3).
%        'mode'      One of {'out', 'in', 'undirected' (default)}.
%
%   GRAPH = GENERATE('regular_tree', ...) Creates a tree graph where all nodes
%       have the same degree.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'height'   Distance between the root and leaves (defaults to 4).
%        'degree'   Degree of the nodes of the tree (defaults to 3).
%        'mode'     One of {'out', 'in', 'undirected' (default)}
%
%   GRAPH = GENERATE('full', ...) Creates a full graph where every possible edge
%       exists. If undirected and no loops, this produces a 'citation' graph.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nNodes'      Number of nodes (defaults to 10).
%        'isdirected'  Whether to return a directed graph (i.e. triangular
%                      --false (default)--or symmetric--true).
%        'loops'       Whether to include loops (diagonal edges) or not
%                      (default)
%
%   GRAPH = GENERATE('citation', ...) Creates a graph where every edge for the
%       source id < destination id is present. This always produces a lower
%       triangle.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nNodes'      Number of nodes (defaults to 10).
%        'isdirected'  Whether to return a directed graph or not (default).
%
%   GRAPH = GENERATE('prufer', ...) Creates a tree graph from a Prufer sequence.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'prufer'   A vector representing the prufer sequence. The resulting
%                   graph is has n + 2 nodes where n is the number of elements
%                   in the sequence. All elements must be <= n + 2. (defaults
%                   to 1:10).
%
%   GRAPH = GENERATE('atlas', ...) Return a graph from the graph atlas.
%       only the center node. Params:
%
%        Name        Description
%       --------------------------------------------------------------------
%        'atlasId'   The index of the atlas graph to return (defaults to 1).
%                    Max index is 1253.
%
%   GRAPH = GENERATE('de_bruijn', ...) Creates a de Bruijn graph. There's an edge
%        for all unique strings of length STRINGLENGTH that can be made with
%        NLETTERS. Edges are made between nodes if shifting the string of one
%        node to the right 1 produces the other nodes strings.
%
%        Name            Description
%       --------------------------------------------------------------------
%        'nLetters'      Number of letters in the alphabet (defaults to 3).
%        'stringLength'  Length of the strings (defaults to 2).
%
%   GRAPH = GENERATE('kautz', ...) Creates a Kautz graph. There's an edge for all
%       unique strings of length STRINGLENGTH + 1 that can be made with
%       NLETTERS + 1, where no two consecutive letters are the same. Edges are
%       made between nodes if shifting the string of one node to the right 1
%       produces the other nodes strings.
%
%        Name            Description
%       --------------------------------------------------------------------
%        'nLetters'      Number of letters in the alphabet (defaults to 3).
%        'stringLength'  Length of the strings (defaults to 2).
%
%   GRAPH = GENERATE('circulant', ...) Creates a circular graph where each node
%       is connected to a node some shift nodes away. If there are more than
%       one shifts, each node has a connection for each shift.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nNodes'      Number of nodes (defaults to 10).
%        'shifts'      A vector of shifts, one edge is created for each node
%                      to the node shift away from it for each shift
%                      (defaults to [1]).
%        'isdirected'  Whether to consider the graph directed or not (default).
%
%   GRAPH = GENERATE('petersen', ...) Creates a graph with an outer and inner
%       ring. The inner ring has connections between nodes some shift away in
%       the inner ring (similar to 'circulant'). In addition there's one
%       connection between each outer ring node in a corresponding inner ring
%       node.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'nNodes'   Number of nodes (defaults to 5).
%        'shift'    The distance between nodes in the outer ring to make
%                   connections with (simiar to shifts in 'circulant' but there
%                   can only be one). Most be < nNodes / 2. (Defaults to 2.)
%
%   See also igraph.famous, igraph.game.

    arguments
        method (1, :) char {igraph.args.mustBeMemberi(method, ...
                                                      {'star', 'wheel', 'SquareLattice', ...
                                                       'TriangularLattice', ...
                                                       'hexagonallattice', 'ring', ...
                                                       'karytree', 'symmetrictree', ...
                                                       'RegularTree', 'full', ...
                                                       'FullCitation', 'citation', ...
                                                       'FullMultipartite', 'multipartite', ...
                                                       'turan', 'RealizeDegreeSequence', ...
                                                       'prufer', 'atlas', 'DeBruijn', ...
                                                       'kautz', 'circulant', 'petersen', ...
                                                       'ChordalRing'})}
        graphOpts.makeSparse (1, 1) logical = true;
        graphOpts.dtype (1, :) char ...
            {igraph.args.mustBeMemberi(graphOpts.dtype, ...
                                       {'logical', 'double'})} = 'double';
        methodOpts.nNodes;
        methodOpts.mode;
        methodOpts.center;
        methodOpts.isdirected;
        methodOpts.ismutual;
        methodOpts.iscircular;
        methodOpts.children;
        methodOpts.height;
        methodOpts.degree;
        methodOpts.loops;
        methodOpts.prufer;
        methodOpts.atlasId;
        methodOpts.nLetters;
        methodOpts.stringLength;
        methodOpts.shifts;
        methodOpts.shift;
    end

    method = lower(method);
    if strcmp(method, 'fullcitation')
        method = 'citation';
    elseif strcmp(method, 'fullmultipartite')
        method = 'multipartite';
    end

    graphOpts.dtype = lower(graphOpts.dtype);
    methodOpts = namedargs2cell(methodOpts);

    switch method
      case {'star', 'wheel'}
        methodOpts = parseOptionsStar(methodOpts{:});
      case 'ring'
        methodOpts = parseOptionsRing(methodOpts{:});
      case 'karytree'
        methodOpts = parseOptionsKaryTree(methodOpts{:});
      case 'regulartree'
        methodOpts = parseOptionsRegularTree(methodOpts{:});
      case 'full'
        methodOpts = parseOptionsFull(methodOpts{:});
      case 'citation'
        methodOpts = parseOptionsCitation(methodOpts{:});
      case 'prufer'
        methodOpts = parseOptionsPrufer(methodOpts{:});
      case 'atlas'
        methodOpts = parseOptionsAtlas(methodOpts{:});
      case {'debruijn', 'kautz'}
        methodOpts = parseOptionsKautz(methodOpts{:});
      case 'circulant'
        methodOpts = parseOptionsCirculant(methodOpts{:});
      case 'petersen'
        methodOpts = parseOptionsPetersen(methodOpts{:});
      otherwise
        error("The method %s has not been implemented.\n\n" + ...
              "Please report an issue or submit a pull request to github.", ...
              method);
    end

    graph = mexIgraphDispatcher(mfilename(), method, graphOpts, methodOpts);
end

function opts = parseOptionsStar(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.mode (1, :) char {igraph.args.mustBeMemberi(opts.mode, ...
                                                         {'out', 'in', 'mutual', ...
                                                          'undirected'})} = 'undirected';
        opts.center (1, 1) {mustBePositive, mustBeInteger} = 1;
    end

    if opts.center > opts.nNodes
        error("Invalid value for 'center' argument. Value must be less " + ...
              "than 'nNodes'.");
    end

    opts.center = opts.center - 1;
end

function opts = parseOptionsRing(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.isdirected (1, 1) logical;
        opts.ismutual (1, 1) logical = false;
        opts.iscircular (1, 1) logical = false;
    end

    if ~igraph.args.isoptionset(opts, 'isdirected')
        if opts.ismutual
            opts.isdirected = true;
        else
            opts.isdirected = false;
        end
    end

    if opts.ismutual && ~opts.isdirected
        error("Igraph:generateRing", ...
              "Cannot create mutual edges when graph is not directed. " + ...
              "ismutual can only be true when isdirected is true.")
    end
end

function opts = parseOptionsKaryTree(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.children (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.mode (1, :) char {igraph.args.mustBeMemberi(opts.mode, ...
                                                         {'out', 'in', 'undirected'})} = ...
                                                         'undirected';
    end

    opts.mode = lower(opts.mode);
end

function opts = parseOptionsRegularTree(opts)
    arguments
        opts.height (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
        opts.degree (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.mode (1, :) char {igraph.args.mustBeMemberi(opts.mode, ...
                                                         {'out', 'in', 'undirected'})} = ...
                                                         'undirected';
    end

    opts.mode = lower(opts.mode);
end

function opts = parseOptionsFull(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.isdirected (1, 1) logical = false;
        opts.loops (1, 1) logical = false;
    end
end

function opts = parseOptionsCitation(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.isdirected (1, 1) logical = false;
    end
end

function opts = parseOptionsPrufer(opts)
    arguments
        opts.prufer {mustBeInteger, mustBeVector, mustBePositive} = 1:10;
    end

    opts.prufer = opts.prufer - 1;
end

function opts = parseOptionsAtlas(opts)
    arguments
        opts.atlasId (1, 1) {mustBeNonnegative, mustBeInteger, ...
                             mustBeLessThan(opts.atlasId,1254)} = 2;
    end
    opts.atlasId = opts.atlasId - 1;
end

function opts = parseOptionsKautz(opts)
    arguments
        opts.nLetters (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.stringLength (1, 1) {mustBeNonnegative, mustBeInteger} = 2;
    end
end

function opts = parseOptionsCirculant(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.shifts {mustBeNonnegative, mustBeVector, mustBeInteger} = [1];
        opts.isdirected (1, 1) logical = false;
    end
end

function opts = parseOptionsPetersen(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 5;
        opts.shift (1, 1) {mustBeNonnegative, mustBeInteger} = 2;
    end

    if opts.nNodes < 3
        error("nNodes must be at least 3.");
    end

    if opts.shift > (opts.nNodes / 2)
        error("shift must be less than nNodes / 2.\n\n" + ...
              "nNodes = %d, shift = %d, shift must be less than %g", ...
              opts.nNodes, opts.shift, opts.nNodes / 2);
    end
end

function adj = generate(method, adjOptions, methodOptions)
%GENERATE create an adjacency matrix with a deterministic generator
%   ADJ = GENERATE(METHOD) use METHOD to generate a deterministic adjacency
%       matrix. Each method has its own set of optional arguments, see below
%       for more information on a specific method. All resulting graphs are
%       unweighted. METHOD can be one of the following:
%
%       'star', 'wheel', 'ring', 'kary_tree', 'regular_tree', 'full',
%       'citation', 'prufer', 'atlas', 'de_bruijn', 'kautz', 'circulant',
%       'petersen'.
%
%   ADJ = GENERATE(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) in addition to
%       method specific parameters, there are parameters for specifying the
%       how the resulting ADJ is represented common to all methods. These
%       follow the same conventions as the rest of matlab-igraph.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'makeSparse'   Whether to return a sparse (default) or full matrix.
%        'dtype'        The data type to use, either 'double' (default) or
%                       'logical'.
%
%   ADJ = GENERATE('star', ...) Creates a graph where every node connects to
%       only the center node.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'nNodes'   Number of nodes (defaults to 10).
%        'mode'     One of {'out', 'in', 'mutual', 'undirected' (default)}.
%        'center'   The index of the center node (defaults to 1).
%
%   ADJ = GENERATE('wheel', ...) Creates a circular graph with a center node
%       all other edges connect to (i.e. a wheel with spokes). See also ring
%       and star generators.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'nNodes'   Number of nodes (defaults to 10).
%        'mode'     One of {'out', 'in', 'mutual', 'undirected' (default)}.
%        'center'   The index of the center node (defaults to 1).
%
%   ADJ = GENERATE('ring', ...) Creates a circular graph where every node has
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
%   ADJ = GENERATE('kary_tree', ...) Creates a tree graph where every node has
%       the same number of children.
%
%        Name        Description
%       --------------------------------------------------------------------
%        'nNodes'    Number of nodes (defaults to 10).
%        'children'  The number of children for each nodes (defaults to 3).
%        'mode'      One of {'out', 'in', 'undirected' (default)}.
%
%   ADJ = GENERATE('regular_tree', ...) Creates a tree graph where all nodes
%       have the same degree.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'height'   Distance between the root and leaves (defaults to 4).
%        'degree'   Degree of the nodes of the tree (defaults to 3).
%        'mode'     One of {'out', 'in', 'undirected' (default)}
%
%   ADJ = GENERATE('full', ...) Creates a full graph where every possible edge
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
%   ADJ = GENERATE('citation', ...) Creates a graph where every edge for the
%       source id < destination id is present. This always produces a lower
%       triangle.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nNodes'      Number of nodes (defaults to 10).
%        'isdirected'  Whether to return a directed graph or not (default).
%
%   ADJ = GENERATE('prufer', ...) Creates a tree graph from a Prufer sequence.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'prufer'   A vector representing the prufer sequence. The resulting
%                   graph is has n + 2 nodes where n is the number of elements
%                   in the sequence. All elements must be <= n + 2. (defaults
%                   to 1:10).
%
%   ADJ = GENERATE('atlas', ...) Return a graph from the graph atlas.
%       only the center node. Params:
%
%        Name        Description
%       --------------------------------------------------------------------
%        'atlasId'   The index of the atlas graph to return (defaults to 1).
%                    Max index is 1253.
%
%   ADJ = GENERATE('de_bruijn', ...) Creates a de Bruijn graph. There's an edge
%        for all unique strings of length STRINGLENGTH that can be made with
%        NLETTERS. Edges are made between nodes if shifting the string of one
%        node to the right 1 produces the other nodes strings.
%
%        Name            Description
%       --------------------------------------------------------------------
%        'nLetters'      Number of letters in the alphabet (defaults to 3).
%        'stringLength'  Length of the strings (defaults to 2).
%
%   ADJ = GENERATE('kautz', ...) Creates a Kautz graph. There's an edge for all
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
%   ADJ = GENERATE('circulant', ...) Creates a circular graph where each node
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
%   ADJ = GENERATE('petersen', ...) Creates a graph with an outer and inner
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
        method (1, :) char {mustBeMemberi(method, ...
                                          {'star', 'wheel', 'square_lattice', ...
                                           'triangular_lattice', ...
                                           'hexagonal_lattice', 'ring', ...
                                           'kary_tree', 'symmetric_tree', ...
                                           'regular_tree', 'full', ...
                                           'full_citation', 'citation', ...
                                           'full_multipartite', 'multipartite', ...
                                           'turan', 'realize_degree_sequence', ...
                                           'prufer', 'atlas', 'de_bruijn', ...
                                           'kautz', 'circulant', 'petersen', ...
                                           'chordal_ring'})}
        adjOptions.makeSparse (1, 1) logical = true;
        adjOptions.dtype (1, :) char {mustBeMemberi(adjOptions.dtype, ...
                                                    {'logical', 'double'})} = ...
                                                    'double';
        methodOptions.nNodes;
        methodOptions.mode;
        methodOptions.center;
        methodOptions.isdirected;
        methodOptions.ismutual;
        methodOptions.iscircular;
        methodOptions.children;
        methodOptions.height;
        methodOptions.degree;
        methodOptions.loops;
        methodOptions.prufer;
        methodOptions.atlasId;
        methodOptions.nLetters;
        methodOptions.stringLength;
        methodOptions.shifts;
        methodOptions.shift;
    end

    method = lower(method);
    if strcmp(method, 'full_citation')
        method = 'citation';
    elseif strcmp(method, 'full_multipartite')
        method = 'multipartite';
    end

    adjOptions.dtype = lower(adjOptions.dtype);
    requiredArguments = namedargs2cell(adjOptions);
    methodOptions = namedargs2cell(methodOptions);
    generatorWrapper = @(varargin) ...
        mexIgraphGenerate(method, requiredArguments{2:2:end}, varargin{:});

    switch method
      case {'star', 'wheel'}
        methodOptions = parseOptionsStar(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.mode, ...
                               methodOptions.center);
      case 'ring'
        methodOptions = parseOptionsRing(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.isdirected, ...
                               methodOptions.ismutual, methodOptions.iscircular);
      case 'kary_tree'
        methodOptions = parseOptionsKaryTree(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.children, methodOptions.mode);
      case 'regular_tree'
        methodOptions = parseOptionsRegularTree(methodOptions{:});
        adj = generatorWrapper(methodOptions.height, methodOptions.degree, ...
                               methodOptions.mode);
      case 'full'
        methodOptions = parseOptionsFull(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.isdirected, methodOptions.loops);
      case 'citation'
        methodOptions = parseOptionsCitation(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.isdirected);
      case 'prufer'
        methodOptions = parseOptionsPrufer(methodOptions{:});
        adj = generatorWrapper(methodOptions.prufer);
      case 'atlas'
        methodOptions = parseOptionsAtlas(methodOptions{:});
        adj = generatorWrapper(methodOptions.atlasId);
      case {'de_bruijn', 'kautz'}
        methodOptions = parseOptionsKautz(methodOptions{:});
        adj = generatorWrapper(methodOptions.nLetters, ...
                               methodOptions.stringLength);
      case 'circulant'
        methodOptions = parseOptionsCirculant(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.shifts, ...
                               methodOptions.isdirected);
      case 'petersen'
        methodOptions = parseOptionsPetersen(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.shift);
      otherwise
        error("The method %s has not been implemented.\n\n" + ...
              "Please report an issue or submit a pull request to github.", ...
              method);
    end

end

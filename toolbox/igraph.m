%IGRAPH high level interface for the igraph C library
%   The IGRAPH toolbox connects directly with MATLAB's builtin graph class
%   types GRAPH and DIGRAPH adding many efficient graph algorithms.
%
%   IGRAPH provides methods for obtaining graphs, working with graphs, and
%   saving the results.
%
%   Obtaining graphs can be done through reading from file (IGRAPH.LOAD) or
%   generating graphs (IGRAPH.RANDGAME, IGRAPH.GENERATE, IGRAPH.FAMOUS), or
%   modifying existing graphs (IGRAPH.REWIRE).
%
%   IGRAPH gives access to many graph algorithms to work with graphs that have
%   been loaded into memory. Such as testing graphs (IGRAPH.ISTREE,
%   IGRAPH.ISSUBISOMORPHIC), community detection (IGRAPH.CLUSTER), and testing
%   membership partitions (IGRAPH.COMPARE, IGRAPH.MODULARITY).
%
%   Graphs in memory can be saved to various graph file formats through
%   (IGRAPH.SAVE). This allows saving modified graphs for later use as well as
%   saving graphs from MATLAB into a format that can be used by external
%   programs. Additionally, the IGRAPH.CONVERT function can convert between
%   different file formats (including to mat files to improve speed of reading
%   into MATLAB).
%
%   Common IGRAPH Arguments:
%       Functions that accept graphs as arguments and functions that return
%       graphs have common sets of arguments.
%
%       IGRAPH functions accepting graphs:
%       Functions that accept a graph will accept the ISDIRECTED argument and
%       if the method can use weights, the ISWEIGHTED and WEIGHT arguments.
%
%       ISDIRECTED is always available to functions that accept a graph. By
%       default this will be the result of IGRAPH.ISDIRECTED on the graph. This
%       can be overridden in the case a graph that appears undirected (a
%       symmetric graph or a triangular graph) should be treated as directed by
%       passing TRUE to this argument. If the graph is not symmetric or
%       triangular, and this is set to FALSE, an error is likely to occur.
%
%       ISWEIGHTED can be set if the algorithm works on weighted graphs. By
%       default this is the result of IGRAPH.ISWEIGHTED, by passing TRUE to
%       this argument, the graph will be treated as weighted even if weights
%       are binary (either implied by the existence of edges without explicit
%       weights or if weights are supplied and they are all 0 or 1). By passing
%       false to the argument, the weights will always be ignored. If a
%       weighted graph is passed to a function that cannot handle weights, the
%       weights will be ignored without warning.
%
%       WEIGHT is an additional argument that is available whenever the
%       ISWEIGHTED argument is available. When the provided graph is a in the
%       'graph' representation (either a GRAPH or DIGRAPH class object), the
%       WEIGHT argument can be set to the name of an edge attribute (edge
%       attributes are variables in the graph's edge table, not including the
%       'EndNodes' variable). By default it will use the edge attribute named
%       'Weight' (capital 'W', attribute names are case-sensitive). If the
%       WEIGHT argument given a name not in the graph's edge attribute list an
%       error will be thrown. If the WEIGHT argument is set but the graph is
%       not of a 'graph' representation (see REPR option in functions returning
%       graphs section) a warning will be printed and the value of WEIGHT will
%       be ignored.
%
%       If the results of the function are of the form of a vector with one
%       value per node or edge and the graph is in a 'graph' representation,
%       the RESULT argument can be used to provide the name of the attribute to
%       store the results. If left empty the vector will be returned as an
%       output.
%
%       IGRAPH functions returning graphs:
%       Functions that return a graph by default will return a graph in the
%       'graph' representation (either a MATLAB GRAPH object if undirected or
%       DIGRAPH object if directed). If there are weights they will be placed
%       in the edge attribute 'Weights' (Capital 'W', attribute names are
%       case-sensitive). A exception to this rule is functions that modify a
%       graph. In this case, the graph return will default to the same
%       characteristics as the original graph.
%
%       Using MATLAB graphs has several benefits, including the ability to
%       handle multiple edges between a single node pair (multigraphs), store
%       attributes directly in the graph object, and use the methods provided
%       by the GRAPH / DIGRAPH classes. For the case against using the GRAPH
%       classes, where graphs are large, creating GRAPH objects will be slower
%       than raw matrices (however this may be insignificant compare to the
%       time of running algorithms on the large graphs). Secondly it may be
%       easier to manipulate matrices directly in MATLAB due to MATLAB's simple
%       matrix syntax.
%
%       To change the format a graph is returned in, the arguments: REPR,
%       DTYPE, and WEIGHT can be passed to any function that returns a graph
%       (including those that modify a graph).
%
%       REPR describes the graphs representation can be one of the strings
%       'full', 'sparse', or 'graph' (default). If REPR is 'full' the graph
%       will be represented as an adjacency matrix. 'sparse' will also
%       represent the graph as an adjacency matrix but will be in a sparse
%       matrix instead of a full matrix. This can significantly reduce memory
%       requirements for large graphs with a low edge density. Lastly 'graph'
%       will return a graph as either a MATLAB GRAPH or DIGRAPH object
%       depending on if the graph is directed or not.
%
%       DTYPE changes the data type used to store the graph. It can be either
%       'double' (default) or 'logical'. 'logical' can reduce memory
%       requirements but it will always drop any weights the graph may have. In
%       the case of a 'graph' representation, the weights attribute will be
%       omitted altogether rather than storing 1 for every edge.
%
%       WEIGHT is the same as for input graphs. When the return representation
%       is 'graph', the WEIGHT argument can be used to store WEIGHTS in
%       an edge attribute different than the default 'Weight' name. If a value
%       is passed when REPR is not 'graph' a warning will be printed and the
%       value will be ignored.
%
%   Examples:
%      Get the well known Zachary Karate Club graph, display information about
%      it, plot it and save to a file to use elsewhere.
%          g = igraph.famous('zachary');
%          igraph.summary(g);
%          igraph.plot(g, 'kk');
%          igraph.save('zkc.edges', g);
%
%   See also GRAPH, DIGRAPH.

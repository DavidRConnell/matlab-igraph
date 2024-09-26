function graph = randgame(method, graphOpts, methodOpts)
%RANDGAME create graph with a stochastic game generator
%   GRAPH = GAME(METHOD) use METHOD to generate a random graph. Results can be
%   reproduced by setting the random generator using IGRAPH.RNG. Each method
%   has its own set of optional arguments, see below for more information on a
%   specific method. All resulting graphs are unweighted. METHOD can be one of
%   the following:
%
%       'grg', 'BarabasiBag', 'BarabasiPsumTree', 'BarabasiPsumTreeMultiple',
%       'ErdosRenyi', 'WattsStrogatz', 'SmallWorld', 'DegSeqConfiguration',
%       'DegSeqConfigurationSimple', 'DegSeqEdgeSwitchingSimple',
%       'DegSeqFastHeurSimple', 'DegSeqVl', 'kRegular', 'StaticFitness',
%       'StaticPowerLaw', 'ForestFire', 'GrowingRandom', 'CallawayTraits',
%       'establishment', 'preference', 'AsymmetricPreference', 'RecentDegree',
%       'BarabasiAging', 'RecentDegreeAging', 'LastCit', 'CitedType',
%       'CitingCitedType', 'sbm', 'hsbm', 'DotProduct', 'TreePrufer',
%       'TreeLERW', 'SimpleInterconnectedIslands'.
%
%       Note: The method name is case insensitive, for example, 'barabasibag',
%       'barabasiBag', and 'BarabasiBag', are all allowed.
%
%   GRAPH = GENERATE(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) in addition to
%   method specific parameters, there are parameters REPR and DTYPE for
%   specifying how the resulting GRAPH is represented common to all methods.
%   See the "functions returning graphs" section in help IGRAPH for more
%   information.
%
%   Note: The default values for most method options are chosen somewhat
%   randomly primarily for demonstration purposes.
%
%   GRAPH = GAME('GeneratorRandomGraph', ...)
%   GRAPH = GAME('grg', ...) Creates a graph by randomly placing points on a
%   square and connecting those within radius distance of each other.
%
%         Name       Description
%       --------------------------------------------------------------------
%        'nNodes'   Number of nodes (defaults to 10).
%        'radius'   How close two points need to be for an edge to form
%                   (default 0.1).
%        'torus'    Whether boundaries wrap (true) or not (false, default).
%
%   GRAPH = GAME('BarabasiPsumTree', ...)
%   GRAPH = GAME('BarabasiPsumTreeMultiple', ...)
%   GRAPH = GAME('BarabasiBag', ...) Generate graphs using a Barabàsi Albert
%   model. Produces a graph where each node is initially connected to
%   nConnections other nodes. Nodes are added one at a time and the probability
%   a new node matches with another node is related to the number of edges the
%   other node has made. There are three variants of this method. The Barabàsi
%   Bag is the basic algorithm, the other two methods use a prefix-sum tree,
%   either with or without multiple edges.
%
%         Name             Description
%       --------------------------------------------------------------------
%        'nNodes'         Number of nodes (defaults to 10 or the length of
%                         'nConnections' if that is a vector).
%        'power'          Power of preferential attachment. The larger power
%                         the stronger the preference for highly connected
%                         nodes (defaults to 1 and must be one for
%                         'BarabasiBag').
%        'nConnections'   The number of edges a new node should make with other
%                         nodes. If it is a vector, it should have length equal
%                         to the number of nodes in the graph. Defaults to 1.
%        'outPreference'  Whether to use out degree only (true, default) or use
%                         total degree (false).
%        'attractiveness' A constant to add the probability that each node is
%                         connected to ensure 0 degree nodes have some
%                         likelihood of making a connection.
%        'isdirected'     Whether to generate a directed (true) or undirected
%                         (false) graph. When undirected, it overrides
%                         'outPreference'. Defaults to false or the
%                         directedness of 'startfrom'.
%        'startFrom'      A graph to use as the starting point (a
%                         'nConnections' clique by default).
%
%   GRAPH = GAME('RenyiErdos', ...) Generate graphs with a fixed number of
%   edges or a constant probability of an edge existing. Either select the
%   number of edges or the probability any given edge exists. Cannot select
%   both NEDGES and PROBABILITY.
%
%         Name           Description
%       --------------------------------------------------------------------
%        'nNodes'       Number of nodes (defaults to 10)
%        'nEdges'       Number of edges (defaults to 10)
%        'probability'  The probability a random edge exists (ignored by
%                       default)
%        'isdirected'   Whether the graph should be directed (true) or
%                       undirected (false, default).
%        'loops'        Whether to allow loops (true). Defaults to false.
%
%   GRAPH = GAME('SmallWorld', ...)
%   GRAPH = GAME('WattsStrogatz', ...) Uses the Watts-Strogatz model to generate
%   a graph with small-world properties. This method rewires a lattice graph.
%
%         Name           Description
%       --------------------------------------------------------------------
%        'dim'          The dimension of the lattice (defaults to 2)
%        'size'         Length of each dimension (defaults to 4)
%        'radius'       Distance, in steps from node, to connect nodes
%                       (defaults to 1). Edges are made between nodes if they
%                       are at least this close on the grid.
%        'probability'  Probability of each edge is rewired (defaults to 0.2).
%        'loops'        Whether to allow loops (true). Defaults to false.
%        'multiple'     Whether multiple edges are allowed between node pairs
%                       (default false, requires the use a 'graph'
%                       representation).
%
%   GRAPH = GAME('DegSeqConfiguration', ...)
%   GRAPH = GAME('DegSeqConfigurationSimple', ...)
%   GRAPH = GAME('DegSeqFastHuerSimple', ...)
%   GRAPH = GAME('DegSeqEdgeSwitchingSimple', ...)
%   GRAPH = GAME('DegSeqVl', ...) Create a graph with the provided degree
%   sequence.
%
%   For creating directed graphs both INDEGREE and OUTDEGREE must be specified
%   and the same length. For undirected graphs, only DEGREE should be
%   specified.
%
%         Name         Description
%       --------------------------------------------------------------------
%        'degree'     The degree sequence for an undirected graph (default a
%                     vector of 10 3s).
%        'outDegree'  Degree sequence for edges leaving the nodes (no default).
%        'inDegree'   Degree sequence for edges entering the nodes (no default).
%
%   GRAPH = GAME('kRegular', ...) Generate a graph where each node has the same
%   degree.
%
%         Name          Description
%       --------------------------------------------------------------------
%        'nNodes'      Number of nodes (default to 10).
%        'degree'      The degree of all nodes (default to 3).
%        'isdirected'  Whether graph should be directed (true) or undirected
%                      (false, default).
%
%   GRAPH = GAME('StaticFitness', ...) Use node fitness scores to determine edge
%   probabilities.
%
%   When creating a directed graph, both 'infitness' and 'outfitness' must be
%   specified and the same length. For undirected graphs use 'fitness'.
%
%         Name          Description
%       --------------------------------------------------------------------
%        'nEdges'      The number of edges the graph should have (default 20).
%        'fitness'     A vector of fitness scores for each node, use for
%                      undirected graphs (default a vector of 10 3s).
%        'inFitness'   The out fitness vector for directed graphs (no default).
%        'outFitness'  The in fitness vector for directed graphs(no default).
%        'loops'       Whether to allow self-loop edges (default false).
%
%   GRAPH = GAME('StaticPowerLaw', ...) Generate a graph a power-law node degree
%   distribution
%
%   When creating a directed graph, both 'inexponent' and 'outexponent' must be
%   specified. For undirected graphs use 'exponent'. Values must be greater
%   than or equal to 2.
%
%         Name                   Description
%       --------------------------------------------------------------------
%        'nNodes'               The number of nodes the graph should have
%                               (default 10).
%        'nEdges'               The number of edges the graph should have
%                               (default 20).
%        'exponent'             The power-law exponent to model the degree
%                               distribution on (default 2).
%        'inExponent'           The power-law exponent for the in degree
%                               (only if directed, no default).
%        'outExponent'          The out degree power-law exponent (if directed,
%                               no default)
%        'loops'                Whether to allow self-loop edges (default
%                               false).
%        'finiteSizeCorrection' Whether to use sie correction (default false).
%
%   GRAPH = GAME('ForestFire', ...) Create a graph with the forest fire model.
%
%         Name                 Description
%       --------------------------------------------------------------------
%        'nNodes'             The number of nodes the graph should have
%                             (default 10).
%        'forwardProbability' forward burning probability (default 0.5).
%        'backwardFactor'     backward factor (default 1).
%        'nAmbassadors'       Number of ambassador vertices (default 3).
%        'isdirected'         Whether graph should be directed (default false).
%
%   GRAPH = GAME('GrowingRandom', ...) Grow a graph by randomly adding a nodes
%   one at a time.
%
%         Name           Description
%       --------------------------------------------------------------------
%        'nNodes'       The number of nodes the graph should have (default 10).
%        'edgesPerStep' The number of edges to add each step (default 4).
%        'isdirected'   Whether graph should be directed (default false).
%        'citation'     Whether all new edges start from the most recent node
%                       (default false).
%
%   GRAPH = GAME('CallawayTraits', ...)
%   GRAPH = GAME('establishment', ...) Randomly add nodes of different types and
%   connect them based the preference for that type to connect to another.
%
%   For 'CallawayTraits', 'edgesPerStep' new edges are made each time step. For
%   'establishment', 'edgesPerStep' new edges are tried. In 'CallawayTraits'
%   the tries are made until 'edgesPerStep' new edges whereas 'establishment'
%   only makes that many tries regardless of howmany new edges are actually
%   made. The success of an attempt is dependent on the node types selected for
%   the attempt and the the probability that edges are made between those two
%   types (PREFERENCE(i, j)).
%
%         Name               Description
%       --------------------------------------------------------------------
%        'nNodes'           The number of nodes the graph should have
%                           (default 10).
%        'edgesPerStep'     The number of edges to add each step (default 4).
%        'nTypes'           Number of unique node types (default 4 or length
%                           of 'typeDistribution').
%        'typeDistribution' Distribution of node types (default uniform
%                           distribution of length 'nTypes').
%        'preference'       'nTypes' X 'nTypes' connection preference matrix
%                           (default uniform distribution).
%        'mixingParam'      Generates a uniform preference matrix with
%                           'mixingParam' as the probability a node in one type
%                           connects to a node in a different type and (1 -
%                           'mixingParam') as the probability a node connects
%                           with a node in the same type. Only one of
%                           'preference' and 'mixingParam' can be set.
%        'isdirected'       Whether graph should be directed (default false
%                           unless preference is specified and non-symmetric).
%
%   GRAPH = GAME('preference', ...) Like 'CallawayTraits' and 'establishment'
%   but all nodes are generated first, then an attempt to make an edge is made
%   for all node pairs. Success is dependent on the preference matrix and the
%   type of each node pair.
%
%   Only one of 'typeDistribution' or 'blockSizes' can be supplied.
%
%         Name               Description
%       --------------------------------------------------------------------
%        'nNodes'           The number of nodes the graph should have
%                           (default 10 or sum(BLOCKSIZES) if set).
%        'nTypes'           Number of unique node types (default 4 or length
%                           of 'typeDistribution').
%        'typeDistribution' Distribution of node types (default uniform
%                           distribution of length 'nTypes').
%        'blockSizes'       Number of nodes in each type (block) (no default).
%        'preference'       'nTypes' X 'nTypes' connection preference matrix
%                           (default uniform distribution).
%        'mixingParam'      Generates a uniform preference matrix with
%                           'mixingParam' as the probability a node in one type
%                           connects to a node in a different type and (1 -
%                           'mixingParam') as the probability a node connects
%                           with a node in the same type. Only one of
%                           'preference' and 'mixingParam' can be set.
%        'isdirected'       Whether graph should be directed (default false
%                           unless preference is specified and non-symmetric).
%        'loops'            Whether to allow self-loop edges (default false).
%
%   GRAPH = GAME('AsymmetricPreference', ...) Like preference but each node is
%   given two node types, an ingoing and outgoing.
%
%   Graph is always directed.
%
%        Name               Description
%       --------------------------------------------------------------------
%        'nNodes'           The number of nodes the graph should have
%                           (default 10).
%        'nOutTypes'        Number of unique node types (default 4 or
%                           size(TYPEDISTRIBUTION, 1)
%        'nInTypes'         Number of unique node types (default 4 or
%                           size(TYPEDISTRIBUTION, 2)
%        'typeDistribution' Distribution of node types (default uniform
%                           distribution of size 'nOutTypes' X 'nInTypes').
%        'preference'       'nOutTypes' X 'nInTypes' connection preference
%                           matrix (default uniform distribution).
%        'loops'            Whether to allow self-loop edges (default false).
%
%   GRAPH = GAME('RecentDegree', ...) Randomly add edges to nodes proportional to
%   the number of edges the the node has recently gained.
%
%         Name            Description
%       --------------------------------------------------------------------
%        'nNodes'        The number of nodes the graph should have
%                        (default 10 or length of 'edgesPerStep' if a vector).
%                        Used as the number of time steps in the simulation.
%        'degreeExp'     Probability a node gains a new edge is proportional
%                        to the number of edges it has recently gained to
%                        'degreeExp' (default 1).
%        'timeWindow'    Include an edge as recent if it was created within
%                        'timeWindow' time steps of now (default 3). (Should
%                        be less than 'nNodes'.)
%        'edgesPerStep'  How many edges to add in a time step. Either a vector
%                        of length 'nNodes' or a scalar to use the same value
%                        for each time step (default 3).
%        'outPreference' Whether to count edges that started from a node
%                        (during it's time step). (default false.)
%        'zeroAppeal'    A baseline probability so nodes without recent edges
%                        don't have zero likelihood of gaining an edge
%                        (default 0.1).
%        'isdirected'    Whether graph should be directed (default false unless
%                        preference is specified and non-symmetric).
%
%   GRAPH = GAME('BarabasiAging', ...) Similar to 'RecentDegree' but adds in an
%   age component such that older nodes are preferred. Unlike 'RecentDegree'
%   all edges are taken into consideration, not just recent edges. One node is
%   added every timestep.
%
%         Name               Description
%       --------------------------------------------------------------------
%        'nNodes'           The number of nodes the graph should have
%                           (default 10).
%        'edgesPerStep'     How many edges to add in a time step. Either a
%                           vector of length 'nNodes' or a scalar to use the
%                           same value for each time step (default 3).
%        'outPreference'    Whether to count edges that started from a node
%                           (during it's time step). (default false.)
%        'zeroDegreeAppeal' (default 0.1).
%        'degreeCoef'       (default 1).
%        'degreeExp'        (default 1).
%                           The degree component of the probability a node gets
%                           an edge is 'zeroDegreeAppeal' + 'degreeCoef' *
%                           degree ^ 'degreeExp'.
%        'zeroAgeAppeal'    (default 0.1).
%        'ageCoef'          (default 1).
%        'ageExp'           (default 1).
%                           The age component of the probability a node gets
%                           an edge is 'zeroAgeAppeal' + 'ageCoef' * age ^
%                           'ageExp'
%        'agingBins'        How many time steps represent 1 age. (If 3, a
%                           node gains 1 to it's age every three time steps.)
%                           (default 1).
%        'isdirected'       Whether graph should be directed (default false).
%
%   GRAPH = GAME('RecentDegreeAging', ...) 'recentDegree' with aging component.
%
%         Name            Description
%       --------------------------------------------------------------------
%        'nNodes'        The number of nodes the graph should have
%                        (default 10).
%        'edgesPerStep'  How many edges to add in a time step. Either a vector
%                        of length 'nNodes' or a scalar to use the same value for
%                        each time step (default 3).
%        'outPreference' Whether to count edges that started from a node
%                        (during it's time step). (default false.)
%        'zeroAppeal'    The baseline probability a node gains an edge
%                        (default 0.1).
%        'degreeExp'     The degree component of the probability a node gets
%                        an edge is degree ^ 'degreeExp' (default 1).
%        'ageExp'        The age component of the probability a node gets
%                        an edge is age ^ 'ageExp'
%        'agingBins'     How many time steps represent 1 age. (If 3, a
%                        node gains 1 to it's age every three time steps.)
%                        (default 1).
%        'timeWindow'    Include an edge as recent if it was created within
%                        'timeWindow' time steps of now (default 3). (Should
%                        be less than 'nNodes'.)
%        'isdirected'    Whether graph should be directed (default false).
%
%   GRAPH = GAME('LastCit', ...) Simulation that makes edges proportional to how
%   recently a node was last cited. Nodes are binned based on the last time
%   they were cited. Note: this is different than other methods which use
%   node creation as age.
%
%         Name           Description
%       --------------------------------------------------------------------
%        'nNodes'       The number of nodes the graph should have (default 10).
%        'edgesPerStep' How many edges to add in a time step (default 3).
%                       (Must be an integer not a vector.)
%        'agingBins'    The number of age bins to use (default 'nNodes').
%        'preference'   A vector of length 'agingBins' + 1 with the preferences
%                       for each age bin (default ones(1, AGINGBINS + 1)).
%        'isdirected'   Whether graph should be directed (default false).
%
%   GRAPH = GAME('CitedType', ...) Create a network one node at a time and add
%   edges with other nodes with preference based on node types
%
%         Name           Description
%       --------------------------------------------------------------------
%        'edgesPerStep' How many edges to add in a time step (default 3).
%        'types'        Vector of node types (labeled from 1). The number of
%                       nodes in the resulting graph is length 'types'.
%                       (default 1:10.).
%        'preference'   Vector of type preference of length max(TYPES).
%                       (default ones(1, max(TYPES))).
%        'isdirected'   Whether graph should be directed (default false).
%
%   GRAPH = GAME('CitingCitedType', ...) Generate a graph by adding edges with
%   probability given by the preference matrix and the types of both the nodes
%   being wired.
%
%         Name           Description
%       --------------------------------------------------------------------
%        'edgesPerStep' How many edges to add in a time step (default 3).
%        'types'        Vector of node types (labeled from 1). The number of
%                       nodes in the resulting graph is length TYPES. (default
%                       1:10.)
%        'preference'   Matrix of type preference of size max(TYPES) X
%                       max(TYPES). (default ones(max(TYPES), max(TYPES)).)
%        'isdirected'   Whether graph should be directed (default false).
%
%   GRAPH = GAME('StochasticBlockModel', ...)
%   GRAPH = GAME('sbm', ...) Create a block graph where edges are created with
%   probability dependent on which block each node is a member of.
%
%         Name         Description
%       --------------------------------------------------------------------
%        'blockSizes' A vector containing the sizes for each block. Number of
%                     nodes is equal to sum(BLOCKSIZES). (default ones(1, 5) *
%                     2)
%        'preference' A matrix of the probabilities for an edge between nodes
%                     of different blocks. Size should be K X K, where K is the
%                     number of blocks (length(BLOCKSIZES)). (default
%                     ones(K, K) / (K ^ 2).)
%        'isdirected' Whether graph should be directed (default false).
%        'loops'      Whether to allow self-loop edges (default false).
%
%   GRAPH = GAME('HierarchicalStochasticBlockMethod', ...)
%   GRAPH = GAME('hsbm', ...) Generate a hierarchical graph with the
%   hierarchical stochastic block method.
%
%   NOTE: NNODES, BLOCKSIZES, and RHO are all dependent on each other, so
%   changing one default may require changing the others.
%
%         Name         Description
%       --------------------------------------------------------------------
%        'nNodes'     The number of nodes in the graph (default 20).
%        'blockSizes' The number of nodes in a block (default 5).
%        'rho'        Fraction of a block's node in each within block cluster.
%                     (default [2 2 1] / 5).
%        'preference' Matrix of size K X K, where K is the number of within
%                     block clusters (length(RHO)), with the preference for a
%                     node from 1 cluster to connect to a node in another
%                     cluster.
%        'pOut'       Probability a node in one block connects with a node in
%                     a different block (default 0.3).
%
%   GRAPH = GAME('DotProduct', ...) Create a graph where the probability an
%   edge exists is the dot product between the two nodes latent positions.
%
%         Name         Description
%       --------------------------------------------------------------------
%        'positions'  A matrix with nNodes columns. Each column represents
%                     the position of a node (default ones(3, 10) / 3).
%        'isdirected' Whether graph should be directed (default false).
%
%   GRAPH = GAME('TreePrufer', ...)
%   GRAPH = GAME('TreeLERW', ...) Generate random tree graphs.
%
%   Graphs can be made with two methods, Prufer and loop-erased random walk.
%   For Prufer, ISDIRECTED must be false.
%
%         Name         Description
%       --------------------------------------------------------------------
%        'nNodes'     The number of nodes in the graph (default 10).
%        'isdirected' Whether graph should be directed (default false).
%
%   GRAPH = GAME('SimpleInterConnectedIslands', ...) Create a network with
%   islands, where nodes within an island are more likely to connect than nodes
%   between an island.
%
%         Name          Description
%       --------------------------------------------------------------------
%        'nIslands'    The number of islands in the graph (default 5).
%        'islandSize'  Size of each island (default 4).
%        'pIn'         Within island edge probability (default 0.75).
%        'nInterEdges' The number of edges between two islands (default 4).
%
%   See also IGRAPH.FAMOUS, IGRAPH.GENERATE, IGRAPH.REWIRE, IGRAPH.RNG.

    arguments
        method (1, :) char ...
            {igutils.mustBeMemberi(method, ...
                                   {'grg', 'GeneratorRandomGraph' 'BarabasiBag', ...
                                    'BarabasiPsumtree', ...
                                    'BarabasiPsumtreeMultiple', 'ErdosRenyi', ...
                                    'WattsStrogatz', 'SmallWorld', ...
                                    'DegSeqConfiguration', ...
                                    'DegSeqConfigurationSimple', ...
                                    'DegSeqFastHeurSimple', ...
                                    'DegSeqEdgeSwitchingSimple', 'DegseqVl', ...
                                    'KRegular', 'StaticFitness', ...
                                    'StaticPowerLaw', 'ForestFire', ...
                                    'GrowingRandom', 'CallawayTraits', ...
                                    'establishment', 'preference', ...
                                    'AsymmetricPreference', 'RecentDegree', ...
                                    'BarabasiAging', 'RecentDegreeAging', ...
                                    'lastcit', 'CitedType', 'CitingCitedType', ...
                                    'sbm', 'StochasticBlockMethod', 'hsbm', ...
                                    'HierachicalStochasticBlockMethod', 'HsbmList', ...
                                    'DotProduct', 'treeLERW', 'treePrufer', ...
                                    'SimpleInterconnectedIslands'})};
        graphOpts.?igutils.GraphOutProps;
        methodOpts.nNodes;
        methodOpts.radius;
        methodOpts.torus;
        methodOpts.power;
        methodOpts.nConnections;
        methodOpts.outPreference;
        methodOpts.attractiveness;
        methodOpts.isdirected;
        methodOpts.startFrom;
        methodOpts.nEdges;
        methodOpts.loops;
        methodOpts.multiple;
        methodOpts.probability;
        methodOpts.dim;
        methodOpts.size;
        methodOpts.degree;
        methodOpts.outDegree;
        methodOpts.inDegree;
        methodOpts.fitness;
        methodOpts.outFitness;
        methodOpts.inFitness;
        methodOpts.outExponent;
        methodOpts.inExponent;
        methodOpts.finiteSizeCorrection;
        methodOpts.forwardProbability;
        methodOpts.backwardFactor;
        methodOpts.nAmbassadors;
        methodOpts.citation;
        methodOpts.nTypes;
        methodOpts.edgesPerStep;
        methodOpts.typeDistribution;
        methodOpts.nOutTypes;
        methodOpts.nInTypes;
        methodOpts.timeWindow;
        methodOpts.zeroAppeal;
        methodOpts.degreeExp;
        methodOpts.ageExp;
        methodOpts.agingBins;
        methodOpts.zeroDegreeAppeal;
        methodOpts.zeroAgeAppeal;
        methodOpts.degreeCoef;
        methodOpts.ageCoef;
        methodOpts.preference;
        methodOpts.mixingParam;
        methodOpts.types;
        methodOpts.blockSizes;
        methodOpts.clusterSizes;
        methodOpts.pOut;
        methodOpts.positions;
        methodOpts.nIslands;
        methodOpts.islandSize;
        methodOpts.pIn;
        methodOpts.nInterEdges;
    end

    method = lower(method);
    switch method
      case 'generatorrandomgraph'
        method = 'grg';
      case 'smallworld'
        method = 'wattsstrogatz';
      case 'stochasticblockmethod'
        method = 'sbm';
      case 'hierarchicalstochasticblockmethod'
        method = 'hsbm';
      case {'treelerw', 'treeprufer'}
        methodOpts.method = method(length('tree')+1:end);
        method = 'tree';
    end

    graphOpts = namedargs2cell(graphOpts);
    graphOpts = igutils.setGraphOutProps(graphOpts{:});
    methodOpts = namedargs2cell(methodOpts);

    switch method
      case 'grg'
        methodOpts = parseOptionsGrg(methodOpts{:});
      case {'barabasibag', 'barabasipsumtree', 'barabasipsumtreemultiple'}
        methodOpts = parseOptionsBarabasi(method, methodOpts{:});
      case 'erdosrenyi'
        methodOpts = parseOptionsErdosRenyi(methodOpts{:});
        if igutils.isoptionset(methodOpts, "probability")
            methodOpts.useEdges = false;
        else
            methodOpts.useEdges = true;
        end
      case 'wattsstrogatz'
        methodOpts = parseOptionsWattsStrogatz(methodOpts{:});
      case {'degseqconfiguration', 'degseqconfigurationsimple', ...
            'degseqfastheursimple', 'degseqedgeswitchingsimple', 'degseqvl'}
        methodOpts = parseOptionsDegSeq(methodOpts{:});
      case 'kregular'
        methodOpts = parseOptionsKRegular(methodOpts{:});
      case 'staticfitness'
        methodOpts = parseOptionsStaticFitness(methodOpts{:});
      case 'staticpowerlaw'
        methodOpts = parseOptionsStaticPowerLaw(methodOpts{:});
      case 'forestfire'
        methodOpts = parseOptionsForestFire(methodOpts{:});
      case 'growingrandom'
        methodOpts = parseOptionsGrowingRandom(methodOpts{:});
      case {'callawaytraits', 'establishment'}
        methodOpts = parseOptionsCallawayTraits(methodOpts{:});
      case 'preference'
        methodOpts = parseOptionsPreference(methodOpts{:});
      case 'asymmetricpreference'
        methodOpts = parseOptionsAsymmetricPreference(methodOpts{:});
      case 'recentdegree'
        methodOpts = parseOptionsRecentDegree(methodOpts{:});
      case 'barabasiaging'
        methodOpts = parseOptionsBarabasiAging(methodOpts{:});
      case 'recentdegreeaging'
        methodOpts = parseOptionsRecentDegreeAging(methodOpts{:});
      case 'lastcit'
        methodOpts = parseOptionsLastCit(methodOpts{:});
      case 'citedtype'
        methodOpts = parseOptionsCitedType(methodOpts{:});
      case 'citingcitedtype'
        methodOpts = parseOptionsCitingCitedType(methodOpts{:});
      case 'sbm'
        methodOpts = parseOptionsSBM(methodOpts{:});
      case 'hsbm'
        methodOpts = parseOptionsHSBM(methodOpts{:});
      case 'dotproduct'
        methodOpts = parseOptionsDotProduct(methodOpts{:});
      case 'tree'
        methodOpts = parseOptionsTree(methodOpts{:});
      case 'simpleinterconnectedislands'
        methodOpts = ...
            parseOptionsSimpleInterconnectedIslands(methodOpts{:});
      otherwise
        error("The method ""%s"" has not been implemented.\n\n" + ...
              "Please report an issue or submit a pull request to github.", ...
              method);
    end

    if igutils.isoptionset(methodOpts, "multiple") && ...
            methodOpts.multiple && ~strcmp(graphOpts.repr, 'graph')
        throwAsCaller(MException("igraph:invalidOption", ...
                                 "Adjacency matrices cannot express " + ...
                                 "multigraphs, use 'graph' as " + ...
                                 "representation or set multiple to false."));
    end

    graph = mexIgraphDispatcher(mfilename(), method, graphOpts, methodOpts);
end

function opts = parseOptionsGrg(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.radius (1, 1) {mustBeNonnegative} = 5;
        opts.torus (1, 1) logical = false;
    end
end

function opts = parseOptionsBarabasi(method, opts)
    arguments
        method;
        opts.isdirected (1, 1) logical;
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger};
        opts.power (1, 1) {mustBeNonnegative} = 1;
        opts.nConnections (1, :) {mustBeNonnegative, mustBeInteger} = 1;
        opts.outPreference (1, 1) logical = false;
        opts.attractiveness (1, 1) {mustBeNumeric} = 1;
        opts.startFrom {igutils.mustBeGraph} = [];
    end

    if igutils.isoptionset(opts, "nNodes")
        if (length(opts.nConnections) > 1) && ...
           ((length(opts.nConnections)) ~= opts.nNodes)
            eid = "igraph:invalidVectorLength";
            msg = "If out sequence is not a scalar it must have " + ...
                  "length equal to the number of nodes.";
           throwAsCaller(MException(eid, msg));
        end
    else
        if length(opts.nConnections) == 1
            opts.nNodes = 10;
        else
            opts.nNodes = length(opts.nConnections);
        end
    end

    if method == "barabasibag" && opts.power ~= 1
        eid = "igraph:badPower";
        throwAsCaller(MException(eid, ...
                                 "Power must be 1 when using BarabasiBag."));
    end

    if method == "barabasibag" && opts.attractiveness ~= 1
        eid = "igraph:badAttractiveness";
        throwAsCaller(MException(eid, "Attractiveness must be 1 when " + ...
                                  "using BarabasiBag."));
    end

    if (~opts.outPreference) && (opts.attractiveness <= 0)
        eid = "igraph:badAttractiveness";
        throwAsCaller(MException(eid, "Attractiveness must be positive " + ...
                                 "when out preference is false."))
    end

    if ~igutils.isoptionset(opts, "isdirected")
       if isempty(opts.startFrom)
           opts.isdirected = false;
       else
           opts.isdirected = igraph.isdirected(opts.startFrom);
       end
    end
end

function opts = parseOptionsErdosRenyi(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.nEdges (1, 1) {mustBeNonnegative, mustBeInteger};
        opts.probability (1, 1) ...
            {mustBeNonnegative, mustBeLessThanOrEqual(opts.probability, 1)};
        opts.isdirected (1, 1) logical = false;
        opts.loops (1, 1) logical = false;
    end

    isoptionset = @igutils.isoptionset;
    if (~isoptionset(opts, "nEdges")) && (~isoptionset(opts, "probability"))
        opts.nEdges = 10;
    elseif (isoptionset(opts, "nEdges") && (isoptionset(opts, "probability")))
        throwAsCaller(MException("igraph:badOptions", ...
                                 "Cannot set both probability and " + ...
                                 "number of edges."));
    end
end

function opts = parseOptionsWattsStrogatz(opts)
    arguments
        opts.dim (1, 1) {mustBeNonnegative, mustBeInteger} = 2;
        opts.size (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
        opts.radius (1, 1) {mustBeNonnegative, mustBeInteger} = 1;
        opts.probability (1, 1) ...
            {mustBeNonnegative, ...
             mustBeLessThanOrEqual(opts.probability, 1)} = 0.2;
        opts.loops (1, 1) logical = false;
        opts.multiple (1, 1) logical = false;
    end
end

function opts = parseOptionsDegSeq(opts)
    arguments
        opts.degree (1, :) {mustBeNonnegative, mustBeInteger};
        opts.outDegree (1, :) {mustBeNonnegative, mustBeInteger};
        opts.inDegree (1, :) {mustBeNonnegative, mustBeInteger};
    end

    isoptionset = @igutils.isoptionset;
    if isoptionset(opts, "degree") && ...
            (isoptionset(opts, "outDegree") || isoptionset(opts, "inDegree"))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "degree should be set for undirected " + ...
                                 "graphs. Both inDegree and outDegree " + ...
                                 "should be set for directed graphs. " + ...
                                 "Cannot set both degree and outDegree " + ...
                                 "or inDegree."));
    end

    if (isoptionset(opts, "outDegree") && ~isoptionset(opts, "inDegree")) || ...
            (isoptionset(opts, "inDegree") && ~isoptionset(opts, "outDegree"))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "degree should be set for undirected " + ...
                                 "graphs. Both inDegree and outDegree " + ...
                                 "should be set for directed graphs. " + ...
                                 "If outDegree is set inDegree must " + ...
                                 "also be set."));
    end

    if isoptionset(opts, "outDegree")
        if length(opts.outDegree) ~= length(opts.inDegree)
            throwAsCaller(MException("igraph:badArgument", ...
                                     "outDegree and inDegree must have " + ...
                                     "the same length."))
        end
    end

    if ~isoptionset(opts, "inDegree")
        opts.inDegree = [];
    end

    % opts.degree just for user interface. Internally use outDegree like igraph.
    if isoptionset(opts, "degree")
        opts.outDegree = opts.degree;
    end

    if ~(isoptionset(opts, "degree") || isoptionset(opts, "outDegree"))
        opts.outDegree = ones(1, 10) * 3;
    end
end

function opts = parseOptionsKRegular(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.degree (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.isdirected (1, 1) logical = false;
        opts.multiple (1, 1) logical = false;
    end
end

function opts = parseOptionsStaticFitness(opts)
    arguments
        opts.fitness (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.outFitness (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.inFitness (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.nEdges (1, 1) {mustBeInteger, mustBePositive} = 20;
        opts.loops (1, 1) logical = false;
        opts.multiple (1, 1) logical = false;
    end

    isoptionset = @igutils.isoptionset;
    if isoptionset(opts, "fitness") && ...
            (isoptionset(opts, "outFitness") || isoptionset(opts, "inFitness"))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "fitness should be set for undirected " + ...
                                 "graphs. Both inFitness and outFitness " + ...
                                 "should be set for directed graphs. " + ...
                                 "Cannot set both fitness and outFitness " + ...
                                 "or inFitness."));
    end

    if (isoptionset(opts, "outFitness") && ~isoptionset(opts, "inFitness")) || ...
            (isoptionset(opts, "inFitness") && ~isoptionset(opts, "outFitness"))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "fitness should be set for undirected " + ...
                                 "graphs. Both inFitness and outFitness " + ...
                                 "should be set for directed graphs. " + ...
                                 "If outFitness is set inFitness must " + ...
                                 "also be set."));
    end

    if isoptionset(opts, "outFitness")
        if length(opts.outFitness) ~= length(opts.inFitness)
            throwAsCaller(MException("igraph:badArgument", ...
                                     "outFitness and inFitness must have " + ...
                                     "the same length."))
        end
    end

    if ~isoptionset(opts, "inFitness")
        opts.inFitness = [];
    end

    % opts.fitness just for user interface. Internally use outFitness like
    % igraph.
    if isoptionset(opts, "fitness")
        opts.outFitness = opts.fitness;
    end

    if ~isoptionset(opts, "outFitness")
        opts.outFitness = ones(1, 10) * 3;
    end
end

function opts = parseOptionsStaticPowerLaw(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive} = 10;
        opts.nEdges (1, 1) {mustBeInteger, mustBePositive} = 20;
        opts.exponent (1, 1) {mustBeNumeric};
        opts.outExponent (1, 1) {mustBeNumeric};
        opts.inExponent (1, 1) {mustBeNumeric};
        opts.loops (1, 1) logical = false;
        opts.multiple (1, 1) logical = false;
        opts.finiteSizeCorrection (1, 1) logical = false;
    end

    isoptionset = @igutils.isoptionset;
    if isoptionset(opts, "exponent") && ...
            (isoptionset(opts, "outExponent") || ...
             isoptionset(opts, "inExponent"))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "exponent should be set for undirected " + ...
                                 "graphs. Both inExponent and outExponent " + ...
                                 "should be set for directed graphs. " + ...
                                 "Cannot set both exponent and " + ...
                                 "outExponent or inExponent."));
    end

    if (isoptionset(opts, "outExponent") && ...
        ~isoptionset(opts, "inExponent")) || ...
        (isoptionset(opts, "inExponent") && ...
         ~isoptionset(opts, "outExponent"))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "exponent should be set for undirected " + ...
                                 "graphs. Both inExponent and outExponent " + ...
                                 "should be set for directed graphs. " + ...
                                 "If outExponent is set inExponent must " + ...
                                 "also be set."));
    end

    if ~isoptionset(opts, "inExponent")
        opts.inExponent = -1;
    end

    % opts.exponent just for user interface. Internally use outExponent like
    % igraph.
    if isoptionset(opts, "exponent")
        opts.outExponent = opts.exponent;
    end

    if ~isoptionset(opts, "outExponent")
        opts.outExponent = 2;
    end

    if opts.inExponent >= 0 && opts.inExponent < 2
        throwAsCaller(MException("igraph:badValue", ...
                                 "If inExponent is set it must be at least 2."));

    end

    if opts.outExponent >= 0 && opts.outExponent < 2
        throwAsCaller(MException("igraph:badValue", ...
                                 "exponent or outExponent must be at least 2."));

    end

end

function opts = parseOptionsForestFire(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.forwardProbability (1, 1) ...
            {mustBeInRange(opts.forwardProbability, 0, 1)} = 0.5;
        opts.backwardFactor (1, 1) {mustBeNonnegative, mustBeNumeric} = 1;
        opts.nAmbassadors (1, 1) {mustBeInteger, mustBePositive} = 3;
        opts.isdirected (1, 1) logical = false;
    end
end

function opts = parseOptionsGrowingRandom(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive} = 10;
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBePositive} = 4;
        opts.isdirected (1, 1) logical = false;
        opts.citation (1, 1) logical = false;
    end
end

function opts = parseOptionsCallawayTraits(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBeNonnegative} = 10;
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBePositive} = 4;
        opts.nTypes (1, :) {mustBeInteger, mustBePositive};
        opts.typeDistribution (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.preference (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.mixingParam (1, 1) {mustBeInRange(opts.mixingParam, 0, 1)};
        opts.isdirected (1, 1) logical;
    end

    isoptionset = @igutils.isoptionset;
    if ~isoptionset(opts, "nTypes")
        if ~(isoptionset(opts, "typeDistribution") || ...
             isoptionset(opts, "preference"))
            opts.nTypes = 4;
        elseif isoptionset(opts, "typeDistribution")
            opts.nTypes = length(opts.typeDistribution);
        else
            opts.nTypes = length(opts.preference);
        end
    end

    if ~isoptionset(opts, "typeDistribution")
       opts.typeDistribution = ones(1, opts.nTypes) / opts.nTypes;
    end

    if isoptionset(opts, "preference") && isoptionset(opts, "mixingParam")
        throwAsCaller(MException("igraph:overConstrained", ...
                                 'Arguments "preference" and ' + ...
                                 '"mixingParam" cannot both be set.'));
    end

    if isoptionset(opts, "mixingParam")
        opts.preference = generatePreference(opts.nTypes, opts.mixingParam);
    end

    if ~isoptionset(opts, "preference")
       opts.preference = ones(opts.nTypes, opts.nTypes)  / opts.nTypes;
    end

    if length(opts.typeDistribution) ~= opts.nTypes
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Type distribution must be of length nTypes"));
    end

    if length(opts.preference) ~= opts.nTypes || ...
            (size(opts.preference, 1) ~= size(opts.preference, 2))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Preference must be a square matrix " + ...
                                 "with sides of length nTypes"));
    end

    if ~isoptionset(opts, "isdirected")
        opts.isdirected = ~issymmetric(opts.preference);
    end

    if ~opts.isdirected && ~issymmetric(opts.preference)
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Preference matrix must be symmetric " + ...
                                 "if graph is undirected"));
    end
end

function opts = parseOptionsPreference(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive};
        opts.nTypes (1, 1) {mustBeInteger, mustBePositive};
        opts.typeDistribution (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.blockSizes (1, :) {mustBeNonnegative, mustBeNumeric};
        opts.preference (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.mixingParam (1, 1) {mustBeInRange(opts.mixingParam, 0, 1)};
        opts.isdirected (1, 1) logical;
        opts.loops (1, 1) logical = false;
    end

    isoptionset = @igutils.isoptionset;
    if isoptionset(opts, "typeDistribution") && isoptionset(opts, "blockSizes")
        throwAsCaller(MException("igraph:overConstrained", ...
                                 "typeDistribution and blockSizes " + ...
                                 "cannot both be set."));
    end

    opts.fixedSizes = false;
    if isoptionset(opts, "blockSizes")
        if isoptionset(opts, "nNodes") && ...
                ~(opts.nNodes == sum(opts.blockSizes))
            throwAsCaller(MException("igraph:overConstrained", ...
                                     "nTypes must be equal to the sum of " + ...
                                     "blockSizes. Leave nTypes blank " + ...
                                     "to calculate based on blockSizes."));
        else
            opts.nNodes = sum(opts.blockSizes);
        end
        opts.fixedSizes = true;
        opts.typeDistribution = opts.blockSizes;
    end

    if ~isoptionset(opts, "nNodes")
        opts.nNodes = 10;
    end

    if ~isoptionset(opts, "nTypes")
        if ~(isoptionset(opts, "typeDistribution") || ...
             isoptionset(opts, "preference"))
            opts.nTypes = 4;
        elseif isoptionset(opts, "typeDistribution")
            opts.nTypes = length(opts.typeDistribution);
        else
            opts.nTypes = length(opts.preference);
        end
    end

    if ~isoptionset(opts, "typeDistribution")
       opts.typeDistribution = ones(1, opts.nTypes) / opts.nTypes;
    end

    if isoptionset(opts, "preference") && isoptionset(opts, "mixingParam")
        throwAsCaller(MException("igraph:overConstrained", ...
                                 'Arguments "preference" and ' + ...
                                 '"mixingParam" cannot both be set.'));
    end

    if isoptionset(opts, "mixingParam")
        opts.preference = generatePreference(opts.nTypes, opts.mixingParam);
    end

    if ~isoptionset(opts, "preference")
       opts.preference = ones(opts.nTypes, opts.nTypes)  / opts.nTypes;
    end

    if length(opts.typeDistribution) ~= opts.nTypes
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Type distribution must be of length nTypes"));
    end

    if length(opts.preference) ~= opts.nTypes || ...
            (size(opts.preference, 1) ~= size(opts.preference, 2))
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Preference must be a square matrix " + ...
                                 "with sides of length nTypes"));
    end

    if ~isoptionset(opts, "isdirected")
        opts.isdirected = ~issymmetric(opts.preference);
    end

    if ~opts.isdirected && ~issymmetric(opts.preference)
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Preference matrix must be symmetric " + ...
                                 "if graph is undirected"));
    end
end

function opts = parseOptionsAsymmetricPreference(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBePositive} = 10;
        opts.nOutTypes (1, 1) {mustBeInteger, mustBePositive};
        opts.nInTypes (1, 1) {mustBeInteger, mustBePositive};
        opts.typeDistribution (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.preference (:, :) {mustBeNonnegative, mustBeNumeric};
        opts.loops (1, 1) logical = false;
    end

    isoptionset = @igutils.isoptionset;
    if ~isoptionset(opts, "nOutTypes")
        if ~(isoptionset(opts, "typeDistribution") || ...
             isoptionset(opts, "preference"))
            opts.nOutTypes = 4;
        elseif isoptionset(opts, "typeDistribution")
            opts.nOutTypes = size(opts.typeDistribution, 1);
        else
            opts.nOutTypes = size(opts.preference, 1);
        end
    end

    if ~isoptionset(opts, "nInTypes")
        if ~(isoptionset(opts, "typeDistribution") || ...
             isoptionset(opts, "preference"))
            opts.nInTypes = 4;
        elseif isoptionset(opts, "typeDistribution")
            opts.nInTypes = size(opts.typeDistribution, 2);
        else
            opts.nInTypes = size(opts.preference, 2);
        end
    end

    if ~isoptionset(opts, "typeDistribution")
        opts.typeDistribution = ones(opts.nOutTypes, opts.nInTypes) / ...
            (opts.nOutTypes * opts.nInTypes);
    end

    if ~isoptionset(opts, "preference")
        opts.preference = ones(opts.nOutTypes, opts.nInTypes)  / ...
            (opts.nOutTypes * opts.nInTypes);
    end

    if (size(opts.typeDistribution, 1) ~= opts.nOutTypes) || ...
            (size(opts.typeDistribution, 2) ~= opts.nInTypes)
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Type distribution must be a matrix " + ...
                                 "of size nOutTypes x nInTypes"));
    end

    if (size(opts.preference, 1) ~= opts.nOutTypes) || ...
            (size(opts.preference, 2) ~= opts.nInTypes)
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Preference must be a matrix of size " + ...
                                 "nOutTypes x nInTypes"));
    end
end

function opts = parseOptionsRecentDegree(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger};
        opts.degreeExp (1, 1) {mustBeNumeric} = 1;
        opts.timeWindow (1, 1) {mustBePositive, mustBeInteger} = 3;
        opts.edgesPerStep (1, :) {mustBeInteger, mustBePositive} = 3;
        opts.outPreference (1, 1) logical = false;
        opts.zeroAppeal (1, 1) {mustBeNonnegative, ...
                                mustBeLessThan(opts.zeroAppeal, 1)} = 0.1;
        opts.isdirected (1, 1) logical = false;
    end

    if ~igutils.isoptionset(opts, "nNodes")
        if length(opts.edgesPerStep) > 1
            opts.nNodes = length(opts.edgesPerStep);
        else
            opts.nNodes = 10;
        end
    end

    if length(opts.edgesPerStep) > 1 && ...
            length(opts.edgesPerStep) ~= opts.nNodes
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Edges Per time step must be a scalar " + ...
                                 "or a vector of length nNodes."));
    end

    if length(opts.edgesPerStep) > 1
        opts.outSeq = opts.edgesPerStep;
        opts.edgesPerStep = 1; % Ignored when outSeq not empty.
    else
        opts.outSeq = [];
    end
end

function opts = parseOptionsBarabasiAging(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.edgesPerStep (1, :) {mustBeNonnegative, mustBeInteger} = 3;
        opts.outPreference (1, 1) logical = false;
        opts.zeroDegreeAppeal (1, 1) {mustBeNumeric} = 0.1;
        opts.degreeCoef (1, 1) {mustBeNumeric} = 1;
        opts.degreeExp (1, 1) {mustBeNumeric} = 1;
        opts.zeroAgeAppeal (1, 1) {mustBeNumeric} = 0.1;
        opts.ageCoef (1, 1) {mustBeNumeric} = 1;
        opts.ageExp (1, 1) {mustBeNumeric} = 1;
        opts.agingBins (1, 1) {mustBePositive, mustBeInteger} = 1;
        opts.isdirected (1, 1) logical = false;
    end

    if length(opts.edgesPerStep) > 1
        opts.outSeq = opts.edgesPerStep;
        opts.edgesPerStep = 1;
    else
        opts.outSeq = [];
    end
end

function opts = parseOptionsRecentDegreeAging(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.edgesPerStep (1, :) {mustBeNonnegative, mustBeInteger} = 3;
        opts.outPreference (1, 1) logical = false;
        opts.zeroAppeal (1, 1) {mustBeNonnegative, mustBeNumeric} = 0.1;
        opts.degreeExp (1, 1) {mustBeNumeric} = 1;
        opts.ageExp (1, 1) {mustBeNumeric} = 1;
        opts.agingBins (1, 1) {mustBePositive, mustBeInteger} = 3;
        opts.timeWindow (1, 1) {mustBePositive, mustBeInteger} = 3;
        opts.isdirected (1, 1) logical = false;
    end

    if length(opts.edgesPerStep) > 1
        opts.outSeq = opts.edgesPerStep;
        opts.edgesPerStep = 1;
    else
        opts.outSeq = [];
    end
end

function opts = parseOptionsLastCit(opts)
    arguments
        opts.nNodes (1, 1) {mustBeInteger, mustBeNonnegative} = 10;
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBeNonnegative} = 3;
        opts.agingBins (1, 1) {mustBeInteger, mustBePositive};
        opts.preference (1, :) {mustBeInteger, mustBeNonnegative};
        opts.isdirected (1, 1) logical = false;
    end

    if ~igutils.isoptionset(opts, "agingBins")
        opts.agingBins = opts.nNodes;
    end

    if ~igutils.isoptionset(opts, "preference")
        opts.preference = ones(1, opts.agingBins + 1);
    end

    if length(opts.preference) ~= opts.agingBins + 1
        throwAsCaller(MException("igraph:wrongSize", ...
                                 "preference must be length agingBins + 1."));
    end
end

function opts = parseOptionsCitedType(opts)
    arguments
        opts.edgesPerStep (1, 1) {mustBeInteger, mustBeNonnegative} = 3;
        opts.types (1, :) {mustBeInteger, mustBePositive} = 1:10;
        opts.preference (1, :) {mustBeNonnegative};
        opts.isdirected (1, 1) logical = false;
    end

    opts.nNodes = length(opts.types);

    if ~igutils.isoptionset(opts, "preference")
        opts.preference = ones(1, max(opts.types));
    end

    if length(opts.preference) ~= max(opts.types)
        throwAsCaller(MException("igraph:wrongSize", ...
                                 "Size of preference must be the number " + ...
                                 "of types (i.e. max(types))."));
    end
end

function opts = parseOptionsCitingCitedType(opts)
    arguments
        opts.edgesPerStep (1, 1) {mustBeNonnegative, mustBeInteger} = 3;
        opts.types (1, :) {mustBePositive, mustBeInteger} = 1:10;
        opts.preference (:, :) {mustBeNumeric, mustBeSquare};
        opts.isdirected (1, 1) logical = false;
    end

    opts.nNodes = length(opts.types);

    if ~igutils.isoptionset(opts, "preference")
        opts.preference = ones(max(opts.types), max(opts.types));
    end

    if length(opts.preference) ~= max(opts.types)
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Preference must be square with side " + ...
                                 "lengths equal to the number of types " + ...
                                 "(i.e. max(types))."));
    end
end

function opts = parseOptionsSBM(opts)
    arguments
        opts.blockSizes (1, :) {mustBePositive, mustBeInteger} = 2 * ones(1, 5);
        opts.preference (:, :) ...
            {mustBeInRange(opts.preference, 0, 1), mustBeSquare};
        opts.isdirected (1, 1) logical = false;
        opts.loops (1, 1) logical = false;
    end

    if ~igutils.isoptionset(opts, "preference")
        opts.preference = ones(length(opts.blockSizes)) / ...
            (length(opts.blockSizes) ^ 2);
    end

    if length(opts.preference) ~= length(opts.blockSizes)
        throwAsCaller(MException("igraph:badArgument", ...
                                 "Preference must be square with side " + ...
                                 "lengths equal to the number of groups " + ...
                                 "(i.e. length(blockSizes))."));
    end

    opts.nNodes = sum(opts.blockSizes);
end

function opts = parseOptionsHSBM(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 20;
        opts.blockSizes (1, 1) {mustBeNonnegative, mustBeInteger} = 5;
        opts.rho (1, :) {mustBeInRange(opts.rho, 0, 1)} = [2 2 1] / 5;
        opts.preference (:, :) {mustBeNonNegative, mustBeSquare};
        opts.pOut (1, 1) {mustBeInRange(opts.pOut, 0, 1)} = 0.3;
    end

    if ~igutils.isoptionset(opts, "preference")
        opts.preference = ones(length(opts.rho));
    end
end

function opts = parseOptionsDotProduct(opts)
    arguments
        opts.positions (:, :) {mustBeNumeric} = ones(3, 10) / 3;
        opts.isdirected (1, 1) logical = false;
    end
end

function opts = parseOptionsTree(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.isdirected (1, 1) logical = false;
        opts.method (1, :) char;
    end
end

function opts = parseOptionsSimpleInterconnectedIslands(opts)
    arguments
        opts.nIslands (1, 1) {mustBeNonnegative, mustBeInteger} = 5;
        opts.islandSize (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
        opts.pIn (1, :) {mustBeInRange(opts.pIn, 0, 1)} = 0.75;
        opts.nInterEdges (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
    end
end

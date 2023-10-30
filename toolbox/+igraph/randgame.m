function adj = randgame(method, adjOptions, methodOptions)
%RANDGAME create an adjacency matrix with a stochastic game generator
%   ADJ = GAME(METHOD) use METHOD to generate a random adjacency matrix.
%       Results can be reproduced by setting the random generator using
%       IGRAPH.RNG. Each method has its own set of optional arguments, see
%       below for more information on a specific method. All resulting graphs
%       are unweighted. METHOD can be one of the following:
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
%   ADJ = GAME(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) in addition to
%       method specific parameters, there are parameters for specifying how the
%       resulting ADJ is represented common to all methods. These follow the
%       same conventions as the rest of matlab-igraph.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'makeSparse'   Whether to return a sparse (default) or full matrix.
%        'dtype'        The data type to use, either 'double' (default) or
%                       'logical'.
%
%       Note: The default values for most method options are chosen somewhat
%       randomly primarily for demonstration purposes.
%
%   ADJ = GAME('GeneratorRandomGraph', ...)
%   ADJ = GAME('grg', ...) Creates a graph by randomly placing points on a
%       square and connecting those within radius distance of each other.
%
%        Name       Description
%       --------------------------------------------------------------------
%        'nNodes'   Number of nodes (defaults to 10).
%        'radius'   How close two points need to be for an edge to form
%                   (default 0.1).
%        'torus'    Whether boundaries wrap (true) or not (false, default).
%
%   ADJ = GAME('BarabasiPsumTree', ...)
%   ADJ = GAME('BarabasiPsumTreeMultiple', ...)
%   ADJ = GAME('BarabasiBag', ...) Generate graphs using a Barabàsi Albert
%       model. Produces a graph where each node is initially connects to
%       nConnections other nodes. Nodes are added one at a time and the
%       probability a new node matches with another node is related to the
%       number of edges the other node has made. There are three variants of
%       this method. The Barabàsi Bag is the basic algorithm, the other two
%       methods use a prefix-sum tree, either with or without multiple edges.
%
%        Name             Description
%       --------------------------------------------------------------------
%        'nNodes'         Number of nodes (defaults to 10 or the length of
%                         NCONNECTIONS if that is a vector).
%        'power'          Power of preferential attachment. The larger power
%                         the stronger the preference for highly connected
%                         nodes (defaults to 1 and must be one for
%                         `BarabasiBag`).
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
%                         OUTPREFERENCE. Defaults to false or the directedness
%                         of STARTFROM.
%        'startFrom'      A graph to use as the starting point (a NCONNECTIONS
%                         clique by default).
%
%   ADJ = GAME('RenyiErdos', ...) Generate graphs with a fixed number of
%       edges or a constant probability of an edge existing. Either select the
%       number of edges or the probability any given edge exists. Cannot select
%       both NEDGES and PROBABILITY.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'nNodes'       Number of nodes (defaults to 10)
%        'nEdges'       Number of edges (defaults to 10)
%        'probability'  The probability a random edge exists (ignored by
%                       default)
%        'isdirected'   Whether the graph should be directed (true) or
%                       undirected (false, default).
%        'loops'        Whether to allow loops (true). Defaults to false.
%
%   ADJ = GAME('SmallWorld', ...)
%   ADJ = GAME('WattsStrogatz', ...) Uses the Watts-Strogatz model to generate
%       a graph with small-world properties. This method rewires a lattice
%       graph.
%
%       NOTE: Does not accept multiple logical since there is currently no
%       method to handle multiple edges using the adjacency matrix
%       representation.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'dim'          The dimension of the lattice (defaults to 2)
%        'size'         Length of each dimension (defaults to 4)
%        'radius'       Distance, in steps from node, to connect nodes
%                       (defaults to 1). Edges are made between nodes if they
%                       are at least this close on the grid.
%        'probability'  Probability of each edge is rewired (defaults to 0.2).
%        'loops'        Whether to allow loops (true). Defaults to false.
%
%   ADJ = GAME('DegSeqConfiguration', ...)
%   ADJ = GAME('DegSeqConfigurationSimple', ...)
%   ADJ = GAME('DegSeqFastHuerSimple', ...)
%   ADJ = GAME('DegSeqEdgeSwitchingSimple', ...)
%   ADJ = GAME('DegSeqVl', ...) Create a graph with the provided degree
%       sequence.
%
%       For creating directed graphs both INDEGREE and OUTDEGREE must be
%       specified and the same length. For undirected graphs, only DEGREE
%       should be specified.
%
%        Name         Description
%       --------------------------------------------------------------------
%        'degree'     The degree sequence for an undirected graph (default a
%                     vector of 10 3s).
%        'outDegree'  Degree sequence for edges leaving the nodes (no default).
%        'inDegree'   Degree sequence for edges entering the nodes (no default).
%
%   ADJ = GAME('kRegular', ...) Generate a graph where each node has the same
%       degree.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nNodes'      Number of nodes (default to 10).
%        'degree'      The degree of all nodes (default to 3).
%        'isdirected'  Whether graph should be directed (true) or undirected
%                      (false, default).
%
%   ADJ = GAME('StaticFitness', ...) Use node fitness scores to determine edge
%       probabilities.
%
%       When creating a directed graph, both INFITNESS and OUTFITNESS must be
%       specified and the same length. For undirected graphs use FITNESS.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nEdges'      The number of edges the graph should have (default 20).
%        'fitness'     A vector of fitness scores for each node, use for
%                      undirected graphs (default a vector of 10 3s).
%        'inFitness'   The out fitness vector for directed graphs (no default).
%        'outFitness'  The in fitness vector for directed graphs(no default).
%        'loops'       Whether to allow self-loop edges (default false).
%
%   ADJ = GAME('StaticPowerLaw', ...) Generate a graph a power-law node degree
%       distribution
%
%       When creating a directed graph, both INEXPONENT and OUTEXPONENT must be
%       specified. For undirected graphs use EXPONENT. Values must be greater
%       than or equal to 2.
%
%        Name                   Description
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
%   ADJ = GAME('ForestFire', ...) Create a graph with the forest fire model.
%
%        Name                 Description
%       --------------------------------------------------------------------
%        'nNodes'             The number of nodes the graph should have
%                             (default 10).
%        'forwardProbability' forward burning probability (default 0.5).
%        'backwardFactor'     backward factor (default 1).
%        'nAmbassadors'       Number of ambassador vertices (default 3).
%        'isdirected'         Whether graph should be directed (default false).
%
%   ADJ = GAME('GrowingRandom', ...) Grow a graph by randomly adding a nodes
%       one at a time.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'nNodes'       The number of nodes the graph should have (default 10).
%        'edgesPerStep' The number of edges to add each step (default 4).
%        'isdirected'   Whether graph should be directed (default false).
%        'citation'     Whether all new edges start from the most recent node
%                       (default false).
%
%   ADJ = GAME('CallawayTraits', ...)
%   ADJ = GAME('establishment', ...) Randomly add nodes of different types and
%        connect them based the preference for that type to connect to another.
%
%        For CALLAWAYTRAITS, EDGESPERSTEP new edges are made each time step.
%        For ESTABLISHMENT, EDGESPERSTEP new edges are tried. In CALLAWAYTRAITS
%        the tries are made until EDGESPERSTEP new edges whereas ESTABLISHMENT
%        only makes that many tries regardless of howmany new edges are
%        actually made. The success of an attempt is dependent on the node
%        types selected for the attempt and the the probability that edges are
%        made between those two types (PREFERENCE(i, j)).
%
%        Name               Description
%       --------------------------------------------------------------------
%        'nNodes'           The number of nodes the graph should have
%                           (default 10).
%        'edgesPerStep'     The number of edges to add each step (default 4).
%        'nTypes'           Number of unique node types (default 4 or length
%                           of TYPEDISTRIBUTION).
%        'typeDistribution' Distribution of node types (default uniform
%                           distribution of length NTYPES).
%        'preference'       NTYPES X NTYPES connection preference matrix
%                           (default uniform distribution).
%        'isdirected'       Whether graph should be directed (default false
%                           unless preference is specified and non-symmetric).
%
%   ADJ = GAME('preference', ...) Like CALLAWAYTRAITS and ESTABLISHMENT but all
%       nodes are generated first, then an attempt to make an edge is made for
%       all node pairs. Success is dependent on the preference matrix and the
%       type of each node pair.
%
%       Only one of TYPEDISTRIBUTION or BLOCKSIZES can be supplied.
%
%        Name               Description
%       --------------------------------------------------------------------
%        'nNodes'           The number of nodes the graph should have
%                           (default 10 or sum(BLOCKSIZES) if set).
%        'nTypes'           Number of unique node types (default 4 or length
%                           of TYPEDISTRIBUTION).
%        'typeDistribution' Distribution of node types (default uniform
%                           distribution of length NTYPES).
%        'blockSizes'       Number of nodes in each type (block) (no default).
%        'preference'       NTYPES X NTYPES connection preference matrix
%                           (default uniform distribution).
%        'isdirected'       Whether graph should be directed (default false
%                           unless preference is specified and non-symmetric).
%        'loops'            Whether to allow self-loop edges (default false).
%
%   ADJ = GAME('AsymmetricPreference', ...) Like preference but each node is
%       given two node types, an ingoing and outgoing.
%
%       Graph is always directed.
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
%                           distribution of size NOUTTYPES X NINTYPES).
%        'preference'       NOUTTYPES X NINTYPES connection preference matrix
%                           (default uniform distribution).
%        'loops'            Whether to allow self-loop edges (default false).
%
%   ADJ = GAME('RecentDegree', ...) Randomly add edges to nodes proportional to
%       the number of edges the the node has recently gained.
%
%        Name            Description
%       --------------------------------------------------------------------
%        'nNodes'        The number of nodes the graph should have
%                        (default 10 or length of EDGESPERSTEP if a vector).
%                        Used as the number of time steps in the simulation.
%        'degreeExp'     Probability a node gains a new edge is proportional
%                        to the number of edges it has recently gained to
%                        DEGREEEXP (default 1).
%        'timeWindow'    Include an edge as recent if it was created within
%                        TIMEWINDOW time steps of now (default 3). (Should
%                        be less than NNODES.)
%        'edgesPerStep'  How many edges to add in a time step. Either a vector
%                        of length NNODES or a scalar to use the same value
%                        for each time step (default 3).
%        'outPreference' Whether to count edges that started from a node
%                        (during it's time step). (default false.)
%        'zeroAppeal'    A baseline probability so nodes without recent edges
%                        don't have zero likelihood of gaining an edge
%                        (default 0.1).
%        'isdirected'    Whether graph should be directed (default false unless
%                        preference is specified and non-symmetric).
%
%   ADJ = GAME('BarabasiAging', ...) Similar to RECENTDEGREE but adds in an age
%       component such that older nodes are preferred. Unlike RECENTDEGREE all
%       edges are taken into consideration, not just recent edges. One node is
%       added every timestep.
%
%        Name               Description
%       --------------------------------------------------------------------
%        'nNodes'           The number of nodes the graph should have
%                           (default 10).
%        'edgesPerStep'     How many edges to add in a time step. Either a
%                           vector of length NNODES or a scalar to use the same
%                           value for each time step (default 3).
%        'outPreference'    Whether to count edges that started from a node
%                           (during it's time step). (default false.)
%        'zeroDegreeAppeal' (default 0.1).
%        'degreeCoef'       (default 1).
%        'degreeExp'        (default 1).
%                           The degree component of the probability a node gets
%                           an edge is ZERODEGREEAPPEAL + DEGREECOEF * degree ^
%                           DEGREEEXP
%        'zeroAgeAppeal'    (default 0.1).
%        'ageCoef'          (default 1).
%        'ageExp'           (default 1).
%                           The age component of the probability a node gets
%                           an edge is ZEROAGEAPPEAL + AGECOEF * age ^
%                           AGEEXP
%        'agingBins'        How many time steps represent 1 age. (If 3, a
%                           node gains 1 to it's age every three time steps.)
%                           (default 1).
%        'isdirected'       Whether graph should be directed (default false).
%
%   ADJ = GAME('RecentDegreeAging', ...) RECENTDEGREE with aging component.
%
%        Name            Description
%       --------------------------------------------------------------------
%        'nNodes'        The number of nodes the graph should have
%                        (default 10).
%        'edgesPerStep'  How many edges to add in a time step. Either a vector
%                        of length NNODES or a scalar to use the same value for
%                        each time step (default 3).
%        'outPreference' Whether to count edges that started from a node
%                        (during it's time step). (default false.)
%        'zeroAppeal'    The baseline probability a node gains an edge
%                        (default 0.1).
%        'degreeExp'     The degree component of the probability a node gets
%                        an edge is degree ^ DEGREEEXP (default 1).
%        'ageExp'        The age component of the probability a node gets
%                        an edge is age ^ AGEEXP
%        'agingBins'     How many time steps represent 1 age. (If 3, a
%                        node gains 1 to it's age every three time steps.)
%                        (default 1).
%        'timeWindow'    Include an edge as recent if it was created within
%                        TIMEWINDOW time steps of now (default 3). (Should
%                        be less than NNODES.)
%        'isdirected'    Whether graph should be directed (default false).
%
%   ADJ = GAME('LastCit', ...) Simulation that makes edges proportional to how
%       recently a node was last cited. Nodes are binned based on the last time
%       they were cited. Note: this is different than other methods which use
%       node creation as age.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'nNodes'       The number of nodes the graph should have (default 10).
%        'edgesPerStep' How many edges to add in a time step (default 3).
%                       (Must be an integer not a vector.)
%        'agingBins'    The number of age bins to use (default NNODES).
%        'preference'   A vector of length AGINGBINS + 1 with the preferences
%                       for each age bin (default ONES(1, AGINGBINS + 1)).
%        'isdirected'   Whether graph should be directed (default false).
%
%   ADJ = GAME('CitedType', ...) Create a network one node at a time and add
%       edges with other nodes with preference based on node types
%
%        Name           Description
%       --------------------------------------------------------------------
%        'edgesPerStep' How many edges to add in a time step (default 3).
%        'types'        Vector of node types (labeled from 1). The number of
%                       nodes in the resulting graph is length TYPES. (default
%                       1:10.)
%        'preference'   Vector of type preference of length MAX(TYPES).
%                       (default ONES(1, MAX(TYPES))).
%        'isdirected'   Whether graph should be directed (default false).
%
%   ADJ = GAME('CitingCitedType', ...) Generate a graph by adding edges with
%       probability given by the preference matrix and the types of both the
%       nodes being wired.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'edgesPerStep' How many edges to add in a time step (default 3).
%        'types'        Vector of node types (labeled from 1). The number of
%                       nodes in the resulting graph is length TYPES. (default
%                       1:10.)
%        'preference'   Matrix of type preference of size MAX(TYPES) X
%                       MAX(TYPES). (default ONES(MAX(TYPES), MAX(TYPES)).)
%        'isdirected'   Whether graph should be directed (default false).
%
%   ADJ = GAME('StochasticBlockModel', ...)
%   ADJ = GAME('sbm', ...) Create a block graph where edges are created with
%       probability dependent on which block each node is a member of.
%
%        Name         Description
%       --------------------------------------------------------------------
%        'blockSizes' A vector containing the sizes for each block. Number of
%                     nodes is equal to SUM(BLOCKSIZES). (default ONES(1, 5) *
%                     2)
%        'preference' A matrix of the probabilities for an edge between nodes
%                     of different blocks. Size should be K X K, where K is the
%                     number of blocks (LENGTH(BLOCKSIZES)). (default
%                     ONES(K, K) / (K ^ 2).)
%        'isdirected' Whether graph should be directed (default false).
%        'loops'      Whether to allow self-loop edges (default false).
%
%   ADJ = GAME('HierarchicalStochasticBlockMethod', ...)
%   ADJ = GAME('hsbm', ...) Generate a hierarchical graph with the
%       hierarchical stochastic block method.
%
%       NOTE: NNODES, BLOCKSIZES, and RHO are all dependent on each other, so
%       changing one default may require changing the others.
%
%        Name         Description
%       --------------------------------------------------------------------
%        'nNodes'     The number of nodes in the graph (default 20).
%        'blockSizes' The number of nodes in a block (default 5).
%        'rho'        Fraction of a block's node in each within block cluster.
%                     (default [2 2 1] / 5).
%        'preference' Matrix of size K X K, where K is the nmuber of within
%                     block clusters (LENGTH(RHO)), with the preference for a
%                     node from 1 cluster to connect to a node in another
%                     cluster.
%        'pOut'       Probability a node in one block connects with a node in
%                     a different block (default 0.3).
%
%   ADJ = GAME('DotProduct', ...) Create a graph where the probability an edge
%       exists is the dot product between the two nodes latent positions.
%
%        Name         Description
%       --------------------------------------------------------------------
%        'positions'  A matrix with nNodes columns. Each column represents
%                     the position of a node (default ONES(3, 10) / 3).
%        'isdirected' Whether graph should be directed (default false).
%
%   ADJ = GAME('TreePrufer', ...)
%   ADJ = GAME('TreeLERW', ...) Generate random tree graphs.
%
%       Graphs can be made with two methods, Prufer and loop-erased random
%       walk. For Prufer, ISDIRECTED must be false.
%
%        Name         Description
%       --------------------------------------------------------------------
%        'nNodes'     The number of nodes in the graph (default 10).
%        'isdirected' Whether graph should be directed (default false).
%
%   ADJ = GAME('SimpleInterConnectedIslands', ...) Create a network with
%       islands, where nodes within an island are more likely to connect than
%       nodes between an island.
%
%        Name          Description
%       --------------------------------------------------------------------
%        'nIslands'    The number of islands in the graph (default 5).
%        'islandSize'  Size of each island (default 4).
%        'pIn'         Within island edge probability (default 0.75).
%        'nInterEdges' The number of edges between two islands (default 4).
%
%   See also igraph.famous, igraph.generate, igraph.rewire, igraph.rng.

    arguments
        method (1, :) char ...
            {mustBeMemberi(method, ...
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
                            'CorrelatedPair', 'SimpleInterconnectedIslands'})}

        adjOptions.makeSparse (1, 1) logical = true;
        adjOptions.dtype (1, :) char ...
            {mustBeMemberi(adjOptions.dtype, {'logical', 'double'})} = 'double';

        methodOptions.nNodes;
        methodOptions.radius;
        methodOptions.torus;
        methodOptions.power;
        methodOptions.nConnections;
        methodOptions.outPreference;
        methodOptions.attractiveness;
        methodOptions.isdirected;
        methodOptions.startFrom;
        methodOptions.nEdges;
        methodOptions.loops;
        methodOptions.probability;
        methodOptions.dim;
        methodOptions.size;
        methodOptions.degree;
        methodOptions.outDegree;
        methodOptions.inDegree;
        methodOptions.fitness;
        methodOptions.outFitness;
        methodOptions.inFitness;
        methodOptions.outExponent;
        methodOptions.inExponent;
        methodOptions.finiteSizeCorrection;
        methodOptions.forwardProbability;
        methodOptions.backwardFactor;
        methodOptions.nAmbassadors;
        methodOptions.citation;
        methodOptions.nTypes;
        methodOptions.edgesPerStep;
        methodOptions.typeDistribution;
        methodOptions.nOutTypes;
        methodOptions.nInTypes;
        methodOptions.timeWindow;
        methodOptions.zeroAppeal;
        methodOptions.degreeExp;
        methodOptions.ageExp;
        methodOptions.agingBins;
        methodOptions.zeroDegreeAppeal;
        methodOptions.zeroAgeAppeal;
        methodOptions.degreeCoef;
        methodOptions.ageCoef;
        methodOptions.preference;
        methodOptions.types;
        methodOptions.blockSizes;
        methodOptions.clusterSizes;
        methodOptions.pOut;
        methodOptions.positions;
        methodOptions.nIslands;
        methodOptions.islandSize;
        methodOptions.pIn;
        methodOptions.nInterEdges;
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
        algo = method(length('tree')+1:end);
        method = 'tree';
    end

    adjOptions.dtype = lower(adjOptions.dtype);

    requiredArguments = namedargs2cell(adjOptions);
    methodOptions = namedargs2cell(methodOptions);
    generatorWrapper = @(varargin) ...
        mexIgraphRandGame(method, requiredArguments{2:2:end}, varargin{:});

    switch method
      case 'grg'
        methodOptions = parseOptionsGrg(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.radius, ...
                               methodOptions.torus);
      case {'barabasibag', 'barabasipsumtree', 'barabasipsumtreemultiple'}
        methodOptions = parseOptionsBarabasi(method, methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.power, ...
                               methodOptions.nConnections, ...
                               methodOptions.outPreference, ...
                               methodOptions.attractiveness, ...
                               methodOptions.isdirected, ...
                               methodOptions.startFrom);
      case 'erdosrenyi'
        methodOptions = parseOptionsErdosRenyi(methodOptions{:});
        if isoptionset(methodOptions, "probability")
            useEdges = false;
            edgeOrProb = methodOptions.probability;
        else
            useEdges = true;
            edgeOrProb = methodOptions.nEdges;
        end

        adj = generatorWrapper(useEdges, methodOptions.nNodes, edgeOrProb, ...
                               methodOptions.isdirected, methodOptions.loops);
      case 'wattsstrogatz'
        methodOptions = parseOptionsWattsStrogatz(methodOptions{:});
        adj = generatorWrapper(methodOptions.dim, methodOptions.size, ...
                               methodOptions.radius, ...
                               methodOptions.probability, methodOptions.loops);
      case {'degseqconfiguration', 'degseqconfigurationsimple', ...
            'degseqfastheursimple', 'degseqedgeswitchingsimple', 'degseqvl'}
        methodOptions = parseOptionsDegSeq(methodOptions{:});
        adj = generatorWrapper(methodOptions.outDegree, methodOptions.inDegree);
      case 'kregular'
        methodOptions = parseOptionsKRegular(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.degree, ...
                               methodOptions.isdirected);
      case 'staticfitness'
        methodOptions = parseOptionsStaticFitness(methodOptions{:});
        adj = generatorWrapper(methodOptions.nEdges, ...
                               methodOptions.outFitness, ...
                               methodOptions.inFitness, ...
                               methodOptions.loops);
      case 'staticpowerlaw'
        methodOptions = parseOptionsStaticPowerLaw(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.nEdges, ...
                               methodOptions.outExponent, ...
                               methodOptions.inExponent, ...
                               methodOptions.loops, ...
                               methodOptions.finiteSizeCorrection);
      case 'forestfire'
        methodOptions = parseOptionsForestFire(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.forwardProbability, ...
                               methodOptions.backwardFactor, ...
                               methodOptions.nAmbassadors, ...
                               methodOptions.isdirected);
      case 'growingrandom'
        methodOptions = parseOptionsGrowingRandom(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.isdirected, ...
                               methodOptions.citation);
      case {'callawaytraits', 'establishment'}
        methodOptions = parseOptionsCallawayTraits(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.nTypes, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.typeDistribution, ...
                               methodOptions.preference, ...
                               methodOptions.isdirected);
      case 'preference'
        methodOptions = parseOptionsPreference(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.nTypes, ...
                               methodOptions.typeDistribution, ...
                               methodOptions.fixedSizes, ...
                               methodOptions.preference, ...
                               methodOptions.isdirected, methodOptions.loops);
      case 'asymmetricpreference'
        methodOptions = parseOptionsAsymmetricPreference(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.nOutTypes, ...
                               methodOptions.nInTypes, ...
                               methodOptions.typeDistribution, ...
                               methodOptions.preference, ...
                               methodOptions.loops);
      case 'recentdegree'
        methodOptions = parseOptionsRecentDegree(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.degreeExp, ...
                               methodOptions.timeWindow, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.outSeq, ...
                               methodOptions.outPreference, ...
                               methodOptions.zeroAppeal, ...
                               methodOptions.isdirected);
      case 'barabasiaging'
        methodOptions = parseOptionsBarabasiAging(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.outSeq, ...
                               methodOptions.outPreference, ...
                               methodOptions.degreeExp, ...
                               methodOptions.ageExp, ...
                               methodOptions.agingBins, ...
                               methodOptions.zeroDegreeAppeal, ...
                               methodOptions.zeroAgeAppeal, ...
                               methodOptions.degreeCoef, ...
                               methodOptions.ageCoef, ...
                               methodOptions.isdirected);
      case 'recentdegreeaging'
        methodOptions = parseOptionsRecentDegreeAging(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.outSeq, ...
                               methodOptions.outPreference, ...
                               methodOptions.degreeExp, ...
                               methodOptions.ageExp, ...
                               methodOptions.agingBins, ...
                               methodOptions.timeWindow, ...
                               methodOptions.zeroAppeal, ...
                               methodOptions.isdirected);
      case 'lastcit'
        methodOptions = parseOptionsLastCit(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.agingBins, ...
                               methodOptions.preference, ...
                               methodOptions.isdirected);
      case 'citedtype'
        methodOptions = parseOptionsCitedType(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.types, ...
                               methodOptions.preference, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.isdirected);
      case 'citingcitedtype'
        methodOptions = parseOptionsCitingCitedType(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, methodOptions.types, ...
                               methodOptions.preference, ...
                               methodOptions.edgesPerStep, ...
                               methodOptions.isdirected);
      case 'sbm'
        methodOptions = parseOptionsSBM(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.preference, ...
                               methodOptions.blockSizes, ...
                               methodOptions.isdirected, ...
                               methodOptions.loops);
      case 'hsbm'
        methodOptions = parseOptionsHSBM(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.blockSizes, ...
                               methodOptions.rho, ...
                               methodOptions.preference, ...
                               methodOptions.pOut);
      case 'dotproduct'
        methodOptions = parseOptionsDotProduct(methodOptions{:});
        adj = generatorWrapper(methodOptions.positions, ...
                               methodOptions.isdirected);
      case 'tree'
        methodOptions = parseOptionsTree(methodOptions{:});
        adj = generatorWrapper(methodOptions.nNodes, ...
                               methodOptions.isdirected, algo);
      case 'simpleinterconnectedislands'
        methodOptions = ...
            parseOptionsSimpleInterconnectedIslands(methodOptions{:});
        adj = generatorWrapper(methodOptions.nIslands, ...
                               methodOptions.islandSize, ...
                               methodOptions.pIn, ...
                               methodOptions.nInterEdges);
      otherwise
        error("The method ""%s"" has not been implemented.\n\n" + ...
              "Please report an issue or submit a pull request to github.", ...
              method);
    end
end

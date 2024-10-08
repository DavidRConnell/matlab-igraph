function save(filename, graph, ioOpts, graphOpts)
%SAVE write a graph to file
%   SAVE(FILENAME, GRAPH) write the graph GRAPH to FILENAME. The storage format
%   used will be guessed by the file extension. WARNING: This will write over a
%   preexisting file with the same name.
%
%   SAVE(FILENAME, GRAPH, 'format', FORMAT) use the provided FORMAT to store
%   the graph instead of trying to determine the format from the file's
%   extension. WARNING: Not all formats that can be saved by igraph can be
%   read, compare format list with IGRAPH.LOAD's list. Additionally, file types
%   'ncol' and 'lgl' use names instead of IDs to track vertices. While it may
%   be useful to be able to read in these file types, given that this toolbox
%   does not have a method for storing names these are likely not the most
%   sensible file types for use within the toolbox. These will also drop
%   information about node order, so the results of saving then loading will be
%   a graph that is guaranteed to be isomorphic (see IGRAPH.ISOMORPHIC) to the
%   original graph but not necessarily equal based on ISEQUAL (nodes are likely
%   to be rearranged).
%
%   Available formats are:
%       Format      File Extension            Description
%   -------------------------------------------------------------------------
%      'mat'       '.mat'                    MAT-file (limited compatibility
%                                            outside of MATLAB)
%      'edgelist'  {'.txt', '.csv', 'edges'} edgelist (unweighted only)
%      'ncol'      '.ncol'                   Large Graph Layout named vertex
%                                            format (undirected only)
%      'lgl'       '.lgl'                    Large Graph Layout format
%                                            (undirected only)
%      'graphml'   '.graphml'                Graphml format
%      'gml'       '.gml'                    GML format
%      'pajek'     '.net'                    Pajek format
%      'dot'       {'.dot', '.gv'}           GraphViz DOT format.
%      'leda'      {'.gw', '.lgr'}           LEDA native format.
%
%   SAVE(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) use the name-value pairs
%   'isweighted' and 'isdirected' instead of guessing the values. By default,
%   these values are guessed using IGRAPH.ISWEIGHTED and IGRAPH.ISDIRECTED.
%   NOTE: These do not have any impact when saving as a MAT-file type.
%
%   WARNING: Overriding defaults may result in losing information about the
%   graph.
%
%   If 'isweighted' is set to false, the weights will not be saved even if the
%   graph is weighted. If 'isdirected' is set to false, the direction of edges
%   may be dropped (i.e. A(j, i) may be stored as A(i, j)). Otherwise, if
%   'isdirected' is set to true, edges A(i, j) and A(j, i) will be stored for
%   all i,j in adjacency matrix A, even if A is symmetric. The 'overwrite' flag
%   determines if SAVE should continue when a file already exists at the
%   provide path or error. By default SAVE will not write over a file. When
%   'overwrite' is set to true, write over a file already saved at FILENAME.
%
%   See also IGRAPH.LOAD, IGRAPH.CONVERT, IGRAPH.ISWEIGHTED, IGRAPH.ISDIRECTED.

    arguments
        filename char {mustBeVector}
        graph {igutils.mustBeGraph}
        ioOpts.format char {mustBeVector} = guessFileFormat(filename);
        ioOpts.overwrite (1, 1) logical = false;
        graphOpts.?igutils.GraphInProps;
    end

    graphOpts = namedargs2cell(graphOpts);
    graphOpts = igutils.setGraphInProps(graph, graphOpts{:});

    if ~ioOpts.overwrite && exist(filename, 'file')
        error("igraph:fileExists", "A file already exists at '%s'.\n\n" + ...
              "Change the 'overwrite' name-value pair to true to write " + ...
              "over the old file or choose a new filename.", ...
              filename);
    end

    switch ioOpts.format
      case 'mat'
        save(filename, 'graph');
        return
      case 'edgelist'
        mustBeUnweighted();
      case {'lgl', 'dimacs', 'ncol'}
        mustBeUndirected();
    end

    mexIgraphDispatcher(mfilename(), filename, graph, ioOpts.format, ...
                        graphOpts);

    function mustBeUndirected()
        if graphOpts.isdirected
            eid = "igraph:directed";
            msg = "The '%s' format can only store undirected graphs but " + ...
                  "the provided graph is directed.\n\nYou can override " + ...
                  "this error by setting the 'isdirected' name-value " + ...
                  "argument to false but this will lose information " + ...
                  "if the graph is directed.";
            throwAsCaller(MException(eid, msg, ioOpts.format));
        end
    end

    function mustBeUnweighted()
        if graphOpts.isweighted
            eid = "igraph:weighted";
            msg = "The '%s' format can only store unweighted graphs but " + ...
                  "the provided graph has values outside {0, 1}.\n\n " + ...
                  "You can override this error by setting the " + ...
                  "'isweighted' name-value argument to false, but the " + ...
                  "graph's weights will be not be saved.";
            throwAsCaller(MException(eid, msg, ioOpts.format));
        end
    end
end

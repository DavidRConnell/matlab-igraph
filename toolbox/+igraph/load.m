function adj = load(filename, ioOptions, adjOptions)
%LOAD read a graph from file
%   ADJ = LOAD(FILE) read FILE representing a graph into MATLAB. The file's
%       storage format will be guessed based on the file's extension.
%
%   ADJ = LOAD(FILE, 'FORMAT', FORMAT) use FORMAT to read the file instead of
%       guessing the value from the file extension. In some cases, it may read
%       in part of a file to differentiate between types with the same
%       extension.
%
%   Available formats are:
%      Format       File Extension          Description
%   -------------------------------------------------------------------------
%      'mat'        '.mat'                  MAT-file (limited compatibility
%                                           outside of MATLAB)
%      'edgelist'   {'.txt','.tsv','.csv'}  edgelist (unweighted only)
%      'ncol'       '.ncol'                 Large Graph Layout named vertex
%                                           format (names are replaced by ids
%                                           in range 1:nNodes, undirected only)
%      'lgl'        '.lgl'                  Large Graph Layout format
%                                           (undirected only)
%      'graphdb'    '.zip'                  Graph database format
%      'graphml'    '.graphml'              GraphML format
%      'gml'        '.gml'                  GML format
%      'pajek'      '.net'                  Pajek format
%      'dl'         {'.txt', '.dl'}         UCINET's DL format.
%
%   ADJ = LOAD(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) in addition to the
%   'FORMAT' the folliwing
%
%       Parameter        Value
%        'isweighted'*   Whether to load weights if they exist or not. If no
%                        weights exist in the file, there is no harm in leaving
%                        this as true. By default this is true unless data type
%                        ('dtype') is 'logical', in which case it will be set
%                        to false. If false, ignore weights even if they exist
%                        (also causes data type, to default to 'logical'). If
%                        explicitly true it will error if data type is set to
%                        'logical'.
%
%        'isdirected'*   Whether to treat the adjacency matrix as directed or
%                        undirected. Like ISWEIGHTED, there is no harm in
%                        leaving this true (default) when the graph is
%                        undirected. Setting to false causes the resulting
%                        adjacency matrix to be returned as just the lower
%                        triangle to reduce redundancy. If the resulting
%                        adjacency matrix appears to be directed (as determined
%                        by IGRAPH.ISDIRECTED) and ISDIRECTED is set to false,
%                        a warning will be displayed since edges could be
%                        combined (A(i, j) = A(i, j) + A(j, i)) and this is
%                        likely undesirable. NOTE: some storage methods reorder
%                        nodes, this will not change the information but may
%                        cause the loaded adjacency matrix to be the transpose
%                        of the saved adjacency matrix in the undirected case.
%                        This *should* not impact other igraph calculations. To
%                        prevent this, if ISDIRECTED is left unset,
%                        IGRAPH.ISDIRECTED will be used after loading the ADJ
%                        and the lower triangle will be return if it appears to
%                        be undirected.
%
%        'dtype'         Set the returned ADJ's datatype. By default, the data
%                        type will be determined by the value of ISWEIGHTED
%                        ('logical' if false, 'double' otherwise). Allowed data
%                        types are 'double' and 'logical'. If DTYPE is set to
%                        'logical' and ISWEIGHTED is explicitly set to true,
%                        this will cause an error since a logical adjacency
%                        matrix cannot store weights.
%
%        'makeSparse'    Whether the resulting ADJ should use a full matrix
%                        representation (false) or a sparse representation
%                        (true, default). This should have little impact on
%                        performance since the performance bottlenecks usually
%                        occur after converting to an internal igraph type,
%                        which is independent of MATLAB's representation. The
%                        impact on memory is again limited by the need to
%                        convert to an internal igraph type, but it may be
%                        important if there are multiple adjacency matrices
%                        stored in memory at once.
%
%         'index'        For formats that can store multiple graphs (graphml),
%                        select which graph to read (defaults to 0).
%
%   *NOTE: there is no difference in the representation of a "weighted"
%   adjacency matrix whose edge weights are all 0 or 1 and an unweighted
%   adjacency matrix. Similarly, there is no difference in the representation
%   of a directed and undirected adjacency matrix. These options only impact
%   assumptions made while reading the files.
%
%   See also IGRAPH.SAVE, IGRAPH.ISDIRECTED.

    arguments
        filename char {mustBeVector}
        ioOptions.format char {mustBeVector} = guessFileFormat(filename);
        ioOptions.index (1, 1) double {mustBeInteger, mustBeNonnegative} = 0;
        adjOptions.isweighted (1, 1) logical
        adjOptions.isdirected (1, 1) logical;
        adjOptions.dtype char {mustBeMember(adjOptions.dtype, ...
                                            {'double', 'logical'})};
        adjOptions.makeSparse (1, 1) logical = true;
    end

    if ~strcmp(ioOptions.format, 'graphml') && ioOptions.index ~= 0
        warning("Index option set but file format %s does not store " + ...
                "multiple graphs, ignoring index.", ioOptions.format);
    end

    if strcmp(ioOptions.format, 'mat')
        out = load(filename);
        adj = out.adj;
        return
    end

    if ~isoptionset(adjOptions, 'dtype')
        if isoptionset(adjOptions, 'isweighted') && ~adjOptions.isweighted
            adjOptions.dtype = 'logical';
        else
            adjOptions.dtype = 'double';
        end
    end

    if ~isoptionset(adjOptions, 'isweighted')
        if strcmp(adjOptions.dtype, 'logical')
            adjOptions.isweighted = false;
        else
            % Assuming a truly unweighted adj is weighted does not alter the
            % representation, so it's always safe to assume it's weighted.
            adjOptions.isweighted = true;
        end
    end

    if strcmp(adjOptions.dtype, 'logical') && adjOptions.isweighted
        error("Igraph:badDataType", "Cannot use a logical adjacency " + ...
                   "matrix to represent weighted graph.");
    end

    userSetDirectedness = true;
    if ~isoptionset(adjOptions, 'isdirected')
       adjOptions.isdirected = true;
       userSetDirectedness = false;
    end

    adj = mexIgraphDispatcher(mfilename(), filename, ioOptions, adjOptions);

    if ~userSetDirectedness
        adjOptions.isdirected = igraph.isdirected(adj);
    end

    if ~adjOptions.isweighted && ~islogical(adj)
        adj = double(adj ~= 0);
    end

    if ~adjOptions.isdirected
        if igraph.isdirected(adj)
            warning("Forcing a non-triangular, asymmetric adjacency " + ...
                    "matrix to be undirected. Summing edges A(i, j) " + ...
                    "and A(j, i) for all i and j.")
        elseif issymmetric(adj)
            adj = tril(adj);
        elseif istriu(adj)
            % For undirected graphs, igraph may flip the edges (see
            % igraph_edge()), causing a lower triangle to be written as an
            % upper and vice-versa. So stick to a standard of always using
            % lower triangles to prevent unexpected behavior.
            adj = adj';
        end

        if islogical(adj)
            adj = tril(adj) | triu(adj, 1)';
        else
            adj = tril(adj) + triu(adj, 1)';
        end
    end
end

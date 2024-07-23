function args = setGraphInProps(graph, args)
%SETGRAPHINPROPS set the default values for unassigned graph in options
%   ARGS = SETGRAPHINPROPS(GRAPH, ARGS) Use GRAPH to determine defaults to use
%   for any unset options. In the case ARGS came from a argument block, it will
%   be in the form of a structure. To pass this to a function convert it to a
%   cell with NAMEDARGS2CELL then call with ARGS{:}.
%
%   Options for output graphs (IGUTILS.GRAPHOUTPROPS) can also be passed in and
%   will be returned untouched. This allows a single graph option set to be
%   used for both creation of the input graph and the returned graph.
%
%   See also IGUTILS.GRAPHINPROPS, IGUTILS.SETGRAPHOUTPROPS.

    arguments
        graph
        args.?igutils.GraphOutProps
        args.isweighted
        args.isdirected
    end

    isoptionset = @igutils.isoptionset;
    if ~isoptionset(args, 'isdirected')
        args.isdirected = igraph.isdirected(graph);
    end

    if ~isoptionset(args, 'isweighted')
        args.isweighted = igraph.isweighted(graph);
    end

    if args.isweighted && isoptionset(args, 'weight')
        if ~igutils.isgraph(graph)
            warning("Option 'weight' set but is only used for 'graph' " + ...
                    "representations, the value will be ignored.");
        elseif ~igutils.hasEdgeAttr(graph, args.weight)
            edgeAttrs = igutils.listEdgeAttr(graph);
            throwAsCaller(MException("igraph:invalidWeight", ...
                                     "The requested edge attribute ('%s') " + ...
                                     "to use for weights was not " + ...
                                     "found in graph. (Note names are " + ...
                                     "case sensitive.)\n\n" + ...
                                     "Available edge attributes are:\n%s", ...
                                     args.weight, ...
                                     strjoin("    " + edgeAttrs, '\n')));
        end
    elseif igutils.isgraph(graph)
        if igutils.hasEdgeAttr(graph, 'Weight');
            args.weight = 'Weight';
        elseif igutils.hasEdgeAttr(graph)
            args.weight = igutils.listEdgeAttrs(graph)
            args.weight = args.weight{1};
        end
    else
        args.weight = 'Weight';
    end
end

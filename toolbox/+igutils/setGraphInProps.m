function args = setGraphInProps(graph, args, graphOutArgs)
% By adding graph out props, can use a single structure for graph in and graph
% out. This will then return the graphOutArgs without modification.

    arguments
        graph
        args.isweighted
        args.isdirected
        args.weight
        graphOutArgs.?igutils.GraphOutProps
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

    args = mergeStructs(args, graphOutArgs);
end


function s1 = mergeStructs(s1, s2)
    for fn = fieldnames(s2)'
        s1.(fn{1}) = s2.(fn{1});
    end
end

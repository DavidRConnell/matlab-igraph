function args = setGraphInProps(graph, args, graphOutArgs)
% By adding graph out props, can use a single structure for graph in and graph
% out. This will then return the graphOutArgs without modification.

    arguments
        graph
        args.isweighted
        args.isdirected
        args.weight
        graphOutArgs.?igraph.args.GraphOutProps
    end

    isoptionset = @igraph.args.isoptionset;
    if ~isoptionset(args, 'isdirected')
        args.isdirected = igraph.isdirected(graph);
    end

    if ~isoptionset(args, 'isweighted')
        args.isweighted = igraph.isweighted(graph);
    end

    if args.isweighted && isoptionset(args, 'weight')
        if ~igraph.args.isgraph(graph)
            warning("Option 'weight' set but is only used for 'graph' " + ...
                    "representations, the value will be ignored.");
        elseif ~igraph.args.hasEdgeAttr(graph, args.weight)
            edgeAttrs = graph.Edges.Properties.VariableNames;
            edgeAttrs = edgeAttrs(~strcmp(edgeAttrs, "EndNodes"));
            throwAsCaller(MException("igraph:invalidWeight", ...
                                     "The requested edge attribute ('%s') " + ...
                                     "to use for weights was not " + ...
                                     "found in graph. (Note names are " + ...
                                     "case sensitive.)\n\n" + ...
                                     "Available edge attributes are:\n%s", ...
                                     args.weight, ...
                                     strjoin("    " + edgeAttrs, '\n')));
        end
    elseif igraph.args.isgraph(graph)
        if igraph.args.hasEdgeAttr(graph, 'Weight');
            args.weight = 'Weight';
        elseif igraph.args.hasEdgeAttr(graph)
            args.weight = igraph.args.listEdgeAttrs(graph)
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

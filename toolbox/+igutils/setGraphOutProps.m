function args = setGraphOutProps(args, graphInArgs)
% By adding graph in props, can use a single structure for graph in and graph
% out. This will then return the graphInArgs without modification.

    arguments
        args.template
        args.repr
        args.dtype
        args.multiple
        args.weight
        graphInArgs.?igutils.GraphInProps
    end

    isoptionset = @igutils.isoptionset;
    if isoptionset(args, 'template')
        graph = args.template;
        args = rmfield(args, 'template');

        if ~isoptionset(args, 'dtype')
            args.dtype = dtype(graph);
        end

        if ~isoptionset(args, 'repr')
            args.repr = repr(graph);
        end
    else
        if ~isoptionset(args, 'dtype')
            args.dtype = 'double';
        end

        if ~isoptionset(args, 'repr')
            args.repr = 'graph';
        end
    end

    args.dtype = lower(args.dtype);
    args.repr = lower(args.repr);

    if isoptionset(args, 'weight')
        if ~strcmp(args.repr, 'graph')
            warning("Option weight is only used for 'graph' " + ...
                    "representation, ignoring value.");
        end
    else
        args.weight = 'Weight';
    end

    if isoptionset(args, 'multiple') && args.multiple && ...
            ~strcmp(args.repr, 'graph')
        throwAsCaller(MException("igraph:invalidOption", ...
                                 "Adjacency matrices cannot express " + ...
                                 "multigraphs, use 'graph' as " + ...
                                 "representation or set multiple to false."));
    end

    args = mergeStructs(args, graphInArgs);
end

function type = dtype(graph)
    type = 'double';
    if ~igutils.isgraph(graph) && islogical(graph)
        type = 'logical';
    end
end

function type = repr(graph)
   if igutils.isgraph(graph)
       type = 'graph';
   elseif issparse(graph)
       type = 'sparse';
   else
       type = 'full';
   end
end

function s1 = mergeStructs(s1, s2)
    for fn = fieldnames(s2)'
        s1.(fn{1}) = s2.(fn{1});
    end
end

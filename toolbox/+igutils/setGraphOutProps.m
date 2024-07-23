function args = setGraphOutProps(args)
%SETGRAPHOUTPROPS set the default values for unassigned graph in options
%   ARGS = SETGRAPHOUTPROPS(ARGS) determine defaults to use for any unset
%   options. In the case ARGS came from a argument block, it will be in the
%   form of a structure. To pass this to a function convert it to a cell with
%   NAMEDARGS2CELL then call with ARGS{:}.
%
%   ARGS = SETGRAPHOUTPROPS(ARGS, "TEMPLATE", GRAPH) use GRAPH as a template
%   and set defaults based on the characteristics of TEMPLATE.
%
%   Options for input graphs (IGUTILS.GRAPHINPROPS) can also be passed in and
%   will be returned untouched. This allows a single graph option set to be
%   used for both creation of the input graph and the returned graph.
%
%   See also IGUTILS.GRAPHOUTPROPS, IGUTILS.SETGRAPHINPROPS.

    arguments
        args.?igutils.GraphInProps
        args.template
        args.repr
        args.dtype
        args.multiple
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

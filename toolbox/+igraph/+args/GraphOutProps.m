classdef GraphOutProps
    properties (SetAccess = public)
        repr (1, :) char ...
            {igraph.args.mustBeMemberi(repr, {'sparse', 'full', 'graph'})};
        dtype (1, :) char ...
            {igraph.args.mustBeMemberi(dtype, {'double', 'logical'})};
        weight (1, :) char
    end
end

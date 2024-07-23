classdef GraphOutProps
%GRAPHOUTPROPS common arguments for returing graphs as outputs.
%   This class is intended solely to define common arguments that are used when
%   a graph is returned from a function. Use this in the arguments block to
%   ensure consistent use of arguments.
%
%   See also IGUTILS.GRAPHINPROPS, IGUTILS.SETGRAPHOUTPROPS.

    properties (SetAccess = public)
        repr (1, :) char ...
            {igutils.mustBeMemberi(repr, {'sparse', 'full', 'graph'})};
        dtype (1, :) char ...
            {igutils.mustBeMemberi(dtype, {'double', 'logical'})};
        weight (1, :) char
    end
end

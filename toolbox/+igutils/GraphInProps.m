classdef GraphInProps
%GRAPHINPROPS common arguments for using graphs as inputs.
%   This class is intended solely to define common arguments that are used when
%   a graph is passed as a function input. Use this in the arguments block to
%   ensure consistent use of arguments.
%
%   See also IGUTILS.GRAPHOUTPROPS, IGUTILS.SETGRAPHINPROPS.

    properties (SetAccess = public)
        isdirected (1, 1) logical
        isweighted (1, 1) logical
        weight (1, :) char
    end
end

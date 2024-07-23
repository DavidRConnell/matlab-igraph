classdef GraphInProps
    properties (SetAccess = public)
        isdirected (1, 1) logical
        isweighted (1, 1) logical
        weight (1, :) char
    end
end

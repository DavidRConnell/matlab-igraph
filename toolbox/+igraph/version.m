function varargout = version(type)
%VERSION get the igraph toolbox and library versions
%   VERSION() prints the version of both the toolbox and the C library.
%   VERSION("library") prints the version of the C library.
%   VERSION("toolbox") prints the version of the toolbox.
%
%   VER = VERSION(TYPE) returns a string representation of TYPE's version. TYPE
%   is either "library" or "toolbox".
%
%   VER = VERSION() returns a structure with fields toolbox and library
%   containing a string of their respective versions.

    arguments
        type (1, :) char {...
            igutils.mustBeMemberi(type, ...
                                  {'toolbox', 'library', 'both'})} = 'both';
    end

    if nargout == 0
        disp(mexIgraphVersion(type));
    elseif nargout == 1
        v = mexIgraphVersion(type);
        varargout = {v};
    else
        error("igraph:tooManyOutputs", "Version only returns one value.");
    end
end

function varargout = version()
%VERSION get the internal igraph libraries version
%   VERSION() print the igraph's version.
%   VER = VERSION() returns a string representation of igraph's version.

    if nargout == 0
        disp(mexIgraphVersion());
    elseif nargout == 1
        v = mexIgraphVersion();
        varargout = {v};
    else
        error("Igraph:tooManyOutputs", "Version only returns one value.");
    end
end

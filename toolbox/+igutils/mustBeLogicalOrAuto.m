function mustBeLogicalOrAuto(arg)
%MUSTBELOGICALORAUTO test if a logical or the string 'auto'

    if ~islogical(arg) && ~strcmp(arg, 'auto')
        eid = "Igraph:NotLogical";
        msg = sprintf("Value must be logical or 'auto'");
        throwAsCaller(MException(eid, msg));
    end
end

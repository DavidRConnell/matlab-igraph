function mustBeMode(str)
%MUSTBEMODE check if an allowed mode name

    modes = {'out', 'in', 'all'};
    if ~ismember(lower(str), modes)
        eid = "Igraph:NotAMode";
        msg = sprintf("Value must be member of this set:\n%s", ...
                      sprintf("%s", cellfun(@(str) sprintf("    '%s'\n", str),...
                                            modes)));
        throwAsCaller(MException(eid, msg));
    end
end

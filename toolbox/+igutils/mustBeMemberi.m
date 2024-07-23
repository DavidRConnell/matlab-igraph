function mustBeMemberi(needle, haystack)
%MUSTBEMEMBERI case-insensitive variant of mustBeMember

    if ~ismember(lower(needle), lower(haystack))
        eid = "Igraph:NotMember";
        msg = sprintf("Value must be member of this set:\n%s", ...
                      sprintf("%s", cellfun(@(str) sprintf("    '%s'\n", str),...
                                            haystack)));
        throwAsCaller(MException(eid, msg));
    end
end

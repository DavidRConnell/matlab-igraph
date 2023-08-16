function TF = isoptionset(options, field)
%ISOPTIONSET return true if the option in the option structure exists

    TF = any(contains(fieldnames(options), field));
end

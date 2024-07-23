function TF = isoptionset(options, field)
%ISOPTIONSET return true if the option in the option structure exists

    TF = any(strcmp(fieldnames(options), field));
end

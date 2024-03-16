function pref = generatePreference(nTypes, mixingParam)
    pref = ones(nTypes) * (mixingParam / (nTypes - 1));
    diagIdx = diag(reshape(1:(nTypes ^ 2), [nTypes nTypes]));
    pref(diagIdx) = 1 - mixingParam;
end

function adj = addWeights(adj)
    edgeIdx = adj ~= 0;
    weights = rand(sum(edgeIdx, 'all'), 1);
    adj(edgeIdx) = weights;
end

function [adj, c] = randomGraph(n, nClusters, dtype, useSparseAdj)
    clusterSizes = repmat(floor(n / nClusters), [nClusters 1]);
    clusterSizes(end) = n - sum(clusterSizes(1:end-1));

    edges = cell([1 nClusters]);
    c = zeros(sum(clusterSizes), nClusters);
    count = [1 1];
    for cluster = 1:nClusters
        edges{cluster} = ones(clusterSizes(cluster), dtype);

        count(2) = count(1) + clusterSizes(cluster);
        c(count(1):(count(2) - 1), cluster) = 1;
        count(1) = count(2);
    end
    adj = blkdiag(edges{:});
    if useSparseAdj
        adj = sparse(adj);
    end

    adj = randomizeAdjacency(adj, 0.8);
end

function adj = randomizeAdjacency(adj, pIn)
% For simplicity.
    pOut = 1 - pIn;
    probs = (adj .* pIn) + (~adj .* pOut);
    adj(probs > rand(size(adj))) = 1;
end

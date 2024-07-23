function g = randGraph(nNodes, density)
%RANDGRAPH Produce a random graph
%   Always produces a full.

    g = rand(nNodes) < density;
end

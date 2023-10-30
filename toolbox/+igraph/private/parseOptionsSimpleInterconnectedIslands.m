function opts = parseOptionsSimpleInterconnectedIslands(opts)
    arguments
        opts.nIslands (1, 1) {mustBeNonnegative, mustBeInteger} = 5;
        opts.islandSize (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
        opts.pIn (1, :) {mustBeInRange(opts.pIn, 0, 1)} = 0.75;
        opts.nInterEdges (1, 1) {mustBeNonnegative, mustBeInteger} = 4;
    end
end

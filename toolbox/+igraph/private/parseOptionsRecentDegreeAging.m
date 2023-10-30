function opts = parseOptionsRecentDegreeAging(opts)
    arguments
        opts.nNodes (1, 1) {mustBeNonnegative, mustBeInteger} = 10;
        opts.edgesPerStep (1, :) {mustBeNonnegative, mustBeInteger} = 3;
        opts.outPreference (1, 1) logical = false;
        opts.zeroAppeal (1, 1) {mustBeNonnegative, mustBeNumeric} = 0.1;
        opts.degreeExp (1, 1) {mustBeNumeric} = 1;
        opts.ageExp (1, 1) {mustBeNumeric} = 1;
        opts.agingBins (1, 1) {mustBePositive, mustBeInteger} = 3;
        opts.timeWindow (1, 1) {mustBePositive, mustBeInteger} = 3;
        opts.isdirected (1, 1) logical = false;
    end

    if length(opts.edgesPerStep) > 1
        opts.outSeq = opts.edgesPerStep;
        opts.edgesPerStep = 1;
    else
        opts.outSeq = [];
    end
end

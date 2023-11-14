function plot(adj, layout, options, layoutOpts)
%PLOT draw a graph
%   PLOT(ADJ, LAYOUT) use LAYOUT to plot the vertices of ADJ. LAYOUT can either
%       be a matrix of coordinates (nNodes x 2), such as a layout produced with
%       IGRAPH.LAYOUT, or the name of a method accepted by IGRAPH.LAYOUT.
%
%   PLOT(..., "MEMBERSHIP", C) Color the nodes based on the communities of
%       C. C should be a vector with a community label for each node (such as a
%       membership vector produced by IGRAPH.CLUSTER).
%
%   PLOT(..., "SIZE", SZ) Adjust the size of the plot markers. This can be
%       either a scalar or a vector with one size per node. This is used to
%       display node metrics such as the output of IGRAPH.CENTERALITY. If a
%       SZ is a vector it will be scaled.
%
%   PLOT(..., "AX", AXIS) Plot onto the provided axis instead of creating a new
%       one.
%
%   See also igraph.layout, igraph.plot3d.

    arguments
        adj {mustBeAdj};
        layout;
        options.membership (1, :) {mustBePartition} = [];
        options.size (1, :) = 100;
        options.ax = false;
        options.edgeAlpha = 0.5;
        options.edgeColor = 'k';

        layoutOpts.isdirected;
        layoutOpts.order;
        layoutOpts.center;
        layoutOpts.width;
        layoutOpts.nIterations;
        layoutOpts.charge;
        layoutOpts.mass;
        layoutOpts.springLenth;
        layoutOpts.springConstant;
        layoutOpts.stepMax;
        layoutOpts.initial;
        layoutOpts.types;
        layoutOpts.hgap;
        layoutOpts.vgap;
        layoutOpts.maxIterations;
        layoutOpts.startTemp;
        layoutOpts.grid;
        layoutOpts.epsilon;
        layoutOpts.constant;
        layoutOpts.tempMax;
        layoutOpts.tempMin;
        layoutOpts.tempInit;
        layoutOpts.fineIterations;
        layoutOpts.coolingFactor;
        layoutOpts.weightNodeDist;
        layoutOpts.weightBorder;
        layoutOpts.weightEdgeLen;
        layoutOpts.weightEdgeCross;
        layoutOpts.weightNodeEdgeDist;
        layoutOpts.area;
        layoutOpts.coolExp;
        layoutOpts.repulseRadius;
        layoutOpts.cellSize;
        layoutOpts.root;
        layoutOpts.mode;
        layoutOpts.circular
    end

    if isstring(layout) || ischar(layout)
        layoutOpts = namedargs2cell(layoutOpts);
        layout = igraph.layout(adj, layout, layoutOpts{:});
    end

    if size(layout, 1) ~= length(adj)
        error("igraph:wrongSize", "Layout must have exactly one row for " + ...
              "each node in the adj.");
    end

    if size(layout, 2) ~= 2
        error("igraph:wrongSize", "Layout must have exactly two columns.");
    end

    if isempty(options.membership)
        options.membership = ones(1, length(adj));
    end

    if length(options.membership) ~= length(adj)
        error("igraph:wrongSize", "Membership must have exactly " + ...
              "one element for each node in the adj.");
    end

    if isscalar(options.size)
        options.size = zeros(1, length(adj)) + options.size;
    elseif min(options.size) <= 0
        % Scaling the size vector to attempt to ensure a good spread of sizes.
        % May need tweaking.
        options.size = options.size - min(options.size);
        options.size = options.size / max(options.size);
        options.size = (options.size * 500) + 75;
    end

    if isa(options.ax, 'matlab.graphics.axis.Axes')
        fig = gcf();
        ax = options.ax;
    else
        fig = figure('visible', 'off');
        ax = subplot(1, 1, 1);
    end

    [from, to] = find(adj);
    options.edgeColor = validatecolor(options.edgeColor);
    options.edgeColor(4) = options.edgeAlpha;
    plot(ax, ...
         [layout(from, 1)'; layout(to, 1)'], ...
         [layout(from, 2)'; layout(to, 2)'], ...
         color=options.edgeColor);
    hold on

    for i = unique(options.membership)
        scatter(ax, layout(options.membership == i, 1), ...
             layout(options.membership == i, 2), ...
             options.size(options.membership == i), ...
             "filled")
    end
    hold off

    set(ax, 'xtick', []);
    set(ax, 'ytick', [])

    set(fig, 'visible', 'on');
end

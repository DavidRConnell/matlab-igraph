function plot(graph, layout, options, nodeOpts, edgeOpts, layoutOpts)
%PLOT draw a graph
%   PLOT(GRAPH, LAYOUT) use LAYOUT to plot the vertices of GRAPH. LAYOUT can
%       either be a matrix of coordinates (nNodes x 2), such as a layout
%       produced with IGRAPH.LAYOUT, or the name of a method accepted by
%       IGRAPH.LAYOUT.
%
%   PLOT(..., "NODECOLOR", C) Color the nodes based on the communities of C. C
%       should be a vector with a community label for each node (such as a
%       membership vector produced by IGRAPH.CLUSTER).
%
%   PLOT(..., "NODESIZE", SZ) Adjust the size of the plot markers. This can be
%       either a scalar or a vector with one size per node. This is used to
%       display node metrics such as the output of IGRAPH.CENTERALITY. If a SZ
%       is a vector it will be scaled.
%
%   PLOT(..., "AX", AXIS) Plot onto the provided axis instead of creating a new
%       one.
%
%   See also igraph.layout, igraph.plot3d.

    arguments
        graph {igutils.mustBeGraph};
        layout;

        options.ax = false;

        nodeOpts.nodeColor (1, :) = [];
        nodeOpts.size (1, :) = 100;

        edgeOpts.edgeAlpha (1, :) = 0.5;
        edgeOpts.edgeColor (1, :) = 'k';

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
        layout = igraph.layout(graph, layout, layoutOpts{:});
    end

    if size(layout, 1) ~= igraph.numnodes(graph)
        error("igraph:wrongSize", "Layout must have exactly one row for " + ...
              "each node in the graph.");
    end

    if size(layout, 2) ~= 2
        error("igraph:wrongSize", "Layout must have exactly two columns.");
    end

    if isempty(nodeOpts.nodeColor)
        nodeOpts.nodeColor = ones(1, igraph.numnodes(graph));
    else
        nodeOpts.nodeColor = ...
            igutils.validateNodeAttr(graph, nodeOpts.nodeColor);
    end

    if length(nodeOpts.nodeColor) ~= igraph.numnodes(graph)
        error("igraph:wrongSize", "Membership must have exactly " + ...
              "one element for each node in the graph.");
    end

    if isscalar(nodeOpts.size)
        nodeOpts.size = zeros(1, igraph.numnodes(graph)) + ...
            nodeOpts.size;
    else
        nodeOpts.size = igutils.validateNodeAttr(graph, nodeOpts.size);
    end

    if min(nodeOpts.size) <= 0
        % Scaling the size vector to attempt to ensure a good spread of sizes.
        % May need tweaking.
        nodeOpts.size = nodeOpts.size - min(nodeOpts.size);
        nodeOpts.size = nodeOpts.size / max(nodeOpts.size);
        nodeOpts.size = (nodeOpts.size * 500) + 75;
    end

    if isa(options.ax, 'matlab.graphics.axis.Axes')
        fig = gcf();
        ax = options.ax;
    else
        fig = figure('visible', 'off');
        ax = subplot(1, 1, 1);
    end

    if igutils.isgraph(graph)
        from = graph.Edges.EndNodes(:, 1);
        to = graph.Edges.EndNodes(:, 2);
    else
        [from, to] = find(graph);
    end

    edgeOpts.edgeColor = validatecolor(edgeOpts.edgeColor);
    edgeOpts.edgeColor(4) = edgeOpts.edgeAlpha;
    plot(ax, ...
         [layout(from, 1)'; layout(to, 1)'], ...
         [layout(from, 2)'; layout(to, 2)'], ...
         color=edgeOpts.edgeColor);
    hold on

    for i = unique(nodeOpts.nodeColor)
        scatter(ax, layout(nodeOpts.nodeColor == i, 1), ...
             layout(nodeOpts.nodeColor == i, 2), ...
             nodeOpts.size(nodeOpts.nodeColor == i), ...
             "filled")
    end
    hold off

    set(ax, 'xtick', []);
    set(ax, 'ytick', [])

    set(fig, 'visible', 'on');
end

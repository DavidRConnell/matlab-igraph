% Time the conversion of different MATLAB graph representation to and from
% igraph C types. Tests over a range of graph sizes and edge densities.

nNodes = round(10 .^ [1:0.1:3.5]);
densities = [0.1, 0.2, 0.5, 0.75, 1];
reprs = igutils.representations();
colors = [0 0.4470 0.7410
          0.8500 0.3250 0.0980
          0.9290 0.6940 0.1250
          0.4940 0.1840 0.5560
          0.4660 0.6740 0.1880
          0.3010 0.7450 0.9330
          0.6350 0.0780 0.1840];
symbols = {'^', 'square', 'pentagram'};

times = zeros(length(nNodes), length(densities), length(reprs));

for n = 1:length(nNodes)
    for d = 1:length(densities)
        g = randGraph(nNodes(n), densities(d));
        for r = 1:length(reprs)
            graphOpts = struct('dtype', 'double', 'repr', reprs{r}, ...
                               'isdirected', true);
            h = @() convertGraph(g, graphOpts);
            times(n, d, r) = timeit(h);
        end
    end
end

figure()
hold on
for d = 1:length(densities)
    for r = 1:length(reprs)
        semilogx(nNodes', squeeze(times(:, d, r)), ...
                 Color = colors(d, :), ...
                 Marker = symbols{r}, ...
                 DisplayName = sprintf("%s %g", reprs{r}, densities(d)));
        xlabel("Number of nodes");
        ylabel("Time (s)");
    end
end
legend
hold off

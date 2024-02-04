function buildtoolbox(toolboxFolder, outDir, arch, version)
    versionNumber = regexp(version, "v(\d+\.\d+\.\d+)", "tokens");
    versionNumber = versionNumber{1};

    releaseName = strjoin(['matlab-igraph', version, arch], '_');

    uuid = "e361fdb1-fd71-4184-b577-175f9d44ac9f";
    opts = matlab.addons.toolbox.ToolboxOptions(toolboxFolder, uuid);

    opts.ToolboxName = "matlab-igraph";
    opts.ToolboxVersion = versionNumber;
    opts.ToolboxGettingStartedGuide = fullfile(toolboxFolder, ...
                                               "GettingStarted.mlx");
    % opts.ToolboxMatlabPath = {toolboxFolder, fullfile(toolboxFolder, 'doc')};

    opts.AuthorName = "David R. Connell";
    opts.Summary = "Igraph bindings for Matlab";
    opts.Description = ...
        "This toolbox aims to provide a consistent API to perform " + ...
        "graph analyses inside MATLAB while following MATLAB's " + ...
        "conventions. The igraph C library provides efficient graph " + ...
        "analysis functions including graph generation, " + ...
        "reading/writing graphs to file, and community detection. " + ...
        "For more information see https://igraph.org.";

    if ~exist(outDir, "dir")
        mkdir(outDir);
    end
    opts.OutputFile = fullfile(outDir, releaseName);

    opts.MinimumMatlabRelease = "R2019b";
    opts.MaximumMatlabRelease = "";

    matlab.addons.toolbox.packageToolbox(opts);
end

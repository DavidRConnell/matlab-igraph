function buildtoolbox(toolboxFolder, outDir, version)
    versionNumber = regexp(version, "v(\d+\.\d+\.\d+)", "tokens");
    versionNumber = versionNumber{1};

    releaseName = strjoin(['matlab-igraph', version], '_');

    uuid = "e100c63a-9d55-4527-9e0b-a43d8ff89d03";
    opts = matlab.addons.toolbox.ToolboxOptions(toolboxFolder, uuid);

    opts.ToolboxName = "matlab-igraph";
    opts.ToolboxVersion = versionNumber;
    opts.ToolboxImageFile = "images/matlab_igraph_logo.png";
    opts.ToolboxGettingStartedGuide = fullfile(toolboxFolder, ...
                                               "GettingStarted.mlx");

    opts.AuthorName = "David R. Connell";
    opts.Summary = "igraph bindings for Matlab";
    opts.Description = ...
        "This toolbox aims to provide a consistent API to perform " + ...
        "graph analyses inside MATLAB while following MATLAB's " + ...
        "conventions. The igraph C library exposes efficient graph " + ...
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

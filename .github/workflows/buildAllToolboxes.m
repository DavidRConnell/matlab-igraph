function buildAllToolboxes(artifactsDir, outDir, version)
    for d = dir(artifactsDir + "/matlab-igraph*")'
        toolboxDir = fullfile(artifactsDir, d(1).name, ...
                              "matlab-igraph", "toolbox");
        zipFile = dir(fullfile(artifactsDir, d(1).name, ...
                               "matlab-igraph_" + version + "*"));
        arch = regexp(zipFile.name, version + '_(.*)-toolbox', "tokens");
        buildtoolbox(toolboxDir, outDir, arch{1}{1}, version);
    end
end

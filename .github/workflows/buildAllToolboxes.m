function buildAllToolboxes(artifactsDir, outDir, version)
    tmpDir = "tmp/matlab-igraph";
    compiledFileDirs = {"+igraph/private"};
    for d = dir(artifactsDir + "/matlab-igraph*")'
        toolboxDir = fullfile(artifactsDir, d(1).name, ...
                              "matlab-igraph", "toolbox");
        zipFile = dir(fullfile(artifactsDir, d(1).name, ...
                               "matlab-igraph_" + version + "*"));
        arch = regexp(zipFile.name, version + '_(.*)-toolbox', "tokens");
        buildtoolbox(toolboxDir, outDir, arch{1}{1}, version);

        if ~exist(tmpDir, 'dir')
            mkdir(tmpDir);
            copyfile(toolboxDir, fullfile(tmpDir, "toolbox"));
        else
            for mexDir = compiledFileDirs
                copyfile(fullfile(toolboxDir, mexDir{1}, ...
                                  "*." + archmexext(arch{1}{1})), ...
                         fullfile(tmpDir, "toolbox", mexDir{1}));
                if startsWith(arch{1}{1}, "win")
                    copyfile(fullfile(toolboxDir, mexDir{1}, "*.dll"), ...
                             fullfile(tmpDir, "toolbox", mexDir{1}));
                elseif startsWith(arch{1}{1}, "gln")
                    copyfile(fullfile(toolboxDir, mexDir{1}, "lib"), ...
                             fullfile(tmpDir, "toolbox", mexDir{1}));
                end
            end
        end
    end

    buildtoolbox(fullfile(tmpDir, "toolbox"), outDir, "combined", version);
end

function ext = archmexext(arch)
    ext = 'notfound';
    extList = mexext('all');
    for i = 1:length(extList)
       if strcmp(arch, extList(i).arch)
           ext = extList(i).ext;
           return
       end
    end
end

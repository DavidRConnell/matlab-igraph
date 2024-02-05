function postinstall()
% POSTINSTALL system specific clean up of matlab-igraph toolbox
%   POSTINSTALL() Remove compiled files not needed on the current system.
%
%   In order to make the toolbox independent of the end system, files for all
%   supported architects are combined into a single toolbox. To save space,
%   unneeded files can be cleared with this command.

    toolboxDir = findToolbox();
    currentSystem = lower(computer());
    extList = mexext('all');
    compiledFilePaths = {'+igraph/private'};

    for p = compiledFilePaths
        path = fullfile(toolboxDir, p);
        for i = 1:length(extList)
            if ~strcmp(currentSystem, extList(i).arch)
                delete(fullfile(path{1}, "*." + extList(i).ext));
            end
        end

        if ~(startsWith(currentSystem, 'win'))
            delete(fullfile(path{1}, "*.dll"));
        end

        if ~(startsWith(currentSystem, 'gln'))
            rmdir(fullfile(path{1}, "lib"), "s");
        end
    end
end

function d = findToolbox()
    fileParts = strsplit(mfilename('fullpath'), filesep);
    rng = 1:length(fileParts);
    toolboxIdx = rng(strcmp(fileParts, "toolbox"));
    d = strjoin(fileParts(1:toolboxIdx), filesep);
end

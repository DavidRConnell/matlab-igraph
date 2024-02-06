function postinstall()
% POSTINSTALL system specific clean up of matlab-igraph toolbox
%   POSTINSTALL() Remove compiled files not needed on the current system.
%
%   In order to make the toolbox independent of the end system, files for all
%   supported architects are combined into a single toolbox. To save space,
%   unneeded files can be cleared with this command.

    toolboxDir = findToolbox();
    currentSystem = computer('arch');
    extList = mexext('all');
    compiledFilePaths = {'+igraph/private'};

    for p = compiledFilePaths
        path = fullfile(toolboxDir, p);
        for i = 1:length(extList)
            if ~strcmp(currentSystem, extList(i).arch)
                delete(fullfile(path{1}, "*." + extList(i).ext));
            end
        end

        if ~startsWith(currentSystem, 'win')
            delete(fullfile(path{1}, "*.dll"));
        end

        if exist(fullfile(path{1}, 'lib'), 'dir') && ...
             ~startsWith(currentSystem, 'gln')
            rmdir(fullfile(path{1}, "lib"), "s");
        end
    end

    self = mfilename('fullpath') + ".m";
    delete(self);
end

function d = findToolbox()
    fileParts = strsplit(mfilename('fullpath'), filesep);
    rng = 1:length(fileParts);
    % Find the dir above +igraph in case the toolbox gets installed under a
    % new name.
    toolboxIdx = rng(strcmp(fileParts, "+igraph")) - 1;
    d = strjoin(fileParts(1:toolboxIdx), filesep);
end

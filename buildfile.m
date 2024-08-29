function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";

    plan("test").Dependencies = ["testMxIgraph" "testToolbox"];
end

function testTask(~)
end

function makeDocsTask(~)
    docDir = "docs/content";
    if ~exist(fullfile(docDir, "start"), "dir")
        mkdir(fullfile(docDir, "start"));
    end

    export("toolbox/GettingStarted.mlx", fullfile(docDir, "start", "_index.md"));

    examples = dir("toolbox/examples/*.mlx");
    dest = fullfile(docDir, "examples");
    if ~isempty(examples) && ~exist(dest, 'dir')
        mkdir(dest);
    end

    for i = 1:length(examples)
        export(fullfile(examples(i).folder, examples(i).name), ...
               replace(fullfile(dest, examples(i).name), ".mlx", ".md"));
    end
end

function testMxIgraphTask(~)
    oldPath = addpath("toolbox");
    runtests("tests/mxIgraph");
    path(oldPath);
end

function testToolboxTask(~)
    oldPath = addpath("toolbox");
    runtests("tests/toolbox");
    path(oldPath);
end

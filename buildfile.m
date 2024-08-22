function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";

    plan("test").Dependencies = ["testMxIgraph" "testToolbox"];
end

function testTask(~)
end

function makeDocsTask(~)
    export("toolbox/GettingStarted.mlx", "docs/content/start/_index.md");

    examples = dir("toolbox/examples/*.mlx");
    dest = "docs/content/examples";

    if ~isempty(examples) && ~exist(dest, 'dir')
        mkdir(dest);
    end

    for i = 1:length(examples)
        export(fullfile("toolbox", "examples", examples(i).name), ...
               fullfile(dest, examples(i).name))
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

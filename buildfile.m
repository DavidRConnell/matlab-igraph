function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";

    plan("test").Dependencies = ["testMxIgraph" "testToolbox"];
end

function testTask(~)
end

function makeDocsTask(~)
    export("toolbox/GettingStarted.mlx", "docs/content/start/_index.md");
end

function testMxIgraphTask(~)
    runtests("tests/mxIgraph");
end

function testToolboxTask(~)
    oldPath = addpath("toolbox");
    runtests("tests/toolbox");
    path(oldPath);
end

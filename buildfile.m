function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";
    plan("test").Dependencies = {"testMxIgraph", "testToolboxTask"};
end

function testTask(~)
end

function testMxIgraphTask(~)
    results = runtests("tests/mxIgraph");
end

function testToolboxTask(~)
    oldPath = addpath("toolbox");
    results = runtests("tests/toolbox");
    path(oldPath);
end

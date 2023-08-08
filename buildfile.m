function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";
    plan("test").Dependencies = "testMxIgraph";
end

function testTask(~)
end

function testMxIgraphTask(~)
    results = runtests("tests/mxIgraph");
end

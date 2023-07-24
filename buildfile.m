function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";
    % plan("test").Dependencies = [ ];
end

function testTask(~)
    results = runtests(BaseDirectory="tests", OutputDetail="terse");
end

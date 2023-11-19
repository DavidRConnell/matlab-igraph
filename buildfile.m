function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";

    plan("test").Dependencies = ["testSetupEnv" "testMxIgraph" "testToolbox"];
end

function testTask(~)
end

function makeDocsTask(~)
    export("toolbox/gettingStarted.mlx", "docs/content/start/_index.md");
end

function runTest(name)
    import matlab.unittest.plugins.XMLPlugin

    suite = testsuite(['tests/' name]);
    runner = testrunner('textoutput');
    xmlFile = ['build/tests/' name 'TestResults.xml'];
    p = XMLPlugin.producingJUnitFormat(xmlFile);
    addPlugin(runner, p);

    run(runner, suite);
end

function testSetupEnvTask(~)
    if ~exist('build/tests', 'dir')
        mkdir('build/tests');
    end
end

function testMxIgraphTask(~)
    runTest('mxIgraph')
end

function testToolboxTask(~)
    oldPath = addpath("toolbox");
    runTest('toolbox');
    path(oldPath);
end

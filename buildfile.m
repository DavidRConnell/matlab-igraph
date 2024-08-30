function plan = buildfile
    plan = buildplan(localfunctions);
    plan.DefaultTasks = "test";

    plan("test").Dependencies = ["testMxIgraph" "testToolbox"];
    plan("makeDocs").Dependencies = ["copyDocFiles" "exportLivescripts"];
end

function testTask(~)
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

function makeDocsTask(~)
end

function copyDocFilesTask(~)
    docDir = "docs/content";

    if ~exist(fullfile(docDir, "changelog"))
        mkdir(fullfile(docDir, "changelog"));
    end

    copyfile("CHANGELOG.md", fullfile(docDir, "changelog", "_index.md"));
    copyfile("CONTRIBUTING.org", fullfile(docDir, "docs", "contributing.org"));
end

function exportLivescriptsTask(~)
    oldPath = addpath("toolbox");

    dest = "docs/content/start";
    if ~exist(dest, "dir")
        mkdir(dest);
    end

    export("toolbox/GettingStarted.mlx", fullfile(dest, "_index.md"));
    title2frontmatter(fullfile(dest, "_index.md"));

    examples = dir("toolbox/examples/*.mlx");
    for i = 1:length(examples)
        clear mex
        outfile = replace(fullfile(dest, examples(i).name), ".mlx", ".md");
        export(fullfile(examples(i).folder, examples(i).name), outfile);
        title2frontmatter(outfile);
    end

    path(oldPath);
end

function title2frontmatter(fname)
% Convert the title produced by export to Hugo legible frontmatter.

    content = strip(fileread(fname));
    span_open = "# <span style=""color:rgb\(213,80,0\)"">";
    span_close = "</span>";

    title = regexp(content, span_open + "(.*)" + span_close, 'once', 'tokens');
    title = title{1};

    span_open = replace(span_open, "\", "");

    % Without sprintf, the newline characters are left as the literal '\n'.
    content = replace(content, ...
                      span_open + title + span_close, ...
                      sprintf("---\ntitle: " + title + "\ncascade:\n  type: docs\n---\n"));

    writelines(content, fname);
end

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

    if ~exist(fullfile(docDir, "changelog"), "dir")
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
    mlx2hugoFilter(fullfile(dest, "_index.md"));

    examples = dir("toolbox/examples/*.mlx");
    for i = 1:length(examples)
        outfile = replace(fullfile(dest, examples(i).name), ".mlx", ".md");
        export(fullfile(examples(i).folder, examples(i).name), outfile);
        mlx2hugoFilter(outfile);
    end

    path(oldPath);
end

function mlx2hugoFilter(fname)
    content = strip(fileread(fname));

    content = title2frontmatter(content);
    content = rewriteLinks(content);

    writelines(content, fname);
end

function content = title2frontmatter(content)
% Convert the title produced by export to Hugo legible frontmatter.

    span_open = "# <span style=""color:rgb\(213,80,0\)"">";
    span_close = "</span>";

    [title, pos] = regexp(content, span_open + "(.*)" + span_close, ...
                          'once', 'tokens', 'end');

    title = title{1};

    frontmatter = sprintf("---\ntitle: " + title + ...
                          "\ncascade:\n  type: docs\n---\n");

    content = frontmatter + content(pos:end);
end

function content = rewriteLinks(content)
% Convert links to mlx files to hugo shortcode links.

    paths = regexp(content, "\[.*?\]\((.*?)\)", "tokens");
    uniquePaths = cell(1, length(paths));
    for i = 1:length(paths)
        uniquePaths{i} = paths{i}{1};
    end
    uniquePaths = unique(uniquePaths);

    for i = 1:length(uniquePaths)
        path = uniquePaths{i};
        [~, base, ext] = fileparts(path);
        if ext == ".mlx"
            content = replace(content, path, "{{< ref """ + base + """ >}}");
        end
    end
end

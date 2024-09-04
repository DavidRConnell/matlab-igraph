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
    dest = "docs/content/start";
    if ~exist(dest, "dir")
        mkdir(dest);
    end

    mlx2hugo("toolbox/GettingStarted.mlx", fullfile(dest, "_index.md"));

    examples = dir("toolbox/examples/*.mlx");
    for i = 1:length(examples)
        outfile = replace(fullfile(dest, examples(i).name), ".mlx", ".md");
        mlx2hugo(fullfile(examples(i).folder, examples(i).name), outfile);
    end
end

function mlx2hugo(input, output)
    export(input, output, EmbedImages=true);
    content = strip(fileread(output));

    content = title2frontmatter(content);
    content = rewriteLinks(content);

    writelines(content, output);
end

function content = title2frontmatter(content)
% Convert the title produced by export to Hugo legible frontmatter.

    span_open = "# <span style=""color:rgb\(\d{1,3},\d{1,3},\d{1,3}\)"">";
    span_close = "</span>";

    [title, pos] = regexp(content, span_open + "(.*)" + span_close, ...
                          'once', 'tokens', 'end');

    title = title{1};

    frontmatter = sprintf("---\ntitle: " + title + ...
                          "\ncascade:\n  type: docs\n---\n");

    content = frontmatter + content((pos + 1):end);
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

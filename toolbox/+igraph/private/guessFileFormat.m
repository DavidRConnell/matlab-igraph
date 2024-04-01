function type = guessFileFormat(filename)
    [~, name, ext] = fileparts(filename);

    if isempty(name)
        error("igraph:missingFileName", ...
             "Filename must be a non-empty character vector or string scalar.");
    end

    switch lower(ext)
      case ".mat"
        type = "mat";
      case ".txt"
        type = "edgelist";
        fid = fopen(filename);
        if fid == -1
            return
        end

        firstLine = strsplit(fgetl(fid));
        if strcmp(firstLine{1}, 'dl')
            type = "dl";
        end

        fclose(fid);
      case {".tsv", ".csv", ".edges"}
        type = "edgelist";
      case ".dl"
        type = "dl";
      case ".ncol"
        type = "ncol";
      case ".lgl"
        type = "lgl";
      case ".col"
        type = "dimacs";
      case ".zip"
        type = "graphdb";
      case ".graphml"
        type = "graphml";
      case ".gml"
        type = "gml";
      case ".net"
        type = "pajek";
      case {".gv", ".dot"}
        type = "dot";
      case {".gw", ".lgr"}
        type = "leda";
      otherwise
        error("igraph:unknownFileType", ...
              "Unrecognized file extension %s. Use the 'FileType' parameter to specify the file type.", ...
              ext);
    end
end

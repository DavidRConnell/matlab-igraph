function convert(fileIn, fileOut, options)
%CONVERT write a graph to a different file format
%   CONVERT(FILEIN, FILEOUT) load FILEIN and right it to path FILEOUT, using
%   the file extensions to guess file formats.
%   CONVERT(..., "FROM", INFORMAT, "TO", OUTFORMAT) explicitly state the graph
%   formats of the input and/or output files.
%   CONVERT(..., 'OVERWRITE', TF) The SAVE function will not write over an
%   already existing file by default. Setting the OVERWRITE flag to true will
%   tell SAVE to overwrite a file if it already exists.
%   CONVERT(..., 'DELETE', TF) by default, the input file is not modified. To
%   delete the input file after conversion, set the DELETE flag to true.
%
%   See also IGRAPH.LOAD, IGRAPH.SAVE.

    arguments
        fileIn (1, :) char;
        fileOut (1, :) char;
        options.inFormat (1, :) char = guessFileFormat(fileIn);
        options.outFormat (1, :) char = guessFileFormat(fileOut);
        options.overwrite (1, 1) logical = false;
        options.delete (1, 1) logical = false;
    end

    graph = igraph.load(fileIn, "format", options.inFormat);
    igraph.save(fileOut, graph, ...
                "format", options.outFormat, ...
                "overwrite", options.overwrite);

    if options.delete
        delete(fileIn);
    end
end

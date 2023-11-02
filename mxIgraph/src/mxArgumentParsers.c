#include <string.h>

#include "mxIgraph.h"

/* Generic string selector.
   Find which of a predefined set of strings an input value matches.
   Returns the index, which should correspond to an enum that can be used in a
   switch statement.

   If no match found, returns `n_methods`. */
igraph_integer_t mxIgraphSelectMethod(const mxArray *p, const char *methods[],
                                      const igraph_integer_t n_methods)
{
  if (!mxIsChar(p)) {
    mexErrMsgIdAndTxt("Igraph:notChar",
                      "Method name must be specified as a string.");
  }
  char *name = mxArrayToString(p);

  for (igraph_integer_t i = 0; i < n_methods; i++) {
    if (strcmp(name, methods[i]) == 0) {
      return i;
    }
  }

  return n_methods;
}

// Select a file format.
mxIgraphFileFormat_t mxIgraphSelectFileFormat(const mxArray *p)
{
  mxIgraphFileFormat_t res;
  const char *file_formats[MXIGRAPH_FORMAT_N] = {
    [MXIGRAPH_FORMAT_EDGELIST] = "edgelist",
    [MXIGRAPH_FORMAT_NCOL] = "ncol",
    [MXIGRAPH_FORMAT_LGL] = "lgl",
    [MXIGRAPH_FORMAT_DIMACS] = "dimacs",
    [MXIGRAPH_FORMAT_GRAPHDB] = "graphdb",
    [MXIGRAPH_FORMAT_GRAPHML] = "graphml",
    [MXIGRAPH_FORMAT_GML] = "gml",
    [MXIGRAPH_FORMAT_PAJEK] = "pajek",
    [MXIGRAPH_FORMAT_DL] = "dl",
    [MXIGRAPH_FORMAT_DOT] = "dot",
    [MXIGRAPH_FORMAT_LEDA] = "leda"
  };

  res = mxIgraphSelectMethod(p, file_formats, MXIGRAPH_FORMAT_N);
  if (res == MXIGRAPH_FORMAT_N) {
    mexErrMsgIdAndTxt("Igraph:unknownFileFormat",
                      "Received unknown file format.");
  }

  return res;
}

// Select a data type.
mxIgraphDType_t mxIgraphSelectDType(const mxArray *p)
{
  mxIgraphDType_t res;
  const char *dtypes[MXIGRAPH_DTYPE_N] = {
    [MXIGRAPH_DTYPE_LOGICAL] = "logical",
    [MXIGRAPH_DTYPE_DOUBLE] = "double"
  };

  res = mxIgraphSelectMethod(p, dtypes, MXIGRAPH_DTYPE_N);
  if (res == MXIGRAPH_DTYPE_N) {
    mexErrMsgIdAndTxt("Igraph:unknownDType", "Received unknown data type.");
  }

  return res;
}

// Select a neighbor mode.
igraph_neimode_t mxIgraphSelectMode(const mxArray *p)
{
  igraph_neimode_t res;
  igraph_integer_t n_modes = 3;
  // Mode enum starts at 1 not 0.
  const char *modes[3] = {
    [IGRAPH_OUT - 1] = "out",
    [IGRAPH_IN - 1] = "in",
    [IGRAPH_ALL - 1] = "all"
  };

  res = mxIgraphSelectMethod(p, modes, n_modes);

  if (res == n_modes) {
    mexErrMsgIdAndTxt("Igraph:unknownMode", "Received unknown mode type.");
  }

  return res + 1;
}

#include <stdio.h>

#include "mxIgraph.h"
#include "igraph.h"

#define mexIgraphError(id, msg)			\
  fclose(fptr);					\
  igraph_destroy(&graph);			\
  mexErrMsgIdAndTxt((id), (msg));		\
  exit(1)

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetErrorHandler();

  if (nlhs != 0) {
    mexErrMsgIdAndTxt("Igraph:internal:tooManyOutputs",
                      "%s does not provide any outputs.", mexFunctionName());
  }

  if (nrhs != 5) {
    mexErrMsgIdAndTxt("Igraph:internal:wrongNumberOfInputs",
                      "%s must have 5 inputs.", mexFunctionName());
  }

  igraph_t graph;
  igraph_vector_t weights;
  char *filename = mxArrayToString(prhs[1]);
  mxIgraphFileFormat_t format = mxIgraphSelectFileFormat(prhs[2]);
  igraph_bool_t isweighted = mxGetScalar(prhs[3]);
  igraph_bool_t isdirected = mxGetScalar(prhs[4]);
  FILE *fptr;
  igraph_error_t errcode = IGRAPH_SUCCESS;
  igraph_error_handler_t *oldhandler;

  if (!(fptr = fopen(filename, "w"))) {
    mexErrMsgIdAndTxt("Igraph:IOError", "Could not open file %s for writing.",
                      filename);
  }

  if (isweighted) {
    igraph_set_attribute_table(&igraph_cattribute_table);
  }

  mxIgraphGetGraph(prhs[0], &graph, &weights, isdirected);

  if (isweighted) {
    SETEANV(&graph, "weight", &weights);
    igraph_vector_destroy(&weights);
  }

  switch (format) {
  case MXIGRAPH_FORMAT_EDGELIST:
    errcode = igraph_write_graph_edgelist(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_NCOL:
    if (isweighted) {
      errcode = igraph_write_graph_ncol(&graph, fptr, NULL, "weight");
    } else {
      errcode = igraph_write_graph_ncol(&graph, fptr, NULL, NULL);
    }
    break;
  case MXIGRAPH_FORMAT_LGL:
    if (isweighted) {
      errcode = igraph_write_graph_lgl(&graph, fptr, NULL, "weight", true);
    } else {
      errcode = igraph_write_graph_lgl(&graph, fptr, NULL, NULL, true);
    }
    break;
  case MXIGRAPH_FORMAT_DIMACS:
    mexIgraphError("Igraph:notImplemented",
                   "The DIMACS format has not been implemented in matlab-igraph.");
    break;
  case MXIGRAPH_FORMAT_GRAPHDB:
    mexIgraphError("Igraph:notImplemented",
                   "Upstream igraph does not support writing to graphdb format.");
    break;
  case MXIGRAPH_FORMAT_GRAPHML:
    oldhandler = igraph_set_error_handler(igraph_error_handler_ignore);
    errcode = igraph_write_graph_graphml(&graph, fptr, true);
    if (errcode == IGRAPH_UNIMPLEMENTED) {
      mexIgraphError("Igraph:notSupported",
                     "igraph was compiled without GraphML support.");
    }
    igraph_set_error_handler(oldhandler);
    break;
  case MXIGRAPH_FORMAT_GML:
    errcode = igraph_write_graph_gml(&graph, fptr, IGRAPH_WRITE_GML_DEFAULT_SW, 0,
                                     NULL);
    break;
  case MXIGRAPH_FORMAT_PAJEK:
    errcode = igraph_write_graph_pajek(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_DL:
    mexIgraphError("Igraph:notImplemented",
                   "Upstream igraph does not support writing to dl format.");
    break;
  case MXIGRAPH_FORMAT_DOT:
    errcode = igraph_write_graph_dot(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_LEDA:
    if (isweighted) {
      errcode = igraph_write_graph_leda(&graph, fptr, NULL, "weight");
    } else {
      errcode = igraph_write_graph_leda(&graph, fptr, NULL, NULL);
    }
    break;
  default:
    fclose(fptr);
    igraph_destroy(&graph);
    mexErrMsgIdAndTxt("Igraph:unknownFileType",
                      "Unrecognized file format %s. See `help igraph.save` for available file formats.",
                      format);
    exit(1);
  }

  if (errcode != IGRAPH_SUCCESS) {
    mexIgraphError("Igraph:internal:IOError",
                   "Failed to write graph to file.");
  }

  fclose(fptr);
  igraph_destroy(&graph);
}

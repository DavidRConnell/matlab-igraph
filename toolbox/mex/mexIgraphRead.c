#include <stdio.h>

#include "mxIgraph.h"
#include "igraph.h"

#define mexIgraphError(eid, msg)		\
  fclose(fptr);					\
  igraph_destroy(&graph);			\
  igraph_vector_destroy(&weights);		\
  mexErrMsgIdAndTxt((eid), (msg));		\
  exit(1)

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nlhs != 1) {
    mexErrMsgIdAndTxt("Igraph:internal:wrongNumberOfOutputs",
                      "%s returns a single output.", mexFunctionName());
  }

  if (nrhs != 7) {
    mexErrMsgIdAndTxt("Igraph:internal:wrongNumberOfInputs",
                      "%s must have 7 inputs.", mexFunctionName());
  }

  igraph_t graph;
  igraph_vector_t weights;
  char *filename = mxArrayToString(prhs[0]);
  mxIgraphFileFormat_t format = mxIgraphSelectFileFormat(prhs[1]);
  igraph_bool_t is_weighted = mxGetScalar(prhs[2]);
  igraph_bool_t is_directed = mxGetScalar(prhs[3]);
  igraph_bool_t make_sparse = mxGetScalar(prhs[4]);
  mxIgraphDType_t dtype = mxIgraphSelectDType(prhs[5]);
  igraph_integer_t index = mxGetScalar(prhs[6]);
  FILE *fptr;
  igraph_error_t errcode = IGRAPH_SUCCESS;
  igraph_error_handler_t *oldhandler;

  if (!(fptr = fopen(filename, "r"))) {
    mexErrMsgIdAndTxt("Igraph:IOError", "Could not open file %s for reading.",
                      filename);
  }

  igraph_set_attribute_table(&igraph_cattribute_table);

  switch (format) {
  case MXIGRAPH_FORMAT_EDGELIST:
    errcode = igraph_read_graph_edgelist(&graph, fptr, 0, is_directed);
    break;
  case MXIGRAPH_FORMAT_NCOL:
    errcode = igraph_read_graph_ncol(&graph, fptr, NULL, NULL,
                                     IGRAPH_ADD_WEIGHTS_IF_PRESENT, is_directed);
    break;
  case MXIGRAPH_FORMAT_LGL:
    errcode = igraph_read_graph_lgl(&graph, fptr, NULL,
                                    IGRAPH_ADD_WEIGHTS_IF_PRESENT, is_directed);
    break;
  case MXIGRAPH_FORMAT_DIMACS:
    mexIgraphError("Igraph:notImplemented",
                   "The DIMACS format has not been implemented in matlab-igraph");
    break;
  case MXIGRAPH_FORMAT_GRAPHDB:
    errcode = igraph_read_graph_graphdb(&graph, fptr, is_directed);
    break;
  case MXIGRAPH_FORMAT_GRAPHML:
    oldhandler = igraph_set_error_handler(igraph_error_handler_ignore);
    errcode = igraph_read_graph_graphml(&graph, fptr, index);
    if (errcode == IGRAPH_UNIMPLEMENTED) {
      mexIgraphError("Igraph:notSupported",
                     "igraph was compiled without GraphML support.");
    }
    igraph_set_error_handler(oldhandler);
    break;
  case MXIGRAPH_FORMAT_GML:
    errcode = igraph_read_graph_gml(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_PAJEK:
    errcode = igraph_read_graph_pajek(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_DL:
    errcode = igraph_read_graph_dl(&graph, fptr, is_directed);
    break;
  case MXIGRAPH_FORMAT_DOT:
    mexIgraphError("Igraph:notImplemented",
                   "Upstream igraph does not support reading DOT files.");
    break;
  case MXIGRAPH_FORMAT_LEDA:
    mexIgraphError("Igraph:notImplemented",
                   "Upstream igraph does not support reading LEDA files.");
    break;
  default:
    fclose(fptr);
    mexErrMsgIdAndTxt("Igraph:unknownFileType",
                      "Unrecognized file format %s. See `help igraph.load` for available file formats.",
                      format);
    exit(1);
  }

  if (errcode != IGRAPH_SUCCESS) {
    mexIgraphError("Igraph:internal:IOError",
                   "Failed to read graph from file.");
  }

  igraph_vector_init(&weights, igraph_ecount(&graph));
  if (is_weighted &&
      igraph_cattribute_has_attr(&graph, IGRAPH_ATTRIBUTE_EDGE, "weight")) {
    EANV(&graph, "weight", &weights);
  } else {
    for (igraph_integer_t i = 0; i < igraph_ecount(&graph); i++) {
      VECTOR(weights)[i] = 1;
    }
  }

  plhs[0] = mxIgraphCreateAdj(&graph, &weights, make_sparse, dtype);

  fclose(fptr);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
}

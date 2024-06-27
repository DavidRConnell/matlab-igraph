#include <stdio.h>

#include <mxIgraph.h>
#include <utils.h>

#include <igraph_foreign.h>
#include <igraph_attributes.h>

#define mexIgraphError(eid, msg)		\
  fclose(fptr);					\
  igraph_destroy(&graph);			\
  igraph_vector_destroy(&weights);		\
  mexErrMsgIdAndTxt((eid), (msg));		\
  exit(1)

igraph_error_t mexIgraphRead(int nlhs, mxArray* plhs[], int nrhs,
                             mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* method_options = prhs[1];
  mxArray const* adj_options = prhs[2];
  igraph_t graph;
  igraph_vector_t weights;
  char* filename = mxArrayToString(prhs[0]);
  mxIgraphFileFormat_t format = mxIgraphSelectFileFormat(mxIgraphGetArgument(
                                  method_options, "format"));
  igraph_integer_t index = mxIgraphGetInteger(method_options, "index");

  igraph_bool_t is_weighted = mxIgraphGetBool(adj_options, "isweighted");
  igraph_bool_t is_directed = mxIgraphGetBool(adj_options, "isdirected");
  FILE* fptr;
  igraph_error_t errorcode = IGRAPH_SUCCESS;
  igraph_error_handler_t* oldhandler;

  if (!(fptr = fopen(filename, "r"))) {
    mexErrMsgIdAndTxt("Igraph:IOError", "Could not open file %s for reading.",
                      filename);
  }

  igraph_set_attribute_table(&igraph_cattribute_table);

  switch (format) {
  case MXIGRAPH_FORMAT_EDGELIST:
    errorcode = igraph_read_graph_edgelist(&graph, fptr, 0, is_directed);
    break;
  case MXIGRAPH_FORMAT_NCOL:
    errorcode = igraph_read_graph_ncol(&graph, fptr, NULL, NULL,
                                       IGRAPH_ADD_WEIGHTS_IF_PRESENT, is_directed);
    break;
  case MXIGRAPH_FORMAT_LGL:
    errorcode = igraph_read_graph_lgl(&graph, fptr, NULL,
                                      IGRAPH_ADD_WEIGHTS_IF_PRESENT, is_directed);
    break;
  case MXIGRAPH_FORMAT_DIMACS:
    mexIgraphError("Igraph:notImplemented",
                   "The DIMACS format has not been implemented in matlab-igraph");
    break;
  case MXIGRAPH_FORMAT_GRAPHDB:
    errorcode = igraph_read_graph_graphdb(&graph, fptr, is_directed);
    break;
  case MXIGRAPH_FORMAT_GRAPHML:
    oldhandler = igraph_set_error_handler(igraph_error_handler_ignore);
    errorcode = igraph_read_graph_graphml(&graph, fptr, index);
    if (errorcode == IGRAPH_UNIMPLEMENTED) {
      mexIgraphError("Igraph:notSupported",
                     "igraph was compiled without GraphML support.");
    }
    igraph_set_error_handler(oldhandler);
    break;
  case MXIGRAPH_FORMAT_GML:
    errorcode = igraph_read_graph_gml(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_PAJEK:
    errorcode = igraph_read_graph_pajek(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_DL:
    errorcode = igraph_read_graph_dl(&graph, fptr, is_directed);
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

  igraph_vector_init(&weights, igraph_ecount(&graph));
  if (is_weighted &&
      igraph_cattribute_has_attr(&graph, IGRAPH_ATTRIBUTE_EDGE, "weight")) {
    EANV(&graph, "weight", &weights);
  } else {
    for (igraph_integer_t i = 0; i < igraph_ecount(&graph); i++) {
      VECTOR(weights)[i] = 1;
    }
  }

  plhs[0] = mxIgraphCreateAdj(&graph, &weights, adj_options);

  fclose(fptr);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);

  return errorcode;
}

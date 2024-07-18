#include <mxIgraph.h>
#include "utils.h"

igraph_error_t mexIgraphRewire(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* adj_options = prhs[1];
  mxArray const* method_options = prhs[2];

  igraph_t graph;
  igraph_bool_t directed = mxIgraphGetBool(method_options, "isdirected");
  igraph_bool_t preserve_degree = mxIgraphGetBool(method_options,
                                  "preserveDegree");
  igraph_real_t probability = mxIgraphGetReal(method_options, "probability");
  igraph_integer_t n_rewires = mxIgraphGetInteger(method_options, "nRewires");
  igraph_neimode_t mode = mxIgraphSelectMode(method_options);
  igraph_bool_t loops = mxIgraphGetBool(method_options, "loops");
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphGetGraph(prhs[0], &graph, NULL, directed);

  if (preserve_degree) {
    errorcode = igraph_rewire(&graph, n_rewires,
                              loops ? IGRAPH_REWIRING_SIMPLE_LOOPS :
                              IGRAPH_REWIRING_SIMPLE);
  } else {
    errorcode = igraph_rewire_directed_edges(&graph, probability, loops, mode);
  }

  plhs[0] = mxIgraphCreateAdj(&graph, NULL, adj_options);
  igraph_destroy(&graph);

  return errorcode;
}

#include <mxIgraph.h>
#include "utils.h"

igraph_error_t mexIgraphOptimalModularity(int nlhs, mxArray* plhs[], int nrhs,
    mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  mxArray const* graph_options = prhs[1];
  igraph_vector_t weights;
  igraph_real_t modularity;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphGetGraph(prhs[0], &graph, &weights, graph_options);

  igraph_community_optimal_modularity(&graph, &modularity, NULL,
                                      MXIGRAPH_WEIGHTS(&weights));

  plhs[0] = mxCreateDoubleScalar(modularity);

  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);

  return errorcode;
}

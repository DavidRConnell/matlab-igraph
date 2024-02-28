#include "mxIgraph.h"
#include "utils.h"

igraph_error_t mexIgraphOptimalModularity(int nlhs, mxArray* plhs[], int nrhs,
    mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(1);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  igraph_vector_t weights;
  igraph_real_t modularity;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphGetGraph(prhs[0], &graph, &weights, false);

  igraph_community_optimal_modularity(&graph, &modularity, NULL, &weights);

  plhs[0] = mxCreateDoubleScalar(modularity);

  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);

  return errorcode;
}

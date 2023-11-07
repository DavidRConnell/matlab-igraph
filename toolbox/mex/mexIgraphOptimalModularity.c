#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  VERIFY_N_INPUTS_EQUAL(1);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  igraph_vector_t weights;
  igraph_real_t modularity;

  mxIgraphGetGraph(prhs[0], &graph, &weights, false);

  igraph_community_optimal_modularity(&graph, &modularity, NULL, &weights);

  plhs[0] = mxCreateDoubleScalar(modularity);

  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
}

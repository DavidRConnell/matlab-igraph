#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_int_t membership;
  igraph_real_t resolution = mxGetScalar(prhs[2]);
  igraph_bool_t directed = mxGetScalar(prhs[3]);
  igraph_real_t modularity;

  mxIgraphGetGraph(prhs[0], &graph, &weights, directed);
  mxIgraphArrayToPartition(prhs[1], &membership);

  igraph_modularity(&graph, &membership, &weights, resolution,
                    directed, &modularity);

  plhs[0] = mxCreateDoubleScalar(modularity);

  igraph_vector_int_destroy(&membership);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
}

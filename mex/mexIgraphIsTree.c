#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  igraph_bool_t directed = mxGetScalar(prhs[1]);
  igraph_neimode_t mode = mxIgraphSelectMode(prhs[2]);
  igraph_integer_t root;
  igraph_bool_t find_root = mxGetScalar(prhs[3]);
  igraph_bool_t flag;

  mxIgraphGetGraph(prhs[0], &graph, NULL, directed);

  if (find_root) {
    igraph_is_tree(&graph, &flag, &root, mode);
    plhs[0] = mxCreateDoubleScalar(flag ? root + 1 : 0);
  } else {
    igraph_is_tree(&graph, &flag, NULL, mode);
    plhs[0] = mxCreateLogicalScalar(flag);
  }

  igraph_destroy(&graph);
}

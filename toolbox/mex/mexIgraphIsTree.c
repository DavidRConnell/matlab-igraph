#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  const igraph_integer_t n_modes = 4;
  // mode enum starts at 1 not 0.
  const char *modes[] = {
    [IGRAPH_IN - 1] = "in",
    [IGRAPH_OUT - 1] = "out",
    [IGRAPH_ALL - 1] = "all"
  };

  igraph_t graph;
  igraph_bool_t directed = mxGetScalar(prhs[1]);
  igraph_neimode_t mode = mxIgraphSelectMethod(prhs[2], modes, n_modes) + 1;
  igraph_integer_t root;
  igraph_bool_t find_root = mxGetScalar(prhs[3]);
  igraph_bool_t flag;

  if (mode == n_modes) {
    mexErrMsgIdAndTxt("Igraph:unknownMode",
                      "Unknown mode passed to %s.", mexFunctionName());
  }

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

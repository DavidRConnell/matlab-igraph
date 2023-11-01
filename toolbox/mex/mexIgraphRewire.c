#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  VERIFY_N_INPUTS_EQUAL(9);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  igraph_bool_t make_sparse = mxGetScalar(prhs[1]);
  mxIgraphDType_t dtype = mxIgraphSelectDType(prhs[2]);
  igraph_bool_t directed = mxGetScalar(prhs[3]);
  igraph_bool_t preserve_degree = mxGetScalar(prhs[4]);
  igraph_real_t probability = mxGetScalar(prhs[5]);
  igraph_integer_t n_rewires = mxGetScalar(prhs[6]);
  igraph_neimode_t mode;
  igraph_bool_t loops = mxGetScalar(prhs[8]);
  igraph_error_t errorcode;

  igraph_integer_t n_modes = 3;
  const char *modes[3] = {
    [IGRAPH_OUT - 1] = "out",
    [IGRAPH_IN - 1] = "in",
    [IGRAPH_ALL - 1] = "all"
  };

  mode = mxIgraphSelectMethod(prhs[7], modes, n_modes) + 1;
  mxIgraphGetGraph(prhs[0], &graph, NULL, directed);

  if (preserve_degree) {
    errorcode = igraph_rewire(&graph, n_rewires,
                              loops ? IGRAPH_REWIRING_SIMPLE_LOOPS :
                              IGRAPH_REWIRING_SIMPLE);
  } else {
    errorcode = igraph_rewire_directed_edges(&graph, probability, loops, mode);
  }

  if (errorcode != IGRAPH_SUCCESS) {
    igraph_destroy(&graph);
    switch (errorcode) {
    case IGRAPH_ENOMEM:
      mexErrMsgIdAndTxt("Igraph:outOfMemory",
                        "Failed to generate graph due to insufficient memory.");
      break;
    case IGRAPH_EINVAL:
      mexErrMsgIdAndTxt("Igraph:invalidParameter", "Invalid method parameter.");
      break;
    default:
      mexErrMsgIdAndTxt("Igraph:internal:generate",
                        "Failed to rewire graph.");
    }
  }

  plhs[0] = mxIgraphCreateAdj(&graph, NULL, make_sparse, dtype);

  igraph_destroy(&graph);
}

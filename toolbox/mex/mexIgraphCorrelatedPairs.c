#include <string.h>

#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  VERIFY_N_INPUTS_EQUAL(7);
  VERIFY_N_OUTPUTS_ATMOST(2);

  char *method = mxArrayToString(prhs[0]);
  igraph_bool_t make_sparse = mxGetScalar(prhs[1]);
  mxIgraphDType_t dtype = mxIgraphSelectDType(prhs[2]);
  igraph_real_t correlation = mxGetScalar(prhs[4]);
  igraph_real_t probability = mxGetScalar(prhs[5]);
  igraph_bool_t directed = mxGetScalar(prhs[6]);
  igraph_error_t errorcode;

  if (strcmp(method, "correlate") == 0) {
    igraph_t template;
    igraph_t result;

    mxIgraphGetGraph(prhs[3], &template, NULL, directed);

    errorcode = igraph_correlated_game(&template, &result, correlation,
                                       probability, NULL);

    if (errorcode != IGRAPH_SUCCESS) {
      goto error;
    }

    igraph_destroy(&template);

    plhs[0] = mxIgraphCreateAdj(&result, NULL, make_sparse, dtype);
    igraph_destroy(&result);
  } else { // method == "generate"
    igraph_t graph1;
    igraph_t graph2;
    igraph_integer_t n_nodes = mxGetScalar(prhs[3]);

    errorcode = igraph_correlated_pair_game(&graph1, &graph2, n_nodes,
                                            correlation, probability,
                                            directed, NULL);

    if (errorcode != IGRAPH_SUCCESS) {
      goto error;
    }

    plhs[0] = mxIgraphCreateAdj(&graph1, NULL, make_sparse, dtype);
    plhs[1] = mxIgraphCreateAdj(&graph2, NULL, make_sparse, dtype);

    igraph_destroy(&graph1);
    igraph_destroy(&graph2);
  }
  return;

error:
  switch (errorcode) {
  case IGRAPH_ENOMEM:
    mexErrMsgIdAndTxt("Igraph:outOfMemory",
                      "Failed to generate graph due to insufficient memory.");
    break;
  case IGRAPH_EINVAL:
    mexErrMsgIdAndTxt("Igraph:invalidParameter", "Invalid method parameter.");
    break;
  default:
    mexErrMsgIdAndTxt("Igraph:internal:correlatedPair",
                      "Failed to create graph.");
  }
}

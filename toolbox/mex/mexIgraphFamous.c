#include "mxIgraph.h"
#include "igraph.h"

#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetErrorHandler();

  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  char *name = mxArrayToString(prhs[0]);
  igraph_bool_t make_sparse = mxGetScalar(prhs[1]);
  mxIgraphDType_t dtype = mxIgraphSelectDType(prhs[2]);
  igraph_t graph;
  igraph_error_t errorcode;

  errorcode = igraph_famous(&graph, name);

  if (errorcode != IGRAPH_SUCCESS) {
    igraph_destroy(&graph);
    switch (errorcode) {
    case IGRAPH_ENOMEM:
      mexErrMsgIdAndTxt("Igraph:outOfMemory",
                        "Failed to get graph due to insufficient memory.");
      break;
    default:
      mexErrMsgIdAndTxt("Igraph:internal:famous",
                        "Failed to get %s graph.", name);
    }
  }

  plhs[0] = mxIgraphCreateAdj(&graph, NULL, make_sparse, dtype);

  igraph_destroy(&graph);
}

#include "mxIgraph.h"

#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetErrorHandler();

  VERIFY_N_INPUTS_EQUAL(1);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_bool_t flag = mxIgraphIsDirected(prhs[0]);
  plhs[0] = mxCreateLogicalScalar(flag);
}

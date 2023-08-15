#include "mxIgraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetErrorHandler();

  if (nrhs != 1) {
    mexErrMsgIdAndTxt("Igraph:wrongNumberOfInputs", "%s must have 1 input",
                      mexFunctionName());
  }

  if (nlhs != 1 && nlhs != 0) {
    mexErrMsgIdAndTxt("Igraph:wrongNumberOfOutputs", "%s must have 1 output",
                      mexFunctionName());
  }

  igraph_bool_t flag = mxIgraphIsDirected(prhs[0]);
  plhs[0] = mxCreateLogicalScalar(flag);
}

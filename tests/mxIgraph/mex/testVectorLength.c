#include "mxIgraph.h"
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  igraph_integer_t len = mxIgraphVectorLength(prhs[0]);

  plhs[0] = mxCreateDoubleScalar(len);
}

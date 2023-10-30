#include "mxIgraph.h"
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  igraph_bool_t flag = mxIgraphIsDirected(prhs[0]);

  plhs[0] = mxCreateLogicalScalar(flag);
}

#include <mxIgraph.h>
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  igraph_bool_t flag = mxIgraphIsVector(prhs[0]);

  plhs[0] = mxCreateLogicalScalar(flag);
}

#include "mxIgraph.h"
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  igraph_vector_int_t membership;

  mxIgraphArrayToPartition(prhs[0], &membership);
  plhs[0] = mxIgraphCreatePartition(&membership);

  igraph_vector_int_destroy(&membership);
}

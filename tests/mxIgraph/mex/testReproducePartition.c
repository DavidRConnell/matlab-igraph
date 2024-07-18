#include <mxIgraph.h>
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  igraph_vector_int_t membership;

  mxIgraphMembershipFromArray(prhs[0], &membership);
  plhs[0] = mxIgraphMembershipToArray(&membership);

  igraph_vector_int_destroy(&membership);
}

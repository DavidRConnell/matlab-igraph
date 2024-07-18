#include <mxIgraph.h>
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  igraph_integer_t n_edges = mxIgraphECount(prhs[0],
                             mxIgraphIsDirected(prhs[0]));

  plhs[0] = mxCreateDoubleScalar(n_edges);
}

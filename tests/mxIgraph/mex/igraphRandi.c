#include <mxIgraph.h>

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  mxIgraphSetRNG();

  igraph_integer_t l = mxGetScalar(prhs[0]);
  igraph_integer_t h = mxGetScalar(prhs[1]);
  igraph_integer_t x = igraph_rng_get_integer(igraph_rng_default(), l, h);

  plhs[0] = mxCreateDoubleScalar(x);
}

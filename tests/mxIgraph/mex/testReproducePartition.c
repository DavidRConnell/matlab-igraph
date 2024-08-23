#include <mxIgraph.h>
#include "igraph.h"

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  igraph_vector_int_t membership;

  mxIgraphVectorIntFromArray(prhs[0], & membership, true);
  plhs[0] = mxIgraphVectorIntToArray( & membership, true);

  igraph_vector_int_destroy( & membership);
}

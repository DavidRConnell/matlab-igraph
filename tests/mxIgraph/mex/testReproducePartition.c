#include "igraph.h"

#include <mxIgraph.h>

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  igraph_vector_int_t membership;

  mxIgraphVectorIntFromArray(prhs[0], &membership, MXIGRAPH_IDX_SHIFT);
  plhs[0] = mxIgraphVectorIntToArray(&membership, MXIGRAPH_IDX_SHIFT);

  igraph_vector_int_destroy(&membership);
}

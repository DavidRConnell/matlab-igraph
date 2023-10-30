#include "mxIgraph.h"
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  bool useSparse = mxGetScalar(prhs[1]);
  mxIgraphDType_t useLogical = mxGetScalar(prhs[2]);
  igraph_t graph;
  igraph_vector_t weights;

  mxIgraphGetGraph(prhs[0], &graph, &weights, mxIgraphIsDirected(prhs[0]));

  plhs[0] = mxIgraphCreateAdj(&graph, &weights, useSparse, useLogical);

  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
}

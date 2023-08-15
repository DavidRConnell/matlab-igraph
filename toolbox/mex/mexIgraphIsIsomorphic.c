#include "mxIgraph.h"
#include "igraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetErrorHandler();

  if (nrhs != 4) {
    mexErrMsgIdAndTxt("Igraph:internal:wrongNumberOfInputs",
                      "%s must have 4 inputs",
                      mexFunctionName());
  }

  if (nlhs != 1) {
    mexErrMsgIdAndTxt("Igraph:internal:wrongNumberOfOutputs",
                      "%s must have 1 output",
                      mexFunctionName());
  }

  igraph_bool_t adj1IsDirected = mxGetScalar(prhs[2]);
  igraph_bool_t adj2IsDirected = mxGetScalar(prhs[3]);
  igraph_t graph1, graph2;
  igraph_bool_t flag;

  mxIgraphGetGraph(prhs[0], &graph1, NULL, adj1IsDirected);
  mxIgraphGetGraph(prhs[1], &graph2, NULL, adj2IsDirected);

  igraph_isomorphic(&graph1, &graph2, &flag);
  igraph_destroy(&graph1);
  igraph_destroy(&graph2);

  plhs[0] = mxCreateLogicalScalar(flag);
}

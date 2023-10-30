#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

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

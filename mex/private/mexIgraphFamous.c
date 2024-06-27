#include <mxIgraph.h>
#include <utils.h>

#include <igraph_constructors.h>

igraph_error_t mexIgraphFamous(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_N_OUTPUTS_EQUAL(1);

  char* name = mxArrayToString(prhs[0]);
  mxArray const* adj_options = prhs[1];
  igraph_t graph;
  igraph_error_t errorcode;

  if ((errorcode = igraph_famous(&graph, name)) != IGRAPH_SUCCESS) {
    return errorcode;
  };

  plhs[0] = mxIgraphCreateAdj(&graph, NULL, adj_options);
  igraph_destroy(&graph);

  return errorcode;
}

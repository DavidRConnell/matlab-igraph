#include <mxIgraph.h>
#include <utils.h>

#include <igraph_structural.h>

igraph_error_t mexIgraphIsTree(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* opts = prhs[1];
  igraph_t graph;
  igraph_bool_t directed = mxIgraphGetBool(opts, "isdirected");
  igraph_neimode_t mode = mxIgraphSelectMode(opts);
  igraph_integer_t root;
  igraph_bool_t find_root = mxIgraphGetBool(opts, "findRoot");
  igraph_bool_t flag;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphGetGraph(prhs[0], &graph, NULL, directed);

  if (find_root) {
    igraph_is_tree(&graph, &flag, &root, mode);
    plhs[0] = mxCreateDoubleScalar(flag ? root + 1 : 0);
  } else {
    igraph_is_tree(&graph, &flag, NULL, mode);
    plhs[0] = mxCreateLogicalScalar(flag);
  }

  igraph_destroy(&graph);

  return errorcode;
}

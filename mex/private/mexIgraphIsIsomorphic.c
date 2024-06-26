#include <mxIgraph.h>
#include <utils.h>

#include <igraph_topology.h>

igraph_error_t mexIgraphIsIsomorphic(int nlhs, mxArray* plhs[], int nrhs,
                                     mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* adj1Options = prhs[2];
  mxArray const* adj2Options = prhs[3];
  igraph_bool_t adj1IsDirected = mxIgraphGetBool(adj1Options, "isdirected");
  igraph_bool_t adj2IsDirected = mxIgraphGetBool(adj2Options, "isdirected");
  igraph_t graph1, graph2;
  igraph_bool_t flag;

  mxIgraphGetGraph(prhs[0], &graph1, NULL, adj1IsDirected);
  mxIgraphGetGraph(prhs[1], &graph2, NULL, adj2IsDirected);

  igraph_isomorphic(&graph1, &graph2, &flag);
  igraph_destroy(&graph1);
  igraph_destroy(&graph2);

  plhs[0] = mxCreateLogicalScalar(flag);

  return IGRAPH_SUCCESS;
}

igraph_error_t mexIgraphIsSubIsomorphic(int nlhs, mxArray* plhs[], int nrhs,
                                        const mxArray* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* adj1Options = prhs[2];
  mxArray const* adj2Options = prhs[3];
  igraph_bool_t adj1IsDirected = mxIgraphGetBool(adj1Options, "isdirected");
  igraph_bool_t adj2IsDirected = mxIgraphGetBool(adj2Options, "isdirected");
  igraph_t graph1, graph2;
  igraph_bool_t flag;

  mxIgraphGetGraph(prhs[0], &graph1, NULL, adj1IsDirected);
  mxIgraphGetGraph(prhs[1], &graph2, NULL, adj2IsDirected);

  igraph_subisomorphic(&graph1, &graph2, &flag);
  igraph_destroy(&graph1);
  igraph_destroy(&graph2);

  plhs[0] = mxCreateLogicalScalar(flag);

  return IGRAPH_SUCCESS;
}

#include <string.h>

#include <mxIgraph.h>
#include <utils.h>

#include <igraph_games.h>

igraph_error_t mexIgraphCorrelateWith(int nlhs, mxArray* plhs[], int nrhs,
                                      const mxArray* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* adj_options = prhs[1];
  mxArray const* method_options = prhs[2];

  igraph_real_t correlation = mxIgraphGetReal(method_options, "correlation");
  igraph_real_t probability = mxIgraphGetReal(method_options, "density");
  igraph_bool_t directed = mxIgraphGetBool(method_options, "isdirected");
  igraph_error_t errorcode;

  igraph_t template;
  igraph_t result;

  mxIgraphGetGraph(prhs[0], &template, NULL, directed);

  errorcode = igraph_correlated_game(&template, &result, correlation,
                                     probability, NULL);

  if (errorcode != IGRAPH_SUCCESS) {
    return errorcode;
  }

  igraph_destroy(&template);

  plhs[0] = mxIgraphCreateAdj(&result, NULL, adj_options);
  igraph_destroy(&result);

  return errorcode;
}

igraph_error_t mexIgraphGeneratePair(int nlhs, mxArray* plhs[], int nrhs,
                                     const mxArray* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(2);

  mxArray const* adj_options = prhs[1];
  mxArray const* method_options = prhs[2];

  igraph_t graph1;
  igraph_t graph2;
  igraph_integer_t n_nodes = mxGetScalar(prhs[0]);
  igraph_real_t correlation = mxIgraphGetReal(method_options, "correlation");
  igraph_real_t probability = mxIgraphGetInteger(method_options, "density");
  igraph_bool_t directed = mxIgraphGetBool(method_options, "directed");
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  errorcode = igraph_correlated_pair_game(&graph1, &graph2, n_nodes,
                                          correlation, probability,
                                          directed, NULL);

  if (errorcode != IGRAPH_SUCCESS) {
    return errorcode;
  }

  plhs[0] = mxIgraphCreateAdj(&graph1, NULL, adj_options);
  plhs[1] = mxIgraphCreateAdj(&graph2, NULL, adj_options);

  igraph_destroy(&graph1);
  igraph_destroy(&graph2);
  return errorcode;
}

#include <mxIgraph.h>
#include "utils.h"

igraph_error_t mexIgraphCentrality(int nlhs, mxArray* plhs[], int nrhs,
                                   mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  enum {
    MXIGRAPH_CENTRALITY_CLOSENESS = 0,
    MXIGRAPH_CENTRALITY_HARMONIC,
    MXIGRAPH_CENTRALITY_BETWEENNESS,
    MXIGRAPH_CENTRALITY_PAGERANK,
    MXIGRAPH_CENTRALITY_BURT,
    /* MXIGRAPH_CENTRALITY_EIGENVECTOR, */
    MXIGRAPH_CENTRALITY_N
  };

  const char* methods[MXIGRAPH_CENTRALITY_N] = {
    [MXIGRAPH_CENTRALITY_CLOSENESS] = "closeness",
    [MXIGRAPH_CENTRALITY_HARMONIC] = "harmonic",
    [MXIGRAPH_CENTRALITY_BETWEENNESS] = "betweenness",
    [MXIGRAPH_CENTRALITY_PAGERANK] = "pagerank",
    [MXIGRAPH_CENTRALITY_BURT] = "burt"
    /* [MXIGRAPH_CENTRALITY_EIGENVECTOR] = "eigenvector" */
  };

  mxArray const* opts = prhs[2];
  igraph_integer_t method = mxIgraphSelectMethod(prhs[1], methods,
                            MXIGRAPH_CENTRALITY_N);
  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_t res;
  igraph_vector_int_t vertices;
  igraph_vs_t vids;
  igraph_bool_t warning;
  igraph_neimode_t mode = mxIgraphSelectMode(opts);
  igraph_bool_t directed = mxIgraphGetBool(opts, "isdirected");
  igraph_bool_t normalized = mxIgraphGetBool(opts, "normalized");
  igraph_real_t damping = mxIgraphGetReal(opts, "damping");
  igraph_error_t errorcode;

  mxIgraphGetGraph(prhs[0], &graph, &weights, directed);
  mxIgraphGetVectorInt(opts, "vids", &vertices, false);
  igraph_vs_vector(&vids, &vertices);

  igraph_vector_init(&res, 0);

  switch (method) {
  case MXIGRAPH_CENTRALITY_CLOSENESS:
    errorcode = igraph_closeness(&graph, &res, NULL, &warning, vids, mode,
                                 &weights, normalized);

    if (warning) {
      mexWarnMsgTxt("Graph not connected; not all nodes could reach "
                    "all other nodes.");
    }
    break;
  case MXIGRAPH_CENTRALITY_HARMONIC:
    errorcode = igraph_harmonic_centrality(&graph, &res, vids, mode, &weights,
                                           normalized);
    break;
  case MXIGRAPH_CENTRALITY_BETWEENNESS:
    errorcode = igraph_betweenness(&graph, &res, vids, directed, &weights);
    break;
  case MXIGRAPH_CENTRALITY_PAGERANK:
    errorcode = igraph_pagerank(&graph, IGRAPH_PAGERANK_ALGO_PRPACK, &res, NULL,
                                vids, directed, damping, &weights, NULL);
    break;
  case MXIGRAPH_CENTRALITY_BURT:
    errorcode = igraph_constraint(&graph, &res, vids, &weights);
    break;
  /* case MXIGRAPH_CENTRALITY_EIGENVECTOR: */
  /*   errorcode = igraph_eigenvector_centrality(&graph, &res, NULL, directed, */
  /*               normalized, &weights, NULL); */
  /*   break; */
  default:
    mexErrMsgIdAndTxt("Igraph:internal:unknownMethod",
                      "Centrality method %s not known.",
                      mxArrayToString(prhs[0]));
    exit(1);
  }

  igraph_vs_destroy(&vids);
  igraph_vector_int_destroy(&vertices);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);

  plhs[0] = mxIgraphVectorToArray(&res, false);
  igraph_vector_destroy(&res);

  return errorcode;
}

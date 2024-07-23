#include <mxIgraph.h>
#include "utils.h"

igraph_error_t mexIgraphCentrality(int nlhs, mxArray* plhs[], int nrhs,
                                   mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  enum {
    MXIGRAPH_CENTRALITY_CLOSENESS = 0,
    MXIGRAPH_CENTRALITY_HARMONIC,
    MXIGRAPH_CENTRALITY_BETWEENNESS,
    MXIGRAPH_CENTRALITY_PAGERANK,
    MXIGRAPH_CENTRALITY_BURT,
    MXIGRAPH_CENTRALITY_EIGENVECTOR,
    MXIGRAPH_CENTRALITY_N
  };

  const char* methods[MXIGRAPH_CENTRALITY_N] = {
    [MXIGRAPH_CENTRALITY_CLOSENESS] = "closeness",
    [MXIGRAPH_CENTRALITY_HARMONIC] = "harmonic",
    [MXIGRAPH_CENTRALITY_BETWEENNESS] = "betweenness",
    [MXIGRAPH_CENTRALITY_PAGERANK] = "pagerank",
    [MXIGRAPH_CENTRALITY_BURT] = "burt",
    [MXIGRAPH_CENTRALITY_EIGENVECTOR] = "eigenvector"
  };

  igraph_integer_t method = mxIgraphSelectMethod(prhs[1], methods,
                            MXIGRAPH_CENTRALITY_N);
  mxArray const* graph_options = prhs[2];
  mxArray const* method_options = prhs[3];

  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_t res;
  igraph_vector_int_t vertices;
  igraph_vs_t vids;
  igraph_bool_t warning;
  igraph_bool_t const directed = mxIgraphGetBool(graph_options, "isdirected");
  igraph_neimode_t const mode = mxIgraphSelectMode(method_options);
  igraph_bool_t const normalized = mxIgraphGetBool(method_options,
                                   "normalized");
  igraph_real_t const damping = mxIgraphGetReal(method_options, "damping");
  igraph_error_t errorcode;

  mxIgraphGetGraph(prhs[0], &graph, &weights, graph_options);
  mxIgraphGetVectorInt(method_options, "vids", &vertices, true);
  igraph_vs_vector(&vids, &vertices);

  igraph_vector_init(&res, 0);

  switch (method) {
  case MXIGRAPH_CENTRALITY_CLOSENESS:
    errorcode = igraph_closeness(&graph, &res, NULL, &warning, vids, mode,
                                 MXIGRAPH_WEIGHTS(&weights), normalized);

    if (warning) {
      mexWarnMsgTxt("Graph not connected; not all nodes could reach "
                    "all other nodes.");
    }
    break;
  case MXIGRAPH_CENTRALITY_HARMONIC:
    errorcode = igraph_harmonic_centrality(&graph, &res, vids, mode,
                                           MXIGRAPH_WEIGHTS(&weights),
                                           normalized);
    break;
  case MXIGRAPH_CENTRALITY_BETWEENNESS:
    errorcode = igraph_betweenness(&graph, &res, vids, directed,
                                   MXIGRAPH_WEIGHTS(&weights));
    break;
  case MXIGRAPH_CENTRALITY_PAGERANK:
    errorcode = igraph_pagerank(&graph, IGRAPH_PAGERANK_ALGO_PRPACK, &res, NULL,
                                vids, directed, damping,
                                MXIGRAPH_WEIGHTS(&weights), NULL);
    break;
  case MXIGRAPH_CENTRALITY_BURT:
    errorcode = igraph_constraint(&graph, &res, vids,
                                  MXIGRAPH_WEIGHTS(&weights));
    break;
  case MXIGRAPH_CENTRALITY_EIGENVECTOR:
    mxIgraphErrorNotImplemented("Centrality", "eigenvector");
    /*   errorcode = igraph_eigenvector_centrality(&graph, &res, NULL, directed, */
    /*               normalized, MXIGRAPH_WEIGHTS(&weights), NULL); */
    break;
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

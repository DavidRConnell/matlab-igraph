/* Copyright 2024 David R. Connell <david32@dcon.addy.io>.
 *
 * This file is part of matlab-igraph.
 *
 * matlab-igraph is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * matlab-igraph is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with matlab-igraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "utils.h"
#include <mxIgraph.h>

igraph_error_t mexIgraphCentrality(int nlhs, mxArray *plhs[], int nrhs,
                                   mxArray const *prhs[])
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

  const char *methods[MXIGRAPH_CENTRALITY_N] = {
    [MXIGRAPH_CENTRALITY_CLOSENESS] = "closeness",
    [MXIGRAPH_CENTRALITY_HARMONIC] = "harmonic",
    [MXIGRAPH_CENTRALITY_BETWEENNESS] = "betweenness",
    [MXIGRAPH_CENTRALITY_PAGERANK] = "pagerank",
    [MXIGRAPH_CENTRALITY_BURT] = "burt",
    [MXIGRAPH_CENTRALITY_EIGENVECTOR] = "eigenvector"
  };

  igraph_integer_t method =
    mxIgraphSelectMethod(prhs[1], methods, MXIGRAPH_CENTRALITY_N);
  MXIGRAPH_CHECK_METHOD(method, prhs[1]);

  mxArray const *graph_options = prhs[2];
  mxArray const *method_options = prhs[3];

  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_t res;
  igraph_vector_int_t vertices;
  igraph_vs_t vids;
  igraph_bool_t warning;
  igraph_bool_t const directed =
    mxIgraphBoolFromOptions(graph_options, "isdirected");
  igraph_neimode_t const mode = mxIgraphModeFromOptions(method_options);
  igraph_bool_t const normalized =
    mxIgraphBoolFromOptions(method_options, "normalized");
  igraph_real_t const damping =
    mxIgraphRealFromOptions(method_options, "damping");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(mxIgraphFromArray(prhs[0], &graph, &weights, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph);
  IGRAPH_FINALLY(igraph_vector_destroy, &weights);
  IGRAPH_CHECK(
    mxIgraphVectorIntFromOptions(method_options, "vids", &vertices, true));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &vertices);
  IGRAPH_CHECK(igraph_vs_vector(&vids, &vertices));
  IGRAPH_FINALLY(igraph_vs_destroy, &vids);

  IGRAPH_CHECK(igraph_vector_init(&res, 0));
  IGRAPH_FINALLY(igraph_vector_destroy, &res);

  switch (method) {
  case MXIGRAPH_CENTRALITY_CLOSENESS:
    IGRAPH_CHECK(igraph_closeness(&graph, &res, NULL, &warning, vids, mode,
                                  MXIGRAPH_WEIGHTS(&weights), normalized));

    if (warning) {
      IGRAPH_WARNING("Graph not connected; not all nodes could reach "
                     "all other nodes.");
    }
    break;
  case MXIGRAPH_CENTRALITY_HARMONIC:
    IGRAPH_CHECK(igraph_harmonic_centrality(
                   &graph, &res, vids, mode, MXIGRAPH_WEIGHTS(&weights), normalized));
    break;
  case MXIGRAPH_CENTRALITY_BETWEENNESS:
    IGRAPH_CHECK(igraph_betweenness(&graph, &res, vids, directed,
                                    MXIGRAPH_WEIGHTS(&weights)));
    break;
  case MXIGRAPH_CENTRALITY_PAGERANK:
    IGRAPH_CHECK(igraph_pagerank(&graph, IGRAPH_PAGERANK_ALGO_PRPACK, &res,
                                 NULL, vids, directed, damping,
                                 MXIGRAPH_WEIGHTS(&weights), NULL));
    break;
  case MXIGRAPH_CENTRALITY_BURT:
    IGRAPH_CHECK(
      igraph_constraint(&graph, &res, vids, MXIGRAPH_WEIGHTS(&weights)));
    break;
  case MXIGRAPH_CENTRALITY_EIGENVECTOR:
    IGRAPH_ERROR("Eigenvector centrality not implemented",
                 IGRAPH_UNIMPLEMENTED);
    /* errorcode = igraph_eigenvector_centrality(&graph, &res, NULL, directed,
     */
    /*             normalized, MXIGRAPH_WEIGHTS(&weights), NULL); */
    break;
  default:
    IGRAPH_FATAL("Got unexpected method name.");
  }

  igraph_vs_destroy(&vids);
  igraph_vector_int_destroy(&vertices);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
  IGRAPH_FINALLY_CLEAN(4);

  plhs[0] = mxIgraphVectorToArray(&res, MXIGRAPH_IDX_KEEP);
  igraph_vector_destroy(&res);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

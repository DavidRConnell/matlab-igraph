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

igraph_error_t mexIgraphRewire(int nlhs, mxArray *plhs[], int nrhs,
                               mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *graph_options = prhs[1];
  mxArray const *method_options = prhs[2];

  igraph_t graph;
  igraph_bool_t const preserve_degree =
    mxIgraphBoolFromOptions(method_options, "preserveDegree");
  igraph_real_t const probability =
    mxIgraphRealFromOptions(method_options, "probability");
  igraph_integer_t const n_rewires =
    mxIgraphIntegerFromOptions(method_options, "nRewires");
  igraph_neimode_t const mode = mxIgraphModeFromOptions(method_options);
  igraph_bool_t const loops = mxIgraphBoolFromOptions(method_options, "loops");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(mxIgraphFromArray(prhs[0], &graph, NULL, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph);

  if (preserve_degree) {
    IGRAPH_CHECK(igraph_rewire(&graph, n_rewires,
                               loops ? IGRAPH_REWIRING_SIMPLE_LOOPS
                               : IGRAPH_REWIRING_SIMPLE));
  } else {
    IGRAPH_CHECK(
      igraph_rewire_directed_edges(&graph, probability, loops, mode));
  }

  plhs[0] = mxIgraphToArray(&graph, NULL, graph_options);

  igraph_destroy(&graph);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

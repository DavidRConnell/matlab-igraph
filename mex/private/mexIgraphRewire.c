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

#include <mxIgraph.h>
#include "utils.h"

igraph_error_t mexIgraphRewire(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* graph_options = prhs[1];
  mxArray const* method_options = prhs[2];

  igraph_t graph;
  igraph_bool_t const preserve_degree = mxIgraphGetBool(method_options,
                                        "preserveDegree");
  igraph_real_t const probability = mxIgraphGetReal(method_options,
                                    "probability");
  igraph_integer_t const n_rewires = mxIgraphGetInteger(method_options,
                                     "nRewires");
  igraph_neimode_t const mode = mxIgraphSelectMode(method_options);
  igraph_bool_t const loops = mxIgraphGetBool(method_options, "loops");
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphGetGraph(prhs[0], &graph, NULL, graph_options);

  if (preserve_degree) {
    errorcode = igraph_rewire(&graph, n_rewires,
                              loops ? IGRAPH_REWIRING_SIMPLE_LOOPS :
                              IGRAPH_REWIRING_SIMPLE);
  } else {
    errorcode = igraph_rewire_directed_edges(&graph, probability, loops, mode);
  }

  plhs[0] = mxIgraphCreateGraph(&graph, NULL, graph_options);

  igraph_destroy(&graph);

  return errorcode;
}

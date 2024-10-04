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

igraph_error_t mexIgraphModularity(
  int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* graph_options = prhs[2];
  mxArray const* method_options = prhs[3];

  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_int_t membership;
  igraph_real_t resolution =
    mxIgraphRealFromOptions(method_options, "resolution");
  igraph_bool_t directed =
    mxIgraphBoolFromOptions(graph_options, "isdirected");
  igraph_real_t modularity;
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(mxIgraphFromArray(prhs[0], &graph, &weights, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph);
  IGRAPH_FINALLY(igraph_vector_destroy, &weights);
  IGRAPH_CHECK(
    mxIgraphVectorIntFromArray(prhs[1], &membership, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &membership);

  IGRAPH_CHECK(igraph_modularity(&graph, &membership,
    MXIGRAPH_WEIGHTS(&weights), resolution, directed, &modularity));

  plhs[0] = mxCreateDoubleScalar(modularity);

  igraph_vector_int_destroy(&membership);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
  IGRAPH_FINALLY_CLEAN(3);

  return IGRAPH_SUCCESS;
}

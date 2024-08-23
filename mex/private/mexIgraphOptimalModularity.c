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

igraph_error_t mexIgraphOptimalModularity(int nlhs, mxArray *plhs[], int nrhs,
    mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  mxArray const *graph_options = prhs[1];
  igraph_vector_t weights;
  igraph_real_t modularity;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphGetGraph(prhs[0], &graph, &weights, graph_options);

  igraph_community_optimal_modularity(&graph, &modularity, NULL,
                                      MXIGRAPH_WEIGHTS(&weights));

  plhs[0] = mxCreateDoubleScalar(modularity);

  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);

  return errorcode;
}

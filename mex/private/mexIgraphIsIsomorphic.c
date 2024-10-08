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

igraph_error_t mexIgraphIsIsomorphic(int nlhs, mxArray *plhs[], int nrhs,
                                     mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *graph_options = prhs[2];
  igraph_t graph1, graph2;
  igraph_bool_t flag;

  IGRAPH_CHECK(mxIgraphFromArray(prhs[0], &graph1, NULL, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph1);
  IGRAPH_CHECK(mxIgraphFromArray(prhs[1], &graph2, NULL, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph2);

  IGRAPH_CHECK(igraph_isomorphic(&graph1, &graph2, &flag));
  igraph_destroy(&graph1);
  igraph_destroy(&graph2);
  IGRAPH_FINALLY_CLEAN(2);

  plhs[0] = mxCreateLogicalScalar(flag);

  return IGRAPH_SUCCESS;
}

igraph_error_t mexIgraphIsSubIsomorphic(int nlhs, mxArray *plhs[], int nrhs,
                                        const mxArray *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *graph_options = prhs[2];
  igraph_t graph1, graph2;
  igraph_bool_t flag;

  IGRAPH_CHECK(mxIgraphFromArray(prhs[0], &graph1, NULL, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph1);
  IGRAPH_CHECK(mxIgraphFromArray(prhs[1], &graph2, NULL, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph2);

  IGRAPH_CHECK(igraph_subisomorphic(&graph1, &graph2, &flag));
  igraph_destroy(&graph1);
  igraph_destroy(&graph2);
  IGRAPH_FINALLY_CLEAN(2);

  plhs[0] = mxCreateLogicalScalar(flag);

  return IGRAPH_SUCCESS;
}

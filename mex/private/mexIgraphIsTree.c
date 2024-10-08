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

igraph_error_t mexIgraphIsTree(int nlhs, mxArray *plhs[], int nrhs,
                               mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *opts = prhs[1];
  igraph_t graph;
  igraph_neimode_t mode = mxIgraphModeFromOptions(opts);
  igraph_bool_t find_root = mxIgraphBoolFromOptions(opts, "findRoot");
  MXIGRAPH_CHECK_STATUS();
  igraph_integer_t root;
  igraph_bool_t flag;

  IGRAPH_CHECK(mxIgraphFromArray(prhs[0], &graph, NULL, opts));
  IGRAPH_FINALLY(igraph_destroy, &graph);

  if (find_root) {
    IGRAPH_CHECK(igraph_is_tree(&graph, &flag, &root, mode));
    plhs[0] = mxCreateDoubleScalar(flag ? root + 1 : 0);
  } else {
    IGRAPH_CHECK(igraph_is_tree(&graph, &flag, NULL, mode));
    plhs[0] = mxCreateLogicalScalar(flag);
  }

  igraph_destroy(&graph);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

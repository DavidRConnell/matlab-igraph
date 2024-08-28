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

#include <string.h>

#include "utils.h"
#include <mxIgraph.h>

igraph_error_t mexIgraphCompare(int nlhs, mxArray *plhs[], int nrhs,
                                mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *method_name = prhs[2];
  igraph_vector_int_t comm1;
  igraph_vector_int_t comm2;
  igraph_community_comparison_t method;
  igraph_real_t res;

  const char *methods[] = {[IGRAPH_COMMCMP_VI] = "vi",
                           [IGRAPH_COMMCMP_NMI] = "nmi",
                           [IGRAPH_COMMCMP_RAND] = "rand",
                           [IGRAPH_COMMCMP_SPLIT_JOIN] = "split_join",
                           [IGRAPH_COMMCMP_ADJUSTED_RAND] = "adjusted_rand"
                          };
  igraph_integer_t n_methods = sizeof(methods) / sizeof(*methods);
  method = mxIgraphSelectMethod(method_name, methods, n_methods);
  MXIGRAPH_CHECK_METHOD(method, method_name);

  IGRAPH_CHECK(mxIgraphVectorIntFromArray(prhs[0], &comm1, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &comm1);
  IGRAPH_CHECK(mxIgraphVectorIntFromArray(prhs[1], &comm2, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &comm2);

  IGRAPH_CHECK(igraph_reindex_membership(&comm1, NULL, NULL));
  IGRAPH_CHECK(igraph_reindex_membership(&comm2, NULL, NULL));

  IGRAPH_CHECK(igraph_compare_communities(&comm1, &comm2, &res, method));
  igraph_vector_int_destroy(&comm1);
  igraph_vector_int_destroy(&comm2);
  IGRAPH_FINALLY_CLEAN(2);

  plhs[0] = mxCreateDoubleScalar(res);

  return IGRAPH_SUCCESS;
}

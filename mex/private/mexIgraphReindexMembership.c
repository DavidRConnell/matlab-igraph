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

static igraph_error_t mxIgraph_reindex_membership_i(igraph_vector_int_t* memb)
{
  igraph_integer_t const comm_min = igraph_vector_int_min(memb);
  igraph_integer_t const n_comms =
    igraph_vector_int_max(memb) - igraph_vector_int_min(memb) + 1;
  igraph_vector_int_t ids;

  IGRAPH_CHECK(igraph_vector_int_init(&ids, n_comms));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &ids);
  for (igraph_integer_t i = 0; i < igraph_vector_int_size(memb); i++) {
    VECTOR(*memb)[i] -= comm_min;
    VECTOR(ids)[VECTOR(*memb)[i]]++;
  }

  igraph_integer_t shift = 0;
  for (igraph_integer_t i = 0; i < igraph_vector_int_size(&ids); i++) {
    if (VECTOR(ids)[i] == 0) {
      shift++;
    } else {
      VECTOR(ids)[i] = shift;
    }
  }

  for (igraph_integer_t i = 0; i < igraph_vector_int_size(memb); i++) {
    VECTOR(*memb)[i] -= VECTOR(ids)[VECTOR(*memb)[i]];
  }

  igraph_vector_int_destroy(&ids);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

igraph_error_t mexIgraphReindexMembership(
  int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(1);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_matrix_int_t membership;
  igraph_vector_int_t level_memb;

  IGRAPH_CHECK(
    mxIgraphMatrixIntFromArray(prhs[0], &membership, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_matrix_int_destroy, &membership);
  IGRAPH_CHECK(
    igraph_vector_int_init(&level_memb, igraph_matrix_int_ncol(&membership)));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &level_memb);
  for (igraph_integer_t i = 0; i < igraph_matrix_int_nrow(&membership); i++) {
    IGRAPH_CHECK(igraph_matrix_int_get_row(&membership, &level_memb, i));
    IGRAPH_CHECK(mxIgraph_reindex_membership_i(&level_memb));
    IGRAPH_CHECK(igraph_matrix_int_set_row(&membership, &level_memb, i));
  }
  igraph_vector_int_destroy(&level_memb);

  plhs[0] = mxIgraphMatrixIntToArray(&membership, MXIGRAPH_IDX_SHIFT);
  igraph_matrix_int_destroy(&membership);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

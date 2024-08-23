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

static void mxIgraph_reindex_membership_i(igraph_vector_int_t *memb)
{
  igraph_integer_t const comm_min = igraph_vector_int_min(memb);
  igraph_integer_t const n_comms =
    igraph_vector_int_max(memb) - igraph_vector_int_min(memb) + 1;
  igraph_vector_int_t ids;

  igraph_vector_int_init(&ids, n_comms);
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
}

igraph_error_t mexIgraphReindexMembership(int nlhs, mxArray *plhs[], int nrhs,
    mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(1);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_matrix_int_t membership;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphMatrixIntFromArray(prhs[0], &membership, true);

  igraph_vector_int_t level_memb;
  igraph_vector_int_init(&level_memb, igraph_matrix_int_ncol(&membership));
  for (igraph_integer_t i = 0; i < igraph_matrix_int_nrow(&membership); i++) {
    igraph_matrix_int_get_row(&membership, &level_memb, i);
    mxIgraph_reindex_membership_i(&level_memb);
    igraph_matrix_int_set_row(&membership, &level_memb, i);
  }
  igraph_vector_int_destroy(&level_memb);

  plhs[0] = mxIgraphMatrixIntToArray(&membership, true);
  igraph_matrix_int_destroy(&membership);

  return errorcode;
}

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
#include <string.h>

enum {
  MXIGRAPH_GENERATOR_MATLAB = 0,
  MXIGRAPH_GENERATOR_MT19937,
  MXIGRAPH_GENERATOR_GLIBC2,
  MXIGRAPH_GENERATOR_PCG32,
  MXIGRAPH_GENERATOR_PCG64,
  MXIGRAPH_GENERATOR_N
};

igraph_rng_t mxIgraph_rng;
char const *mxIgraph_rng_current_generator = "matlab";

igraph_error_t mexIgraphRNG(int nlhs, mxArray *plhs[], int nrhs,
                            mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_integer_t const seed = mxGetScalar(prhs[0]);
  mxArray const *generator_name = prhs[1];
  igraph_integer_t generator;
  igraph_error_t errcode = IGRAPH_SUCCESS;

  const char *generators[MXIGRAPH_GENERATOR_N] = {
    [MXIGRAPH_GENERATOR_MATLAB] = "matlab",
    [MXIGRAPH_GENERATOR_MT19937] = "mt19937",
    [MXIGRAPH_GENERATOR_GLIBC2] = "glibc2",
    [MXIGRAPH_GENERATOR_PCG32] = "pcg32",
    [MXIGRAPH_GENERATOR_PCG64] = "pcg64"
  };

  /* When a generator name is not passed to the MATLAB calling function, the
     string in generator_name will not be one of the known generators in the
     above generator table, leading to the variable generator being set to -1.
     In this case, the default rng is not set but is still seeded. */
  generator =
    mxIgraphSelectMethod(generator_name, generators, MXIGRAPH_GENERATOR_N);

  if (generator >= 0) {
    mxIgraph_rng_current_generator = generators[generator];
  }
  plhs[0] = mxCreateString(mxIgraph_rng_current_generator);

  switch (generator) {
  case MXIGRAPH_GENERATOR_MT19937:
    errcode = igraph_rng_init(&mxIgraph_rng, &igraph_rngtype_mt19937);
    break;
  case MXIGRAPH_GENERATOR_GLIBC2:
    errcode = igraph_rng_init(&mxIgraph_rng, &igraph_rngtype_glibc2);
    break;
  case MXIGRAPH_GENERATOR_PCG32:
    errcode = igraph_rng_init(&mxIgraph_rng, &igraph_rngtype_pcg32);
    break;
  case MXIGRAPH_GENERATOR_PCG64:
    errcode = igraph_rng_init(&mxIgraph_rng, &igraph_rngtype_pcg64);
    break;
  }

  if (errcode != IGRAPH_SUCCESS) {
    IGRAPH_ERROR("Failed to initialize random number generator.", errcode);
  }

  if (generator == MXIGRAPH_GENERATOR_MATLAB) {
    mxIgraphSetRNG();
    /* MATLAB RNG should only be seeded using the MATLAB rng function, so
    return before seeding. */
    return IGRAPH_SUCCESS;
  } else if (generator >= 0) {
    igraph_rng_set_default(&mxIgraph_rng);
  } else if (strcmp(mxIgraph_rng_current_generator, "matlab") == 0) {
    return IGRAPH_SUCCESS;
  }

  errcode = igraph_rng_seed(igraph_rng_default(), seed);

  if (errcode != IGRAPH_SUCCESS) {
    IGRAPH_ERROR("Failed to seed random numeber generator.", errcode);
  }

  return IGRAPH_SUCCESS;
}

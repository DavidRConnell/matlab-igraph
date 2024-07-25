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

/* Wrap MATLAB's internal RNG for use in igraph.

Depends on functions in the mwmathrng library that are not made public. This
could break in future versions of MATLAB if the exported functions change. */

#include <mxIgraph.h>

void mrRandu(double* arr, size_t len);
void mrRandi(double* arr, size_t len, double l, double h);
void mrRandn(double* arr, size_t len);

static igraph_rng_t mxIgraph_rng_instance = {
  .type = 0,
  .state = 0,
  .is_seeded = 1
};

static igraph_error_t mxIgraph_rng_init(void** state)
{
  // Nothing to do, MATLAB already initialized it.
  return IGRAPH_SUCCESS;
}

static void mxIgraph_rng_destroy(void* state)
{
  // Again nothing to do; MATLAB handles it.
}

static igraph_error_t mxIgraph_rng_seed(void* state, igraph_uint_t seed)
{
  IGRAPH_ERROR("MATLAB RNG error, unsupported function called",
               IGRAPH_EINTERNAL);
  return IGRAPH_SUCCESS;
}

igraph_real_t mxIgraph_rng_get_real(void* state)
{
  double x = 0;
  mrRandu( & x, 1);

  return x;
}

static igraph_uint_t mxIgraph_rng_get(void* state)
{
  return (mxIgraph_rng_get_real(state) * 0x40000000UL);
}

igraph_integer_t mxIgraph_rng_get_integer(void* state, igraph_integer_t l,
    igraph_integer_t h)
{
  double x = 0;
  mrRandi( & x, 1, l, h);

  return x;
}

static igraph_real_t mxIgraph_rng_get_normal(void* state)
{
  double x = 0;
  mrRandn( & x, 1);

  return x;
}

static igraph_rng_type_t mxIgraph_rng_type = {
  /* name= */      "MATLAB",
  /* bits = */     32,
  /* init= */      mxIgraph_rng_init,
  /* destroy= */   mxIgraph_rng_destroy,
  /* seed= */      mxIgraph_rng_seed,
  /* get= */       mxIgraph_rng_get,
  /* get_int= */   mxIgraph_rng_get_integer,
  /* get_real= */  mxIgraph_rng_get_real,
  /* get_norm= */  mxIgraph_rng_get_normal,
  /* get_geom= */  NULL,
  /* get_binom= */ NULL,
  /* get_exp= */   NULL,
  /* get_gamma= */ NULL,
  /* get_pois= */  NULL,
};

void mxIgraphSetRNG(void)
{
  /* For some reason it seems the MATLAB rng function needs to be touched if it
  hasn't been otherwise MATLAB crashes. This does not modify the RNG. */
  mexCallMATLAB(0, NULL, 0, NULL, "rng");
  igraph_rng_init( & mxIgraph_rng_instance, & mxIgraph_rng_type);
  igraph_rng_set_default( & mxIgraph_rng_instance);
}

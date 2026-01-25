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

/* On all OSes this library is linked against the libmwmathrng C++ library.
   This library is private so there is no header file for it so we must define
   the internal function headers here. On Windows, there is expected to be a
   .lib import library with symbol information in addition to the actual .dll
   library with the definitions. But since the library is not public, the
   import is not provided so we must create a stub import library with the
   headers we need (generated in the CMakeLists.txt). For simplicity, we define
   the headers with plain names and therefore need the extern "C" to tell the
   C++ linker these functions are used by C (they aren't they are only used in
   this C++ file), do not mangle them. On Linux and macOS only the one dynamic
   library is needed, and this is compiled by C++ with mangled names. Because
   of that we need the C++ compiler to mangle the names here as well. */
#if defined(_WIN32) && defined(__cplusplus)
extern "C" {
#endif // defined(_WIN32) && defined(__cplusplus)

  void mrRandu(double* arr, size_t len);
  void mrRandi(double* arr, size_t len, double l, double h);
  void mrRandn(double* arr, size_t len);

#if defined(_WIN32) && defined(__cplusplus)
}
#endif // defined(_WIN32) && defined(__cplusplus)

static igraph_rng_t mxIgraph_rng_instance = {
  .type = 0, .state = 0, .is_seeded = 1
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
  IGRAPH_ERROR(
    "MATLAB RNG error, unsupported function called", IGRAPH_EINTERNAL);
  return IGRAPH_SUCCESS;
}

igraph_real_t mxIgraph_rng_get_real(void* state)
{
  double x = 0;
  mrRandu(&x, 1);

  return x;
}

static igraph_uint_t mxIgraph_rng_get(void* state)
{
  return (mxIgraph_rng_get_real(state) * 0x40000000UL);
}

static igraph_integer_t mxIgraph_rng_get_integer(
  void* state, igraph_integer_t l, igraph_integer_t h)
{
  double x = 0;
  mrRandi(&x, 1, l, h);

  return x;
}

static igraph_real_t mxIgraph_rng_get_normal(void* state)
{
  double x = 0;
  mrRandn(&x, 1);

  return x;
}

static igraph_rng_type_t mxIgraph_rng_type = {
  /* name= */ "MATLAB",
  /* bits = */ 32,
  /* init= */ mxIgraph_rng_init,
  /* destroy= */ mxIgraph_rng_destroy,
  /* seed= */ mxIgraph_rng_seed,
  /* get= */ mxIgraph_rng_get,
  /* get_int= */ mxIgraph_rng_get_integer,
  /* get_real= */ mxIgraph_rng_get_real,
  /* get_norm= */ NULL,
  /* get_geom= */ NULL,
  /* get_binom= */ NULL,
  /* get_exp= */ NULL,
  /* get_gamma= */ NULL,
  /* get_pois= */ NULL,
};

void mxIgraphSetRNG(void)
{
  /* For some reason it seems the MATLAB rng function needs to be touched if it
  hasn't been otherwise MATLAB crashes. This does not modify the RNG. */
  mexCallMATLAB(0, NULL, 0, NULL, "rng");
  igraph_rng_init(&mxIgraph_rng_instance, &mxIgraph_rng_type);
  igraph_rng_set_default(&mxIgraph_rng_instance);
}

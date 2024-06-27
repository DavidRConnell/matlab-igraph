#include <mxIgraph.h>
#include <utils.h>

#include <igraph_random.h>

enum {
  MXIGRAPH_GENERATOR_MT19937 = 0,
  MXIGRAPH_GENERATOR_GLIBC2,
  MXIGRAPH_GENERATOR_PCG32,
  MXIGRAPH_GENERATOR_PCG64,
  MXIGRAPH_GENERATOR_N
};

igraph_error_t mexIgraphRNG(int nlhs, mxArray* plhs[], int nrhs,
                            mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_NO_OUTPUTS;

  igraph_integer_t seed = mxGetScalar(prhs[0]);
  igraph_integer_t generator;
  igraph_rng_t new_rng;
  igraph_error_t errcode = IGRAPH_SUCCESS;

  const char* generators[MXIGRAPH_GENERATOR_N] = {
    [MXIGRAPH_GENERATOR_MT19937] = "mt19937",
    [MXIGRAPH_GENERATOR_GLIBC2] = "glibc2",
    [MXIGRAPH_GENERATOR_PCG32] = "pcg32",
    [MXIGRAPH_GENERATOR_PCG64] = "pcg64"
  };
  generator = mxIgraphSelectMethod(prhs[1], generators, MXIGRAPH_GENERATOR_N);

  switch (generator) {
  case MXIGRAPH_GENERATOR_MT19937:
    errcode = igraph_rng_init(&new_rng, &igraph_rngtype_mt19937);
    break;
  case MXIGRAPH_GENERATOR_GLIBC2:
    errcode = igraph_rng_init(&new_rng, &igraph_rngtype_glibc2);
    break;
  case MXIGRAPH_GENERATOR_PCG32:
    errcode = igraph_rng_init(&new_rng, &igraph_rngtype_pcg32);
    break;
  case MXIGRAPH_GENERATOR_PCG64:
    errcode = igraph_rng_init(&new_rng, &igraph_rngtype_pcg64);
    break;
  }

  if (generator < MXIGRAPH_GENERATOR_N) {
    igraph_rng_set_default(&new_rng);
  }

  if (errcode != IGRAPH_SUCCESS) {
    mexErrMsgIdAndTxt("Igraph:internal:incorrectRandomGenerator",
                      "Failed to initialize a new RNG.");
  }

  errcode = igraph_rng_seed(igraph_rng_default(), seed);

  if (errcode != IGRAPH_SUCCESS) {
    mexErrMsgIdAndTxt("Igraph:internal:improperSeed",
                      "Failed to seed the RNG.");
  }

  return errcode;
}

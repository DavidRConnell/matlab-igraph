#ifndef UTILS_H
#define UTILS_H

#include "mex.h"

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#define VERIFY_N_INPUTS_EQUAL(expected)				\
  do {								\
    if (nrhs != (expected)) {					\
      mexErrMsgIdAndTxt("Igraph:internal:nrhs",			\
			"%s expected %d inputs got %d.",	\
			mexFunctionName(), (expected), nrhs);	\
    }								\
  } while (0)

#define VERIFY_NO_INPUTS				\
  if (nrhs != 0) {					\
    mexErrMsgIdAndTxt("Igraph:internal:nrhs",		\
		      "%s expected no inputs got %d.",	\
		      mexFunctionName(), nrhs);		\
  }							\
  (void)prhs

#define VERIFY_N_INPUTS_ATLEAST(bound)					\
  do {									\
    if (nrhs < (bound)) {						\
      mexErrMsgIdAndTxt("Igraph:internal:nrhs",				\
			"%s expected atleast %d inputs got %d.",	\
			mexFunctionName(), (bound), nrhs);		\
    }									\
  } while (0)

#define VERIFY_N_INPUTS_ATMOST(bound)				\
  do {								\
    if (nrhs > (bound)) {					\
      mexErrMsgIdAndTxt("Igraph:internal:nrhs",			\
			"%s expected atmost %d inputs got %d.",	\
			mexFunctionName(), (bound), nrhs);	\
    }								\
  } while (0)

#define VERIFY_N_INPUTS_BETWEEN(lower, upper)				\
  do {									\
    if ((nrhs < (lower)) || (nrhs > (upper))) {				\
      mexErrMsgIdAndTxt("Igraph:internal:nrhs",				\
			"%s expected between %d and %d inputs got %d.",	\
			mexFunctionName(), (lower), (upper), nrhs);	\
    }									\
  } while (0)

#define VERIFY_N_OUTPUTS_EQUAL(expected)			\
  do {								\
    if (nlhs != (expected)) {					\
      mexErrMsgIdAndTxt("Igraph:internal:nlhs",			\
			"%s expected %d outputs got %d.",	\
			mexFunctionName(), (expected), nlhs);	\
    }								\
  } while (0)

#define VERIFY_NO_OUTPUTS				\
  if (nlhs != 0) {					\
    mexErrMsgIdAndTxt("Igraph:internal:nlhs",		\
		      "%s expected no outputs got %d.",	\
		      mexFunctionName(), nlhs);		\
  }							\
  (void)plhs						\

#define VERIFY_N_OUTPUTS_ATLEAST(bound)					\
  do {									\
    if (nlhs < (bound)) {						\
      mexErrMsgIdAndTxt("Igraph:internal:nlhs",				\
			"%s expected atleast %d outputs got %d.",	\
			mexFunctionName(), (bound), nlhs);		\
    }									\
  } while (0)

#define VERIFY_N_OUTPUTS_ATMOST(bound)					\
  do {									\
    if (nlhs > (bound)) {						\
      mexErrMsgIdAndTxt("Igraph:internal:nlhs",				\
			"%s expected atmost %d outputs got %d.",	\
			mexFunctionName(), (bound), nlhs);		\
    }									\
  } while (0)

#define VERIFY_N_OUTPUTS_BETWEEN(lower, upper)				\
  do {									\
    if ((nlhs < (lower)) || (nrhs > (upper))) {				\
      mexErrMsgIdAndTxt("Igraph:internal:nlhs",				\
			"%s expected between %d and %d outputs got %d.", \
			mexFunctionName(), (lower), (upper), nlhs);	\
    }									\
  } while (0)

#endif

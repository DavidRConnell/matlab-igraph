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

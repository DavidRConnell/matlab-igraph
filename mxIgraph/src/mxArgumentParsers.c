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
#include <string.h>

/* Generic string selector.
   Find which of a predefined set of strings an input value matches.
   Returns the index, which should correspond to an enum that can be used in a
   switch statement.

   If no match found, returns -1. */
igraph_integer_t mxIgraphSelectMethod(const mxArray *p, const char *methods[],
                                      const igraph_integer_t n_methods)
{
  if (!mxIsChar(p)) {
    mxIgraphSetError(IGRAPH_EINVAL);
    mxIgraphSetErrorMsg("Method name must be specified as a string.");
    return -1;
  }

  char *name = mxArrayToString(p);
  for (igraph_integer_t i = 0; i < n_methods; i++) {
    if (methods[i] && strcmp(name, methods[i]) == 0) {
      return i;
    }
  }

  mxIgraphSetError(IGRAPH_EINVAL);
  mxIgraphSetErrorMsg("Recieved unexpected method name \"%s\".", name);
  return -1;
}

// Select a file format.
mxIgraphFileFormat_t mxIgraphSelectFileFormat(const mxArray *p)
{
  mxIgraphFileFormat_t res;
  const char *file_formats[MXIGRAPH_FORMAT_N] = {
    [MXIGRAPH_FORMAT_EDGELIST] = "edgelist",
    [MXIGRAPH_FORMAT_NCOL] = "ncol",
    [MXIGRAPH_FORMAT_LGL] = "lgl",
    [MXIGRAPH_FORMAT_DIMACS] = "dimacs",
    [MXIGRAPH_FORMAT_GRAPHDB] = "graphdb",
    [MXIGRAPH_FORMAT_GRAPHML] = "graphml",
    [MXIGRAPH_FORMAT_GML] = "gml",
    [MXIGRAPH_FORMAT_PAJEK] = "pajek",
    [MXIGRAPH_FORMAT_DL] = "dl",
    [MXIGRAPH_FORMAT_DOT] = "dot",
    [MXIGRAPH_FORMAT_LEDA] = "leda"
  };

  return mxIgraphSelectMethod(p, file_formats, MXIGRAPH_FORMAT_N);
}

// Select a graph representation.
mxIgraphRepr_t mxIgraphReprFromOptions(const mxArray *p)
{
  mxIgraphRepr_t res;
  const char *reps[MXIGRAPH_REPR_N] = {[MXIGRAPH_REPR_GRAPH] = "graph",
                                       [MXIGRAPH_REPR_FULL] = "full",
                                       [MXIGRAPH_REPR_SPARSE] = "sparse"
                                      };

  return mxIgraphSelectMethod(mxGetField(p, 0, "repr"), reps, MXIGRAPH_REPR_N);
}

// Select a data type.
mxIgraphDType_t mxIgraphDTypeFromOptions(const mxArray *p)
{
  mxIgraphDType_t res;
  const char *dtypes[MXIGRAPH_DTYPE_N] = {
    [MXIGRAPH_DTYPE_LOGICAL] = "logical", [MXIGRAPH_DTYPE_DOUBLE] = "double"
  };

  return mxIgraphSelectMethod(mxGetField(p, 0, "dtype"), dtypes,
                              MXIGRAPH_DTYPE_N);
}

// Select a neighbor mode.
igraph_neimode_t mxIgraphModeFromOptions(const mxArray *p)
{
  igraph_neimode_t res;
  igraph_integer_t n_modes = 3;
  // Mode enum starts at 1 not 0.
  const char *modes[3] = {[IGRAPH_OUT - 1] = "out",
                          [IGRAPH_IN - 1] = "in",
                          [IGRAPH_ALL - 1] = "all"
                         };

  res = mxIgraphSelectMethod(mxGetField(p, 0, "mode"), modes, n_modes);

  if (res == -1) {
    mxIgraphSetError(IGRAPH_EINVMODE);
    mxIgraphSetErrorMsg("Received invalid mode name \"%s\".",
                        mxGetField(p, 0, "mode"));
  }

  return res + 1;
}

mxArray *mxIgraphGetArgument(mxArray const *arg_struct,
                             char const *fieldname)
{
  mxArray *arg = NULL;
  if (!(arg = mxGetField(arg_struct, 0, fieldname))) {
    mxIgraphSetError(IGRAPH_EINVAL);
    mxIgraphSetErrorMsg("Received invalid argument name \"%s\".", fieldname);
  }

  return arg;
}

igraph_integer_t mxIgraphIntegerFromOptions(const mxArray *arg_struct,
    char const fieldname[1])
{
  mxArray *arg = mxIgraphGetArgument(arg_struct, fieldname);
  return arg ? mxGetScalar(arg) : -1;
}

igraph_real_t mxIgraphRealFromOptions(const mxArray *arg_struct,
                                      char const fieldname[1])
{
  mxArray *arg = mxIgraphGetArgument(arg_struct, fieldname);
  return arg ? mxGetScalar(arg) : -1;
}

igraph_bool_t mxIgraphBoolFromOptions(const mxArray *arg_struct,
                                      char const fieldname[1])
{
  mxArray *arg = mxIgraphGetArgument(arg_struct, fieldname);
  return arg ? mxGetScalar(arg) : 0;
}

char *mxIgraphStringFromOptions(mxArray const *arg_struct,
                                char const fieldname[1])
{
  mxArray *arg = mxIgraphGetArgument(arg_struct, fieldname);
  return arg ? mxArrayToString(arg) : "";
}

igraph_error_t mxIgraphVectorFromOptions(const mxArray *arg_struct,
    char const fieldname[1],
    igraph_vector_t *vec,
    igraph_bool_t const shift_start)
{
  return mxIgraphVectorFromArray(mxIgraphGetArgument(arg_struct, fieldname),
                                 vec, shift_start);
}

igraph_error_t mxIgraphVectorIntFromOptions(const mxArray *arg_struct,
    char const fieldname[1],
    igraph_vector_int_t *vec,
    igraph_bool_t const shift_start)
{
  return mxIgraphVectorIntFromArray(mxIgraphGetArgument(arg_struct, fieldname),
                                    vec, shift_start);
}

igraph_error_t mxIgraphVectorBoolFromOptions(const mxArray *arg_struct,
    char const fieldname[1],
    igraph_vector_bool_t *vec,
    igraph_bool_t const shift_start)
{
  return mxIgraphVectorBoolFromArray(mxIgraphGetArgument(arg_struct, fieldname),
                                     vec, shift_start);
}

igraph_error_t mxIgraphMatrixFromOptions(const mxArray *arg_struct,
    char const fieldname[1],
    igraph_matrix_t *mat,
    igraph_bool_t const shift_start)
{
  return mxIgraphMatrixFromArray(mxIgraphGetArgument(arg_struct, fieldname),
                                 mat, shift_start);
}

igraph_error_t mxIgraphMatrixIntFromOptions(const mxArray *arg_struct,
    char const fieldname[1],
    igraph_matrix_int_t *mat,
    igraph_bool_t const shift_start)
{
  return mxIgraphMatrixIntFromArray(mxIgraphGetArgument(arg_struct, fieldname),
                                    mat, shift_start);
}

igraph_error_t mxIgraphMatrixBoolFromOptions(const mxArray *arg_struct,
    char const fieldname[1],
    igraph_matrix_bool_t *mat,
    igraph_bool_t const shift_start)
{
  return mxIgraphMatrixBoolFromArray(mxIgraphGetArgument(arg_struct, fieldname),
                                     mat, shift_start);
}

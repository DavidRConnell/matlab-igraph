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

#include <mxIgraph.h>

/* Generic string selector.
   Find which of a predefined set of strings an input value matches.
   Returns the index, which should correspond to an enum that can be used in a
   switch statement.

   If no match found, returns `n_methods`. */
igraph_integer_t mxIgraphSelectMethod(const mxArray* p, const char* methods[],
                                      const igraph_integer_t n_methods)
{
  if (!mxIsChar(p)) {
    mexErrMsgIdAndTxt("Igraph:notChar",
                      "Method name must be specified as a string.");
  }
  char* name = mxArrayToString(p);

  for (igraph_integer_t i = 0; i < n_methods; i++) {
    if (methods[i] && strcmp(name, methods[i]) == 0) {
      return i;
    }
  }

  return n_methods;
}

// Select a file format.
mxIgraphFileFormat_t mxIgraphSelectFileFormat(const mxArray* p)
{
  mxIgraphFileFormat_t res;
  const char* file_formats[MXIGRAPH_FORMAT_N] = {
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

  res = mxIgraphSelectMethod(p, file_formats, MXIGRAPH_FORMAT_N);
  if (res == MXIGRAPH_FORMAT_N) {
    mexErrMsgIdAndTxt("Igraph:unknownFileFormat",
                      "Received unknown file format.");
  }

  return res;
}

// Select a graph representation.
mxIgraphRepr_t mxIgraphSelectRepr(const mxArray* p)
{
  mxIgraphRepr_t res;
  const char* reps[MXIGRAPH_REPR_N] = {
    [MXIGRAPH_REPR_GRAPH] = "graph",
    [MXIGRAPH_REPR_FULL] = "full",
    [MXIGRAPH_REPR_SPARSE] = "sparse"
  };

  res = mxIgraphSelectMethod(mxGetField(p, 0, "repr"), reps,
                             MXIGRAPH_REPR_N);

  if (res == MXIGRAPH_REPR_N) {
    mexErrMsgIdAndTxt("Igraph:unknownRep",
                      "Received unknown graph representation.");
  }

  return res;
}

// Select a data type.
mxIgraphDType_t mxIgraphSelectDType(const mxArray* p)
{
  mxIgraphDType_t res;
  const char* dtypes[MXIGRAPH_DTYPE_N] = {
    [MXIGRAPH_DTYPE_LOGICAL] = "logical",
    [MXIGRAPH_DTYPE_DOUBLE] = "double"
  };

  res = mxIgraphSelectMethod(mxGetField(p, 0, "dtype"), dtypes,
                             MXIGRAPH_DTYPE_N);
  if (res == MXIGRAPH_DTYPE_N) {
    mexErrMsgIdAndTxt("Igraph:unknownDType", "Received unknown data type.");
  }

  return res;
}

// Select a neighbor mode.
igraph_neimode_t mxIgraphSelectMode(const mxArray* p)
{
  igraph_neimode_t res;
  igraph_integer_t n_modes = 3;
  // Mode enum starts at 1 not 0.
  const char* modes[3] = {
    [IGRAPH_OUT - 1] = "out",
    [IGRAPH_IN - 1] = "in",
    [IGRAPH_ALL - 1] = "all"
  };

  res = mxIgraphSelectMethod(mxGetField(p, 0, "mode"), modes, n_modes);

  if (res == n_modes) {
    mexErrMsgIdAndTxt("Igraph:unknownMode", "Received unknown mode type.");
  }

  return res + 1;
}

mxArray* mxIgraphGetArgument(mxArray const* arg_struct,
                             char const* fieldname)
{
  mxArray* arg = NULL;
  if (!(arg = mxGetField(arg_struct, 0, fieldname))) {
    mexErrMsgIdAndTxt("Igraph:internal:missingArgument",
                      "Function was not passed an argument named %s", fieldname);
  }

  return arg;
}

igraph_integer_t mxIgraphGetInteger(const mxArray* arg_struct,
                                    char const fieldname[1])
{
  return mxGetScalar(mxIgraphGetArgument(arg_struct, fieldname));
}

igraph_real_t mxIgraphGetReal(const mxArray* arg_struct,
                              char const fieldname[1])
{
  return mxGetScalar(mxIgraphGetArgument(arg_struct, fieldname));
}

igraph_bool_t mxIgraphGetBool(const mxArray* arg_struct,
                              char const fieldname[1])
{
  return mxGetScalar(mxIgraphGetArgument(arg_struct, fieldname));
}

char* mxIgraphGetString(mxArray const* arg_struct, char const fieldname[1])
{
  return mxArrayToString(mxIgraphGetArgument(arg_struct, fieldname));
}

void mxIgraphGetVector(const mxArray* arg_struct, char const fieldname[1],
                       igraph_vector_t* vec, igraph_bool_t const shift_start)
{
  mxIgraphVectorFromArray(mxIgraphGetArgument(arg_struct, fieldname), vec,
                          shift_start);
}

void mxIgraphGetVectorInt(const mxArray* arg_struct, char const fieldname[1],
                          igraph_vector_int_t* vec,
                          igraph_bool_t const shift_start)
{
  mxIgraphVectorIntFromArray(mxIgraphGetArgument(arg_struct, fieldname), vec,
                             shift_start);
}

void mxIgraphGetVectorBool(const mxArray* arg_struct, char const fieldname[1],
                           igraph_vector_bool_t* vec,
                           igraph_bool_t const shift_start)
{
  mxIgraphVectorBoolFromArray(mxIgraphGetArgument(arg_struct, fieldname), vec,
                              shift_start);
}

void mxIgraphGetMatrix(const mxArray* arg_struct, char const fieldname[1],
                       igraph_matrix_t* mat, igraph_bool_t const shift_start)
{
  mxIgraphMatrixFromArray(mxIgraphGetArgument(arg_struct, fieldname), mat,
                          shift_start);
}

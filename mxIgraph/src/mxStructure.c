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

/* Functions for converting between MATLAB and igraph data types. Since C uses
   0-based and MATLAB 1-based indices, it is often necessary to shift values up
   when when going from C to MATLAB types or down 1 when going the other way.
   For this the @a shift_start argument can be set to true. */

#include <mxIgraph.h>

/* Copy a matlab vector to an igraph real vector.

 The igraph vector should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphVectorFromArray(const mxArray* p, igraph_vector_t* vec,
                            igraph_bool_t const shift_start)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_vector_init(vec, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIgraphIsVector(p)) {
    mexErrMsgIdAndTxt("igraph:notVector", "Input should be a vector.");
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("igraph:notDouble", "Input vector should be double.");
  }

  mxDouble* const x_mat = mxGetDoubles(p);
  igraph_integer_t const n = mxIgraphVectorLength(p);
  igraph_integer_t const shift = shift_start ? 1 : 0;

  igraph_vector_init(vec, n);
  for (igraph_integer_t i = 0; i < n; i++) {
    VECTOR(*vec)[i] = (igraph_real_t)x_mat[i] - shift;
  }

  return EXIT_SUCCESS;
}

/* Create a matlab vector from an igraph vector. */
mxArray* mxIgraphVectorToArray(igraph_vector_t const* vec,
                               igraph_bool_t const shift_start)
{
  igraph_integer_t const len = igraph_vector_size(vec);
  igraph_integer_t const shift = shift_start ? 1 : 0;
  mxArray* p = mxCreateDoubleMatrix(1, len, mxREAL);
  double* mxVector = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < len; i++) {
    mxVector[(mwIndex)i] = (double)VECTOR(*vec)[i] + shift;
  }

  return p;
}

/* Copy a matlab vector to an igraph integer vector.

 The igraph vector should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphVectorIntFromArray(const mxArray* p, igraph_vector_int_t* vec,
                               igraph_bool_t const shift_start)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_vector_int_init(vec, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIgraphIsVector(p)) {
    mexErrMsgIdAndTxt("igraph:notVector", "Input should be a vector.");
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("igraph:notDouble", "Input vector should be double.");
  }

  mxDouble* x_mat = mxGetDoubles(p);
  igraph_integer_t const shift = shift_start ? 1 : 0;
  igraph_integer_t const n = mxIgraphVectorLength(p);

  igraph_vector_int_init(vec, n);
  for (igraph_integer_t i = 0; i < n; i++) {
    VECTOR(*vec)[i] = (igraph_integer_t)x_mat[i] - shift;
  }

  return EXIT_SUCCESS;
}

/* Create a matlab vector from an igraph integer vector. */
mxArray* mxIgraphVectorIntToArray(igraph_vector_int_t const* vec,
                                  igraph_bool_t const shift_start)
{
  igraph_integer_t const shift = shift_start ? 1 : 0;
  igraph_integer_t const len = igraph_vector_int_size(vec);
  mxArray* p = mxCreateDoubleMatrix(1, len, mxREAL);
  double* mxVector = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < len; i++) {
    mxVector[(mwIndex)i] = (double)VECTOR(*vec)[i] + shift;
  }

  return p;
}

/* Copy a matlab vector to an igraph boolean vector.

 The igraph vector should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphVectorBoolFromArray(const mxArray* p, igraph_vector_bool_t* vec,
                                igraph_bool_t const shift_start)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_vector_bool_init(vec, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIgraphIsVector(p)) {
    mexErrMsgIdAndTxt("Igraph:NotVector", "Input should be a vector.");
  }

  if (!mxIsLogical(p)) {
    mexErrMsgIdAndTxt("Igraph:NotDouble", "Input vector should be logical.");
  }

  mxLogical* x_mat = mxGetLogicals(p);
  igraph_integer_t const n = mxIgraphVectorLength(p);
  igraph_integer_t const shift = shift_start ? 1 : 0;

  igraph_vector_bool_init(vec, n);
  for (igraph_integer_t i = 0; i < n; i++) {
    VECTOR(*vec)[i] = (igraph_bool_t)x_mat[i] - shift;
  }

  return EXIT_SUCCESS;
}

/* Create a matlab vector from an igraph boolean vector. */
mxArray* mxIgraphVectorBoolToArray(igraph_vector_bool_t const* vec,
                                   igraph_bool_t const shift_start)
{
  igraph_integer_t const len = igraph_vector_bool_size(vec);
  igraph_integer_t const shift = shift_start ? 1 : 0;
  mxArray* p = mxCreateLogicalMatrix(1, len);
  bool* mxVector = mxGetLogicals(p);

  for (igraph_integer_t i = 0; i < len; i++) {
    mxVector[(mwIndex)i] = VECTOR(*vec)[i] + shift;
  }

  return p;
}

/* Copy a matlab matrix to an igraph matrix.

 The igraph matrix should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphMatrixFromArray(const mxArray* p, igraph_matrix_t* mat,
                            igraph_bool_t const shift_start)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_matrix_init(mat, 0, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("Igraph:NotDouble", "Input matrix should be double.");
  }

  mxDouble* x_mat = mxGetDoubles(p);
  igraph_integer_t const m = mxGetM(p);
  igraph_integer_t const n = mxGetN(p);
  igraph_integer_t const shift = shift_start ? 1 : 0;

  igraph_matrix_init(mat, m, n);
  for (igraph_integer_t i = 0; i < m; i++) {
    for (igraph_integer_t j = 0; j < n; j++) {
      MATRIX(*mat, i, j) = (igraph_real_t)x_mat[i + (j * m)] - shift;
    }
  }

  return EXIT_SUCCESS;
}

/* Create a Matlab matrix from an igraph matrix. */
mxArray* mxIgraphMatrixToArray(igraph_matrix_t const* mat,
                               igraph_bool_t const shift_start)
{
  igraph_integer_t const shift = shift_start ? 1 : 0;
  igraph_integer_t const m = igraph_matrix_nrow(mat);
  igraph_integer_t const n = igraph_matrix_ncol(mat);
  mxArray* p = mxCreateDoubleMatrix(m, n, mxREAL);
  double* mxMat = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < m; i++) {
    for (igraph_integer_t j = 0; j < n; j++) {
      mxMat[(mwIndex)i + (mwIndex)(j * m)] = MATRIX(*mat, i, j) + shift;
    }
  }

  return p;
}

/* Copy a matlab matrix to an igraph integer matrix.

 The igraph matrix should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphMatrixIntFromArray(const mxArray* p, igraph_matrix_int_t* mat,
                               igraph_bool_t const shift_start)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_matrix_int_init(mat, 0, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("Igraph:NotDouble", "Input matrix should be double.");
  }

  mxDouble* x_mat = mxGetDoubles(p);
  igraph_integer_t const m = mxGetM(p);
  igraph_integer_t const n = mxGetN(p);
  igraph_integer_t const shift = shift_start ? 1 : 0;

  igraph_matrix_int_init(mat, m, n);
  for (igraph_integer_t i = 0; i < m; i++) {
    for (igraph_integer_t j = 0; j < n; j++) {
      MATRIX(*mat, i, j) = (igraph_real_t)x_mat[i + (j * m)] - shift;
    }
  }

  return EXIT_SUCCESS;
}

/* Create a Matlab matrix from an igraph integer matrix. */
mxArray* mxIgraphMatrixIntToArray(igraph_matrix_int_t const* mat,
                                  igraph_bool_t const shift_start)
{
  igraph_integer_t const shift = shift_start ? 1 : 0;
  igraph_integer_t const m = igraph_matrix_int_nrow(mat);
  igraph_integer_t const n = igraph_matrix_int_ncol(mat);
  mxArray* p = mxCreateDoubleMatrix(m, n, mxREAL);
  double* mxMat = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < m; i++) {
    for (igraph_integer_t j = 0; j < n; j++) {
      mxMat[(mwIndex)i + (mwIndex)(j * m)] = MATRIX(*mat, i, j) + shift;
    }
  }

  return p;
}

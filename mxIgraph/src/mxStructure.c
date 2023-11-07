#include "mxIgraph.h"

/* Copy a matlab vector to an igraph real vector.

 The igraph vector should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphGetVector(const mxArray *p, igraph_vector_t *vec)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_vector_init(vec, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIgraphIsVector(p)) {
    mexErrMsgIdAndTxt("Igraph:NotVector", "Input should be a vector.");
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("Igraph:NotDouble", "Input vector should be double.");
  }

  mxDouble *x_mat = mxGetDoubles(p);
  igraph_integer_t n = mxIgraphVectorLength(p);

  igraph_vector_init(vec, n);
  for (igraph_integer_t i = 0; i < n; i++) {
    VECTOR(*vec)[i] = (igraph_real_t)x_mat[i];
  }

  return EXIT_SUCCESS;
}

/* Create a matlab vector from an igraph vector. */
mxArray *mxIgraphCreateVector(igraph_vector_t const *vec)
{
  igraph_integer_t len = igraph_vector_size(vec);
  mxArray *p = mxCreateDoubleMatrix(1, len, mxREAL);
  double *mxVector = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < len; i++) {
    mxVector[(mwIndex)i] = (double)VECTOR(*vec)[i];
  }

  return p;
}

/* Copy a matlab vector to an igraph integer vector.

 The igraph vector should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphGetVectorInt(const mxArray *p, igraph_vector_int_t *vec)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_vector_int_init(vec, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIgraphIsVector(p)) {
    mexErrMsgIdAndTxt("Igraph:NotVector", "Input should be a vector.");
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("Igraph:NotDouble", "Input vector should be double.");
  }

  mxDouble *x_mat = mxGetDoubles(p);
  igraph_integer_t n = mxIgraphVectorLength(p);

  igraph_vector_int_init(vec, n);
  for (igraph_integer_t i = 0; i < n; i++) {
    VECTOR(*vec)[i] = (igraph_integer_t)x_mat[i];
  }

  return EXIT_SUCCESS;
}

/* Create a matlab vector from an igraph integer vector. */
mxArray *mxIgraphCreateVectorInt(igraph_vector_int_t const *vec)
{
  igraph_integer_t len = igraph_vector_int_size(vec);
  mxArray *p = mxCreateDoubleMatrix(1, len, mxREAL);
  double *mxVector = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < len; i++) {
    mxVector[(mwIndex)i] = (double)VECTOR(*vec)[i];
  }

  return p;
}

/* Copy a matlab vector to an igraph boolean vector.

 The igraph vector should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphGetVectorBool(const mxArray *p, igraph_vector_bool_t *vec)
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

  mxLogical *x_mat = mxGetLogicals(p);
  igraph_integer_t n = mxIgraphVectorLength(p);

  igraph_vector_bool_init(vec, n);
  for (igraph_integer_t i = 0; i < n; i++) {
    VECTOR(*vec)[i] = (igraph_bool_t)x_mat[i];
  }

  return EXIT_SUCCESS;
}

/* Create a matlab vector from an igraph boolean vector. */
mxArray *mxIgraphCreateVectorBool(igraph_vector_bool_t const *vec)
{
  igraph_integer_t len = igraph_vector_bool_size(vec);
  mxArray *p = mxCreateLogicalMatrix(1, len);
  bool *mxVector = mxGetLogicals(p);

  for (igraph_integer_t i = 0; i < len; i++) {
    mxVector[(mwIndex)i] = VECTOR(*vec)[i];
  }

  return p;
}

/* Copy a matlab matrix to an igraph matrix.

 The igraph matrix should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphGetMatrix(const mxArray *p, igraph_matrix_t *mat)
{
  if (mxIgraphIsEmpty(p)) {
    igraph_matrix_init(mat, 0, 0);
    return EXIT_SUCCESS;
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("Igraph:NotDouble", "Input matrix should be double.");
  }

  mxDouble *x_mat = mxGetDoubles(p);
  igraph_integer_t m = mxGetM(p);
  igraph_integer_t n = mxGetN(p);

  igraph_matrix_init(mat, m, n);
  for (igraph_integer_t i = 0; i < m; i++) {
    for (igraph_integer_t j = 0; j < n; j++) {
      MATRIX(*mat, i, j) = (igraph_real_t)x_mat[i + (j * m)];
    }
  }

  return EXIT_SUCCESS;
}

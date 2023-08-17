#include "mxIgraph.h"
#include "igraph_interface.h"

/* Return the length of the vector pointed to by p. */
igraph_integer_t mxIgraphVectorLength(const mxArray *p)
{
  mwSize n = mxGetN(p);
  mwSize m = mxGetM(p);

  if ((n != 1) && (m != 1)) {
    mexErrMsgIdAndTxt("Igraph:NotVector",
                      "Inputs should be a vector not a matrix");
  }

  return n > m ? n : m;
}

/* Copy a matlab vector to an igraph vector.

 The igraph vector should be uninitialized, but it's the callers responsibility
 to destroy it when done. */
int mxIgraphGetVectorInt(const mxArray *p, igraph_vector_int_t *vec)
{
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

static int mxIgraphGetPartitionFromCell(const mxArray *p,
                                        igraph_vector_int_t *membership)
{
  mexErrMsgIdAndTxt("Igraph:NotImplemented",
                    "Getting a partition from a cell format has not been implemented.");

  return EXIT_FAILURE;
}

static int mxIgraphGetPartitionFromDouble(const mxArray *p,
    igraph_vector_int_t *membership)
{
  if (!(mxIgraphIsVector(p))) {
    mexErrMsgIdAndTxt("Igraph:NotVector", "Partition must be a vector.");
  }

  return mxIgraphGetVectorInt(p, membership);
}

/* Copy a matlab partition to an igraph vector.

   If matlab partition starts with node id 1 instead of node id 0, all values
   will be reduced such that the smallest node id is 0.

   Membership should be uninitialized, but it's the callers responsibility to
   destroy it when done.*/
int mxIgraphArrayToPartition(const mxArray *p,
                             igraph_vector_int_t *membership)
{
  int rc = EXIT_FAILURE;

  if (mxIsCell(p)) {
    rc = mxIgraphGetPartitionFromCell(p, membership);
  } else if (mxIsDouble(p)) {
    rc = mxIgraphGetPartitionFromDouble(p, membership);
  }

  igraph_integer_t min_id = igraph_vector_int_min(membership);
  if (min_id != 0) {
    for (igraph_integer_t i = 0; i < igraph_vector_int_size(membership); i++) {
      VECTOR(*membership)[i] -= min_id;
    }
  }

  if (rc) {
    mexErrMsgIdAndTxt("Igraph:NotPartition",
                      "Value not a valid partition format.");
  }

  return rc;
}

/* Create a matlab vector for storing the labels of each node from an igraph
   membership partition.

   Increments igraph partition so that the smallest node id is 1 instead of 0.*/
mxArray *mxIgraphCreatePartition(igraph_vector_int_t const *membership)
{
  igraph_integer_t n_nodes = igraph_vector_int_size(membership);
  mxArray *p = mxCreateDoubleMatrix(1, n_nodes, mxREAL);
  double *mxMembership = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < n_nodes; i++) {
    mxMembership[(mwIndex)i] = (double)VECTOR(*membership)[i] + 1;
  }

  return p;
}

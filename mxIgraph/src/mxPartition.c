#include "mxIgraph.h"

/* Return the length of the vector pointed to by p. */
igraph_integer_t mxIgraphVectorLength(const mxArray* p)
{
  mwSize n = mxGetN(p);
  mwSize m = mxGetM(p);

  if ((n != 1) && (m != 1)) {
    mexErrMsgIdAndTxt("Igraph:NotVector",
                      "Inputs should be a vector not a matrix");
  }

  return n > m ? n : m;
}

static int mxIgraphGetPartitionFromDouble(const mxArray* p,
    igraph_vector_int_t* membership)
{
  if (!(mxIgraphIsVector(p))) {
    mexErrMsgIdAndTxt("Igraph:NotVector", "Partition must be a vector.");
  }

}

/* Copy a matlab partition to an igraph vector.

   If matlab partition starts with node id 1 instead of node id 0, all values
   will be reduced such that the smallest node id is 0.

   Membership should be uninitialized, but it's the callers responsibility to
   destroy it when done.*/
int mxIgraphMembershipFromArray(const mxArray* p,
                                igraph_vector_int_t* membership)
{
  int rc = EXIT_FAILURE;

  if (mxIgraphIsEmpty(p)) {
    igraph_vector_int_init(membership, 0);
    return IGRAPH_SUCCESS;
  }

  if (!mxIsDouble(p)) {
    mexErrMsgIdAndTxt("Igraph:wrongType",
                      "Expected partition to be a double vector.");
  }

  mxIgraphVectorIntFromArray(p, membership, true);

  return rc;
}

/* Create a matlab vector for storing the labels of each node from an igraph
   membership partition.

   Increments igraph partition so that the smallest node id is 1 instead of 0.*/
mxArray* mxIgraphMembershipToArray(igraph_vector_int_t const* membership)
{
  igraph_integer_t n_nodes = igraph_vector_int_size(membership);
  mxArray* p = mxCreateDoubleMatrix(1, n_nodes, mxREAL);
  double* mxMembership = mxGetDoubles(p);

  for (igraph_integer_t i = 0; i < n_nodes; i++) {
    mxMembership[(mwIndex)i] = (double)VECTOR(*membership)[i] + 1;
  }

  return p;
}

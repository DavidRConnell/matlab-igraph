#include "mxIterators.h"

static void mxIgraph_eit_sparse_create(const mxArray* p, mxIgraph_eit* eit,
                                       const igraph_bool_t directed)
{
  eit->ir = mxGetIr(p);
  eit->jc = mxGetJc(p);

  mwIndex col_idx = 0;
  while (eit->jc[col_idx] == eit->jc[col_idx + 1]) col_idx++;
  if (col_idx >= eit->n_nodes) {
    // Empty graph
    col_idx = 0;
  }

  eit->col_idx = col_idx;
  eit->flat_idx = 0;
  eit->row_idx = eit->ir[eit->flat_idx];
  eit->row_start_at_col = false;
  eit->row_stop_at_col = false;

  if ((!directed) &&
      (mxIgraphIsSymmetric(p))) {
    eit->row_stop_at_col = true;
  }
}

static void mxIgraph_eit_full_create(const mxArray* p, mxIgraph_eit* eit,
                                     const igraph_bool_t directed)
{
  eit->ir = NULL;
  eit->jc = NULL;

  eit->col_idx = 0;
  eit->row_idx = 0;
  eit->flat_idx = 0;
  eit->row_start_at_col = false;
  eit->row_stop_at_col = false;

  if (mxIgraphIsTriU(p)) {
    eit->row_stop_at_col = true;
  } else if (mxIgraphIsTriL(p) || (!directed && mxIgraphIsSymmetric(p))) {
    eit->row_start_at_col = true;
  }
}

void mxIgraph_eit_create(const mxArray* p, mxIgraph_eit* eit,
                         const igraph_bool_t directed)
{
  eit->n_nodes = mxIgraphVCount(p);
  eit->stop = eit->n_nodes == 0;

  if (mxIsSparse(p)) {
    mxIgraph_eit_sparse_create(p, eit, directed);
  } else {
    mxIgraph_eit_full_create(p, eit, directed);
  }

  if (mxIsDouble(p)) {
    eit->weight = mxGetDoubles(p);
    eit->is_bool = false;
  } else if (mxIsLogical(p)) {
    eit->weight = mxGetLogicals(p);
    eit->is_bool = true;
  }

  if (!(bool)(MXIGRAPH_EIT_GET_WEIGHT(*eit))) {
    MXIGRAPH_EIT_NEXT(*eit);
  }
}

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

igraph_bool_t mxIgraphIsSquare(const mxArray *p)
{
  mwIndex m = mxGetM(p);
  mwIndex n = mxGetN(p);

  return m == n;
}

static igraph_bool_t is_weighted_sparse_i(const mxArray *p)
{
  mxDouble *adj = mxGetDoubles(p);
  mwIndex *jc = mxGetJc(p);
  mwIndex n = mxGetN(p);
  mwIndex numel = jc[n];

  for (mwIndex i = 0; i < numel; i++) {
    if ((adj[i] != 0) && (adj[i] != 1)) {
      return true;
    }
  }

  return false;
}

static igraph_bool_t is_weighted_full_i(const mxArray *p)
{
  mxDouble *adj = mxGetDoubles(p);
  mwSize m = mxGetM(p);

  mxDouble el;
  for (mwIndex i = 0; i < m; i++) {
    for (mwIndex j = 0; j < m; j++) {
      el = adj[i + (j *m)];
      if ((el != 0) && (el != 1)) {
        return true;
      }
    }
  }

  return false;
}

/* Test if adjacency matrix p points to has values other than 0 or 1. */
igraph_bool_t mxIgraphIsWeighted(const mxArray *p)
{
  // TODO: temporarily always say yes.
  if (mxIgraphIsGraph(p)) {
    return true;
  }

  if (mxIsLogical(p)) {
    return false;
  }

  if (mxIsSparse(p)) {
    return is_weighted_sparse_i(p);
  }

  return is_weighted_full_i(p);
}

static mxDouble sparse_double_index_i(const mxDouble *column, const mwIndex i,
                                      const mwIndex *row_indices,
                                      const mwIndex len)
{
  if (len == 0) {
    return 0;
  }

  mwIndex rng[] = {0, len - 1};
  mwIndex idx = (len - 1) / 2;
  while ((rng[1] >= rng[0]) && (rng[1] < len)) { // Protect against underflow
    if (row_indices[idx] == i) {
      return column[idx];
    }

    if (row_indices[idx] < i) {
      rng[0] = idx + 1;
    } else {
      rng[1] = idx - 1;
    }
    idx = (rng[0] + rng[1]) / 2;
  }

  return 0;
}

static bool sparse_logical_index_i(const bool *column, const mwIndex i,
                                   const mwIndex *row_indices,
                                   const mwIndex len)
{
  if (len == 0) {
    return 0;
  }

  mwIndex rng[] = {0, len - 1};
  mwIndex idx = (len - 1) / 2;
  while ((rng[1] >= rng[0]) && (rng[1] < len)) { // Protect against underflow
    if (row_indices[idx] == i) {
      return column[idx];
    }

    if (row_indices[idx] < i) {
      rng[0] = idx + 1;
    } else {
      rng[1] = idx - 1;
    }
    idx = (rng[0] + rng[1]) / 2;
  }

  return 0;
}

static igraph_bool_t comp_lessthan(mwIndex a, mwIndex b)
{
  return a < b;
}

static igraph_bool_t comp_greaterthan(mwIndex a, mwIndex b)
{
  return a > b;
}

static igraph_bool_t is_tri_sparse(const mxArray *p,
                                   igraph_bool_t comp(mwIndex, mwIndex))
{
  mwIndex *ir = mxGetIr(p);
  mwIndex *jc = mxGetJc(p);
  mwIndex n_nodes = mxGetM(p);
  mwIndex row_i;

  for (mwIndex j = 0; j < n_nodes; j++) {
    for (mwIndex i = jc[j]; i < jc[j + 1]; i++) {
      row_i = ir[i];
      if (comp(row_i, j)) {
        return false;
      }
    }
  }

  return true;
}

static mwIndex regular_index(mwIndex i, mwIndex j, mwIndex n_rows)
{
  return i + (j *n_rows);
}

static mwIndex transposed_index(mwIndex i, mwIndex j, mwIndex n_rows)
{
  return j + (i *n_rows);
}

static igraph_bool_t is_tri_full_logical(const mxArray *p,
    mwIndex index(mwIndex, mwIndex,
                  mwIndex))
{
  bool *adj = mxGetLogicals(p);
  mwIndex n_nodes = mxGetM(p);

  for (mwIndex i = 0; i < n_nodes; i++) {
    for (mwIndex j = (i + 1); j < n_nodes; j++) {
      if (adj[index(i, j, n_nodes)] != 0) {
        return false;
      }
    }
  }

  return true;
}

static igraph_bool_t is_tri_full_double(const mxArray *p,
                                        mwIndex index(mwIndex, mwIndex, mwIndex))
{
  mxDouble *adj = mxGetDoubles(p);
  mwIndex n_nodes = mxGetM(p);

  for (mwIndex i = 0; i < n_nodes; i++) {
    for (mwIndex j = (i + 1); j < n_nodes; j++) {
      if (adj[index(i, j, n_nodes)] != 0) {
        return false;
      }
    }
  }

  return true;
}

igraph_bool_t mxIgraphIsTriU(const mxArray *p)
{
  if (mxIsSparse(p)) {
    return is_tri_sparse(p, comp_greaterthan);
  }

  if (mxIsLogical(p)) {
    return is_tri_full_logical(p, transposed_index);
  }

  return is_tri_full_double(p, transposed_index);
}

igraph_bool_t mxIgraphIsTriL(const mxArray *p)
{
  if (mxIsSparse(p)) {
    return is_tri_sparse(p, comp_lessthan);
  }

  if (mxIsLogical(p)) {
    return is_tri_full_logical(p, regular_index);
  }

  return is_tri_full_double(p, regular_index);
}

static igraph_bool_t is_symmetric_sparse_double_i(const mxArray *p)
{
  mxDouble *adj = mxGetDoubles(p);
  mwIndex *ir = mxGetIr(p);
  mwIndex *jc = mxGetJc(p);
  mwIndex n_nodes = mxGetM(p);

  mxDouble reflection;
  mwIndex row_i;
  for (mwIndex j = 0; j < n_nodes; j++) {
    for (mwIndex i = jc[j]; i < jc[j + 1]; i++) {
      row_i = ir[i];
      reflection = sparse_double_index_i(adj + jc[row_i], j, ir + jc[row_i],
                                         jc[row_i + 1] - jc[row_i]);
      if (reflection != adj[i]) {
        return false;
      }
    }
  }

  return true;
}

static igraph_bool_t is_symmetric_sparse_logical_i(const mxArray *p)
{
  bool *adj = mxGetLogicals(p);
  mwIndex *ir = mxGetIr(p);
  mwIndex *jc = mxGetJc(p);
  mwIndex n_nodes = mxGetM(p);

  mxDouble reflection;
  mwIndex row_i;
  for (mwIndex j = 0; j < n_nodes; j++) {
    for (mwIndex i = jc[j]; i < jc[j + 1]; i++) {
      row_i = ir[i];
      reflection = sparse_logical_index_i(adj + jc[row_i], j, ir + jc[row_i],
                                          jc[row_i + 1] - jc[row_i]);
      if (reflection != adj[i]) {
        return false;
      }
    }
  }

  return true;
}

static igraph_bool_t is_symmetric_full_logical_i(const mxArray *p)
{
  bool *adj = mxGetLogicals(p);
  mwIndex n_nodes = mxGetM(p);

  for (mwIndex i = 0; i < n_nodes; i++) {
    for (mwIndex j = (i + 1); j < n_nodes; j++) {
      if (adj[i + (j *n_nodes)] != adj[j + (i *n_nodes)]) {
        return false;
      }
    }
  }

  return true;
}

static igraph_bool_t is_symmetric_full_double_i(const mxArray *p)
{
  mxDouble *adj = mxGetDoubles(p);
  mwIndex n_nodes = mxGetM(p);

  for (mwIndex i = 0; i < n_nodes; i++) {
    for (mwIndex j = (i + 1); j < n_nodes; j++) {
      if (adj[i + (j *n_nodes)] != adj[j + (i *n_nodes)]) {
        return false;
      }
    }
  }

  return true;
}

/* Test if the adjacency matrix pointed to by p is symmetric. */
igraph_bool_t mxIgraphIsSymmetric(const mxArray *p)
{
  if (!(mxIgraphIsSquare(p))) {
    return false;
  }

  if (mxIsSparse(p) && mxIsDouble(p)) {
    return is_symmetric_sparse_double_i(p);
  } else if (mxIsSparse(p) && mxIsLogical(p)) {
    return is_symmetric_sparse_logical_i(p);
  } else if (mxIsDouble(p)) {
    return is_symmetric_full_double_i(p);
  } else if (mxIsLogical(p)) {
    return is_symmetric_full_logical_i(p);
  }

  IGRAPH_FATAL("Received unexpected data type or representation");
}

// Test if the MATLAB object is of the graph or digraph type.
igraph_bool_t mxIgraphIsGraph(mxArray const *p)
{
  return mxIsClass(p, "graph") || mxIsClass(p, "digraph");
}

/* Guess if the graph is directed or not. */
igraph_bool_t mxIgraphIsDirected(const mxArray *p)
{
  if (mxIgraphIsGraph(p)) {
    return mxIsClass(p, "digraph");
  } else {
    return !(mxIgraphIsTriU(p) || mxIgraphIsTriL(p) || mxIgraphIsSymmetric(p));
  }
}

/* Check if the matlab array pointed to by p is either a row or column
vector. */
igraph_bool_t mxIgraphIsVector(const mxArray *p)
{
  return ((mxGetN(p) == 1) || (mxGetM(p) == 1));
}

/* Test if the matlab array pointed to by p is empty. */
igraph_bool_t mxIgraphIsEmpty(const mxArray *p)
{
  return ((mxGetN(p) == 0) || (mxGetM(p) == 0));
}

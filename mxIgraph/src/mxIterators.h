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

/* WARNING: Cannot use iterators to define predicates since the iterator
   definitions depend on various predicates. */

#ifndef MXITERATORS_H
#define MXITERATORS_H

#include <mxIgraph.h>

typedef struct {
  mwIndex row_idx;
  mwIndex col_idx;
  mwIndex flat_idx; // index for the 1d C array view of the 2d matrix
  mwIndex n_nodes;
  mwIndex* ir;
  mwIndex* jc;
  void* weight;
  bool is_bool;
  bool row_start_at_col;
  bool row_stop_at_col;
  bool stop;
} mxIgraph_eit;

/* Because MATLAB stores matrices in column major order, choose to index with
the column as the "outer" loop and the row as the "inner" loop (i.e. so the
flat indices go up ordinally).

A row_stop of -1 means stop at the current col_idx (row_idx <= col_idx not
row_idx < col_idx). This is useful for triangular matrices where you go up to
the diagonal. */
#define MXIGRAPH_EIT_NEXT(eit)                                                \
  do {                                                                        \
    if ((eit).ir && (eit).jc) {                                               \
      _MXIGRAPH_EIT_SPARSE_NEXT((eit));                                       \
    } else {                                                                  \
      _MXIGRAPH_EIT_FULL_NEXT((eit));                                         \
      while (                                                                 \
        ((double)(MXIGRAPH_EIT_GET_WEIGHT((eit))) == 0) && !(eit).stop) {     \
        _MXIGRAPH_EIT_FULL_NEXT((eit));                                       \
      }                                                                       \
    }                                                                         \
  } while (0)

#define _MXIGRAPH_EIT_FULL_NEXT(eit)                                          \
  if (((eit).row_stop_at_col && ((eit).row_idx < (eit).col_idx)) ||           \
      ((eit).row_idx < ((eit).n_nodes - 1))) {                                \
    (eit).row_idx++;                                                          \
  } else if ((eit).col_idx < ((eit).n_nodes - 1)) {                           \
    (eit).col_idx++;                                                          \
    (eit).row_idx = ((eit).row_start_at_col ? (eit).col_idx : 0);             \
  } else {                                                                    \
    (eit).stop = true;                                                        \
  }                                                                           \
  (eit).flat_idx = (eit).row_idx + ((eit).col_idx * (eit).n_nodes)

#define _MXIGRAPH_SPARSE_FIND_COL(eit)                                        \
  while (                                                                     \
    (((eit).flat_idx >= (eit).jc[(eit).col_idx + 1]) ||                       \
      ((eit).jc[(eit).col_idx] == (eit).jc[(eit).col_idx + 1])) &&            \
    ((eit).col_idx < ((eit).n_nodes - 1)))                                    \
  (eit).col_idx++

#define _MXIGRAPH_EIT_SPARSE_NEXT(eit)                                        \
  do {                                                                        \
    if (!((eit).flat_idx < ((eit).jc[(eit).n_nodes] - 1))) {                  \
      (eit).stop = true;                                                      \
    }                                                                         \
                                                                              \
    if ((eit).flat_idx < ((eit).jc[(eit).n_nodes] - 1)) {                     \
      (eit).flat_idx++;                                                       \
      (eit).row_idx = (eit).ir[(eit).flat_idx];                               \
      _MXIGRAPH_SPARSE_FIND_COL((eit));                                       \
    }                                                                         \
                                                                              \
    while ((eit).row_stop_at_col && ((eit).row_idx > (eit).col_idx) &&        \
           ((eit).col_idx < ((eit).n_nodes) - 1)) {                           \
      (eit).col_idx++;                                                        \
      while ((eit).jc[(eit).col_idx] == (eit).jc[(eit).col_idx + 1])          \
        (eit).col_idx++;                                                      \
      (eit).flat_idx = (eit).jc[(eit).col_idx];                               \
      (eit).row_idx = (eit).ir[(eit).flat_idx];                               \
    }                                                                         \
  } while (0)

#define MXIGRAPH_EIT_GET_ROW(eit) (eit).row_idx
#define MXIGRAPH_EIT_GET_COL(eit) (eit).col_idx
#define MXIGRAPH_EIT_GET_IDX(eit) (eit).flat_idx
#define MXIGRAPH_EIT_GET_WEIGHT(eit)                                          \
  (eit).is_bool ?                                                             \
    ((bool*)(eit).weight)[(eit).flat_idx] :                                   \
    ((double*)(eit).weight)[(eit).flat_idx]

#define MXIGRAPH_EIT_END(eit) (eit).stop

void mxIgraph_eit_create(
  mxArray const* p, mxIgraph_eit* eit, igraph_bool_t const directed);

#endif

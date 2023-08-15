#include "mxIgraph.h"
#include "mxIterators.h"
#include "igraph_interface.h"

/* Return the number of nodes in the adjacency matrix pointed to by p. */
igraph_integer_t mxIgraphVCount(const mxArray *p)
{
  return (igraph_integer_t)mxGetM(p);
}

/* Returns the number of edges in the matrix pointed to by p.

 If the adj is undirected, pairs of symmetric edges are counted as a single
 edge. */
igraph_integer_t mxIgraphECount(const mxArray *p,
                                const igraph_bool_t is_directed)
{
  mxIgraph_eit eit;
  mxIgraph_eit_create(p, &eit, is_directed);

  igraph_integer_t n_edges = 0;
  while (!MXIGRAPH_EIT_END(eit)) {
    n_edges++;
    MXIGRAPH_EIT_NEXT(eit);
  }

  return n_edges;
}

/* Copy a matlab adjacency matrix (sparse or full) to an igraph graph type.

   Creates a graph and it's weights. For unweighted graphs, a weight of 1 is
   given to each edge. (Always creating weights adds memory usage, but it also
   simplifies calling functions that expect weights or NULL. This choice can be
   revisted later.)

   Both the graph and weight vector should be uninitialized but it's the
   callers responsibility to destroy them when done. */
void mxIgraphGetGraph(const mxArray *p, igraph_t *graph,
                      igraph_vector_t *weights,
                      const igraph_bool_t is_directed)
{
  if (!mxIgraphIsSquare(p)) {
    mexErrMsgIdAndTxt("Igraph:notSquare", "Adjacency matrix must be square.");
  }

  igraph_integer_t n_nodes = mxIgraphVCount(p);
  igraph_integer_t n_edges = mxIgraphECount(p, is_directed);
  igraph_vector_int_t edges;
  mxIgraph_eit eit;

  mxIgraph_eit_create(p, &eit, is_directed);
  igraph_empty(graph, n_nodes, is_directed);
  igraph_vector_int_init(&edges, 2 * n_edges);
  igraph_vector_init(weights, n_edges);

  igraph_integer_t edge_idx = 0;
  while (!MXIGRAPH_EIT_END(eit)) {
    VECTOR(edges)[edge_idx] = MXIGRAPH_EIT_GET_ROW(eit);
    VECTOR(edges)[edge_idx + 1] = MXIGRAPH_EIT_GET_COL(eit);
    VECTOR(*weights)[edge_idx / 2] =
      (igraph_real_t)MXIGRAPH_EIT_GET_WEIGHT(eit);
    edge_idx += 2;
    MXIGRAPH_EIT_NEXT(eit);
  }

  igraph_add_edges(graph, &edges, NULL);
}

mxArray *create_adj_full_double_i(igraph_t const *graph,
                                  igraph_vector_t const *weights)
{
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray *p = mxCreateDoubleMatrix(n_nodes, n_nodes, mxREAL);
  double *adj = mxGetDoubles(p);
  igraph_eit_t eit;
  igraph_integer_t eid;
  mwIndex idx;

  igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);

  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    idx = (mwSize)IGRAPH_FROM(graph, eid) +
          (n_nodes * (mwSize)IGRAPH_TO(graph, eid));
    adj[idx] = (double)(weights ? VECTOR(*weights)[eid] : 1);

    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);

  return p;
}

mxArray *create_adj_full_logical_i(igraph_t const *graph)
{
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray *p = mxCreateLogicalMatrix(n_nodes, n_nodes);
  bool *adj = mxGetLogicals(p);
  igraph_eit_t eit;
  igraph_integer_t eid;
  mwIndex idx;

  igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);

  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    idx = (mwSize)IGRAPH_FROM(graph, eid) +
          (n_nodes * (mwSize)IGRAPH_TO(graph, eid));
    adj[idx] = true;

    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);

  return p;
}

static mxArray *create_adj_sparse_double_i(igraph_t const *graph,
    igraph_vector_t const *weights)
{
  mwSize n_edges = (mwSize)igraph_ecount(graph);
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray *p = mxCreateSparse(n_nodes, n_nodes, n_edges, mxREAL);
  double *adj = mxGetDoubles(p);
  mwIndex *jc = mxGetJc(p);
  mwIndex *ir = mxGetIr(p);
  igraph_eit_t eit;
  igraph_integer_t eid;

  igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_TO), &eit);

  mwIndex count = 0;
  mwIndex column = 0, prev_column = 0;
  jc[prev_column] = count;
  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    column = IGRAPH_TO(graph, eid);
    if (prev_column != column) {
      for (mwIndex i = prev_column; i < column; i++) {
        jc[i + 1] = count;
      }
      prev_column = column;
    }

    ir[count] = (mwIndex)IGRAPH_FROM(graph, eid);
    adj[count] = (double)(weights ? VECTOR(*weights)[eid] : 1);

    IGRAPH_EIT_NEXT(eit);
    count++;
  }

  while (column < n_nodes) {
    jc[column + 1] = count;
    column++;
  }

  igraph_eit_destroy(&eit);

  return p;
}

static mxArray *create_adj_sparse_logical_i(igraph_t const *graph)
{
  mwSize n_edges = (mwSize)igraph_ecount(graph);
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray *p = mxCreateSparseLogicalMatrix(n_nodes, n_nodes, n_edges);
  bool *adj = mxGetLogicals(p);
  mwIndex *jc = mxGetJc(p);
  mwIndex *ir = mxGetIr(p);
  igraph_eit_t eit;
  igraph_integer_t eid;

  igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_TO), &eit);

  mwIndex count = 0;
  mwIndex column = 0, prev_column = 0;
  jc[prev_column] = count;
  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    column = IGRAPH_TO(graph, eid);
    if (prev_column != column) {
      for (mwIndex i = prev_column; i < column; i++) {
        jc[i + 1] = count;
      }
      prev_column = column;
    }

    ir[count] = (mwIndex)IGRAPH_FROM(graph, eid);
    adj[count] = true;

    IGRAPH_EIT_NEXT(eit);
    count++;
  }

  while (column < n_nodes) {
    jc[column + 1] = count;
    column++;
  }

  igraph_eit_destroy(&eit);

  return p;
}

/* Create a matlab adjacency matrix using an igraph graph and weight vector.

   If the weights vector is `NULL` the resulting adjacency matrix will use
   `1` for the weight of all edges.

   See `mxIgraphGetGraph` to convert an matlab adj into an igraph graph. */
mxArray *mxIgraphCreateAdj(const igraph_t *graph,
                           const igraph_vector_t *weights,
                           const igraph_bool_t sparse,
                           const mxIgraphDType_t type)
{
  mxArray *p;
  if (sparse && (type == MXIGRAPH_DTYPE_LOGICAL)) {
    p = create_adj_sparse_logical_i(graph);
  } else if (sparse && (type == MXIGRAPH_DTYPE_DOUBLE)) {
    p = create_adj_sparse_double_i(graph, weights);
  } else if (!sparse && (type == MXIGRAPH_DTYPE_LOGICAL)) {
    p = create_adj_full_logical_i(graph);
  } else if (!sparse && (type == MXIGRAPH_DTYPE_DOUBLE)) {
    p = create_adj_full_double_i(graph, weights);
  } else {
    mexErrMsgIdAndTxt("Igraph:internal:unknownType", "Unhandled data type.");
    exit(1);
  }

  return p;
}

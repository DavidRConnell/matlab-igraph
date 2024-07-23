#include <mxIgraph.h>
#include "mxIterators.h"

/* Return the number of nodes in the adjacency matrix pointed to by p. */
igraph_integer_t mxIgraphVCount(const mxArray* p)
{
  return (igraph_integer_t)mxGetM(p);
}

/* Returns the number of edges in the matrix pointed to by p.

 If the adj is undirected, pairs of symmetric edges are counted as a single
 edge. */
igraph_integer_t mxIgraphECount(const mxArray* p,
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

/* Convert a mxArray containing a (di)graph class to a structure.
   and use structures to create MATLAB graph types.

 MATLAB's support for modifying classes created with MATLAB's classdef is
 limited and makes copies of everything. Properties that are available in
 MATLAB are not always available to mex, specifically tables (which the graph
 type uses) seem to have pretend properties that appear to be properties in
 MATLAB but don't actually exist when requesting properties in mex. As such
 it's easy and more performant (less coping data) to convert to an easy to work
 with structure based type.

 This also makes it much easier to create a MATLAB graph type. */
static inline mxArray const* mxGraph2struct_i(mxArray const* p)
{
  mxArray* ret, * in;
  in = (mxArray*)p;

  mexCallMATLAB(1, &ret, 1, &in, "igraph.args.graph2struct");

  return ret;
}

static void get_graph_i(mxArray const* p, igraph_t* graph,
                        igraph_vector_t* weights,
                        igraph_bool_t const is_weighted,
                        igraph_bool_t const is_directed,
                        char const* weight_att)
{
  mxArray const* mxGraph = mxGraph2struct_i(p);

  mxArray const* nodeTable = mxGetField(mxGraph, 0, "Nodes");
  mxArray const* edgeTable = mxGetField(mxGraph, 0, "Edges");
  igraph_integer_t const n_nodes = mxIgraphGetInteger(nodeTable, "n");
  igraph_integer_t const n_edges = mxIgraphGetInteger(edgeTable, "n");

  double* mxEdges = mxGetDoubles(mxGetField(edgeTable, 0, "EndNodes"));
  double* mxWeights;
  igraph_vector_int_t edges;

  igraph_empty(graph, n_nodes, is_directed);
  igraph_vector_int_init(&edges, 2 * n_edges);

  if (is_weighted) {
    igraph_vector_init(weights, n_edges);
    mxWeights = mxGetDoubles(mxGetField(edgeTable, 0, weight_att));
  } else if (weights) {
    igraph_vector_init(weights, 0);
  }

  for (igraph_integer_t i = 0; i < n_edges; i++) {
    VECTOR(edges)[2 * i] = mxEdges[i] - 1;
    VECTOR(edges)[(2 * i) + 1] = mxEdges[i + n_edges] - 1;
    if (is_weighted) {
      VECTOR(*weights)[i] = mxWeights[i];
    }
  }

  igraph_add_edges(graph, &edges, NULL);
  igraph_vector_int_destroy(&edges);
}

static void get_adj_i(mxArray const* p, igraph_t* graph,
                      igraph_vector_t* weights,
                      igraph_bool_t const is_weighted,
                      igraph_bool_t const is_directed)
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

  if (is_weighted) {
    igraph_vector_init(weights, n_edges);
  } else if (weights) {
    igraph_vector_init(weights, 0);
  }

  igraph_integer_t edge_idx = 0;
  while (!MXIGRAPH_EIT_END(eit)) {
    VECTOR(edges)[edge_idx] = MXIGRAPH_EIT_GET_ROW(eit);
    VECTOR(edges)[edge_idx + 1] = MXIGRAPH_EIT_GET_COL(eit);
    if (is_weighted) {
      VECTOR(*weights)[edge_idx / 2] =
        (igraph_real_t)(MXIGRAPH_EIT_GET_WEIGHT(eit));
    }
    edge_idx += 2;
    MXIGRAPH_EIT_NEXT(eit);
  }

  igraph_add_edges(graph, &edges, NULL);
  igraph_vector_int_destroy(&edges);
}

/* Copy a MATLAB adjacency matrix (sparse or full) to an igraph graph type.

   Creates a graph and it's weights. For unweighted graphs, a weight of 1 is
   given to each edge unless the caller passes NULL to the weight argument.

   Both the graph and weight vector should be uninitialized but it's the
   callers responsibility to destroy them when done. */
void mxIgraphGetGraph(mxArray const* p, igraph_t* graph,
                      igraph_vector_t* weights, mxArray const* graph_options)
{
  igraph_bool_t const is_directed = mxIgraphGetBool(graph_options,
                                    "isdirected");
  igraph_bool_t is_weighted;
  char* weight_att;

  if (mxGetField(graph_options, 0, "isweighted")) {
    is_weighted  = mxIgraphGetBool(graph_options, "isweighted");
    weight_att = is_weighted ?
                 mxIgraphGetString(graph_options, "weight") : "";
  } else {
    /* Add defaults so we can get away with not setting weight related fields
    for methods where weights are never used. */
    is_weighted = false;
    weight_att = "";
  }

  if (is_weighted && !weights) {
    mexErrMsgIdAndTxt("igraph:internal:missingWeights",
                      "Graph is weighted but weight vector not provided.");
  }

  if (mxIgraphIsGraph(p)) {
    get_graph_i(p, graph, weights, is_weighted, is_directed, weight_att);
  } else {
    get_adj_i(p, graph, weights, is_weighted, is_directed);
  }
}

mxArray* create_adj_full_double_i(igraph_t const* graph,
                                  igraph_vector_t const* weights)
{
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray* p = mxCreateDoubleMatrix(n_nodes, n_nodes, mxREAL);
  double* adj = mxGetDoubles(p);
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

mxArray* create_adj_full_logical_i(igraph_t const* graph)
{
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray* p = mxCreateLogicalMatrix(n_nodes, n_nodes);
  bool* adj = mxGetLogicals(p);
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

static mxArray* create_adj_sparse_double_i(igraph_t const* graph,
    igraph_vector_t const* weights)
{
  mwSize n_edges = (mwSize)igraph_ecount(graph);
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray* p = mxCreateSparse(n_nodes, n_nodes, n_edges, mxREAL);
  double* adj = mxGetDoubles(p);
  mwIndex* jc = mxGetJc(p);
  mwIndex* ir = mxGetIr(p);
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

static mxArray* create_graph_i(igraph_t const* graph,
                               igraph_vector_t const* weights,
                               char const* weight_att)
{
  mxArray* ret;
  mxArray* in[2];
  igraph_integer_t const n_edges = igraph_ecount(graph);

  char const* top_fields[] = { "Nodes", "Edges" };
  mwSize nfields = sizeof(top_fields) / sizeof(top_fields[0]);
  in[0] = mxCreateStructMatrix(1, 1, nfields, top_fields);
  in[1] = mxCreateLogicalScalar(igraph_is_directed(graph));

  mxArray* mxNodes, * mxEdges;
  char const* node_fields[] = { "n" };
  nfields = sizeof(node_fields) / sizeof(node_fields[0]);
  mxNodes = mxCreateStructMatrix(1, 1, nfields, node_fields);
  mxSetField(mxNodes, 0, "n", mxCreateDoubleScalar(igraph_vcount(graph)));

  char const* edge_fields[weights ? 3 : 2];
  edge_fields[0] = "n";
  edge_fields[1] = "EndNodes";
  if (weights) {
    edge_fields[2] = weight_att;
  }

  mxArray* edge_array = mxCreateDoubleMatrix(n_edges, 2, mxREAL);
  double* mxEndNodes = mxGetDoubles(edge_array);
  for (igraph_integer_t i = 0; i < n_edges; i++) {
    mxEndNodes[i] = IGRAPH_FROM(graph, i) + 1;
    mxEndNodes[i + n_edges] = IGRAPH_TO(graph, i) + 1;
  }

  nfields = sizeof(edge_fields) / sizeof(edge_fields[0]);
  mxEdges = mxCreateStructMatrix(1, 1, nfields, edge_fields);
  mxSetField(mxEdges, 0, "n", mxCreateDoubleScalar(n_edges));
  mxSetField(mxEdges, 0, "EndNodes", edge_array);

  if (weights) {
    mxArray* weight_array = mxCreateDoubleMatrix(n_edges, 1, mxREAL);
    double* mxWeights = mxGetDoubles(weight_array);
    for (igraph_integer_t i = 0; i < n_edges; i++) {
      mxWeights[i] = VECTOR(*weights)[i];
    }
    mxSetField(mxEdges, 0, weight_att, weight_array);
  }

  mxSetField(in[0], 0, "Nodes", mxNodes);
  mxSetField(in[0], 0, "Edges", mxEdges);
  mexCallMATLAB(1, &ret, 2, in, "igraph.args.struct2graph");

  return ret;
}

static mxArray* create_adj_sparse_logical_i(igraph_t const* graph)
{
  mwSize n_edges = (mwSize)igraph_ecount(graph);
  mwSize n_nodes = (mwSize)igraph_vcount(graph);
  mxArray* p = mxCreateSparseLogicalMatrix(n_nodes, n_nodes, n_edges);
  bool* adj = mxGetLogicals(p);
  mwIndex* jc = mxGetJc(p);
  mwIndex* ir = mxGetIr(p);
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
mxArray* mxIgraphCreateGraph(igraph_t const* graph,
                             igraph_vector_t const* weights,
                             mxArray const* graphOpts)
{
  mxIgraphRepr_t repr = mxIgraphSelectRepr(graphOpts);
  mxIgraphDType_t dtype = mxIgraphSelectDType(graphOpts);
  char const* weight_attr = mxGetField(graphOpts, 0, "weight") ?
                            mxIgraphGetString(graphOpts, "weight") :
                            "Weight";

  igraph_bool_t const is_weighted = !(!weights ||
                                      igraph_vector_empty(weights) ||
                                      dtype == MXIGRAPH_DTYPE_LOGICAL);
  mxArray* p;

  if (repr == MXIGRAPH_REPR_GRAPH) {
    p = create_graph_i(graph, is_weighted ? weights : NULL, weight_attr);
  } else if ((repr == MXIGRAPH_REPR_SPARSE) &&
             (dtype == MXIGRAPH_DTYPE_LOGICAL)) {
    p = create_adj_sparse_logical_i(graph);
  } else if ((repr == MXIGRAPH_REPR_SPARSE) &&
             (dtype == MXIGRAPH_DTYPE_DOUBLE)) {
    p = create_adj_sparse_double_i(graph, is_weighted ? weights : NULL);
  } else if ((repr == MXIGRAPH_REPR_FULL) &&
             (dtype == MXIGRAPH_DTYPE_LOGICAL)) {
    p = create_adj_full_logical_i(graph);
  } else if ((repr == MXIGRAPH_REPR_FULL) &&
             (dtype == MXIGRAPH_DTYPE_DOUBLE)) {
    p = create_adj_full_double_i(graph, is_weighted ? weights : NULL);
  } else {
    mexErrMsgIdAndTxt("Igraph:internal:unknownType", "Unhandled data type.");
    exit(1);
  }

  return p;
}

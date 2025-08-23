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

#include "mxIterators.h"

#include <mxIgraph.h>

/* Return the number of nodes in the adjacency matrix pointed to by p. */
igraph_integer_t mxIgraphVCount(mxArray const* p)
{
  return (igraph_integer_t)mxGetM(p);
}

/* Returns the number of edges in the matrix pointed to by p.

 If the adj is undirected, pairs of symmetric edges are counted as a single
 edge. */
igraph_integer_t mxIgraphECount(
  mxArray const* p, igraph_bool_t const is_directed)
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
  mxArray *ret, *in;
  in = (mxArray*)p;

  mexCallMATLAB(1, &ret, 1, &in, "igutils.graph2struct");

  return ret;
}

static igraph_error_t get_graph_i(mxArray const* p, igraph_t* graph,
  igraph_vector_t* weights, igraph_bool_t const is_weighted,
  igraph_bool_t const is_directed, char const* weight_att)
{
  mxArray const* mxGraph = mxGraph2struct_i(p);

  mxArray const* nodeTable = mxGetField(mxGraph, 0, "Nodes");
  mxArray const* edgeTable = mxGetField(mxGraph, 0, "Edges");
  igraph_integer_t const n_nodes = mxIgraphIntegerFromOptions(nodeTable, "n");
  igraph_integer_t const n_edges = mxIgraphIntegerFromOptions(edgeTable, "n");
  MXIGRAPH_CHECK_STATUS();

  double* mxEdges = mxGetDoubles(mxGetField(edgeTable, 0, "EndNodes"));
  double* mxWeights;
  igraph_vector_int_t edges;

  IGRAPH_CHECK(igraph_empty(graph, n_nodes, is_directed));
  IGRAPH_FINALLY(igraph_destroy, graph);
  IGRAPH_CHECK(igraph_vector_int_init(&edges, 2 * n_edges));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &edges);

  if (is_weighted) {
    IGRAPH_CHECK(igraph_vector_init(weights, n_edges));
    mxWeights = mxGetDoubles(mxGetField(edgeTable, 0, weight_att));
  } else if (weights) {
    IGRAPH_CHECK(igraph_vector_init(weights, 0));
  }
  IGRAPH_FINALLY(igraph_vector_destroy, weights);

  for (igraph_integer_t i = 0; i < n_edges; i++) {
    VECTOR(edges)[2 * i] = mxEdges[i] - 1;
    VECTOR(edges)[(2 * i) + 1] = mxEdges[i + n_edges] - 1;
    if (is_weighted) {
      VECTOR(*weights)[i] = mxWeights[i];
    }
  }

  IGRAPH_CHECK(igraph_add_edges(graph, &edges, NULL));
  igraph_vector_int_destroy(&edges);
  IGRAPH_FINALLY_CLEAN(3);

  return IGRAPH_SUCCESS;
}

static igraph_error_t get_adj_i(mxArray const* p, igraph_t* graph,
  igraph_vector_t* weights, igraph_bool_t const is_weighted,
  igraph_bool_t const is_directed)
{
  if (!mxIgraphIsSquare(p)) {
    IGRAPH_ERROR("Adjacency matrix not square.", IGRAPH_EINVAL);
  }

  igraph_integer_t n_nodes = mxIgraphVCount(p);
  igraph_integer_t n_edges = mxIgraphECount(p, is_directed);
  igraph_vector_int_t edges;
  mxIgraph_eit eit;

  mxIgraph_eit_create(p, &eit, is_directed);
  IGRAPH_CHECK(igraph_empty(graph, n_nodes, is_directed));
  IGRAPH_FINALLY(igraph_destroy, graph);
  IGRAPH_CHECK(igraph_vector_int_init(&edges, 2 * n_edges));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &edges);

  if (is_weighted) {
    IGRAPH_CHECK(igraph_vector_init(weights, n_edges));
  } else if (weights) {
    IGRAPH_CHECK(igraph_vector_init(weights, 0));
  }
  IGRAPH_FINALLY(igraph_vector_destroy, weights);

  igraph_integer_t edge_idx = 0;
  while (!MXIGRAPH_EIT_END(eit)) {
    VECTOR(edges)[edge_idx] = MXIGRAPH_EIT_GET_ROW(eit);
    VECTOR(edges)[edge_idx + 1] = MXIGRAPH_EIT_GET_COL(eit);
    if (is_weighted) {
      VECTOR(*weights)
      [edge_idx / 2] = (igraph_real_t)(MXIGRAPH_EIT_GET_WEIGHT(eit));
    }
    edge_idx += 2;
    MXIGRAPH_EIT_NEXT(eit);
  }

  IGRAPH_CHECK(igraph_add_edges(graph, &edges, NULL));
  igraph_vector_int_destroy(&edges);
  IGRAPH_FINALLY_CLEAN(3);

  return IGRAPH_SUCCESS;
}

/* Copy a MATLAB adjacency matrix (sparse or full) to an igraph graph type.

   Creates a graph and it's weights. For unweighted graphs, a weight of 1 is
   given to each edge unless the caller passes NULL to the weight argument.

   Both the graph and weight vector should be uninitialized but it's the
   callers responsibility to destroy them when done. */
igraph_error_t mxIgraphFromArray(mxArray const* p, igraph_t* graph,
  igraph_vector_t* weights, mxArray const* graph_options)
{
  igraph_bool_t const is_directed =
    mxIgraphBoolFromOptions(graph_options, "isdirected");
  MXIGRAPH_CHECK_STATUS();

  igraph_bool_t is_weighted;
  char* weight_att;

  if (mxGetField(graph_options, 0, "isweighted")) {
    is_weighted = mxIgraphBoolFromOptions(graph_options, "isweighted");
    weight_att =
      is_weighted ? mxIgraphStringFromOptions(graph_options, "weight") : "";
    MXIGRAPH_CHECK_STATUS();
  } else {
    /* Add defaults so we can get away with not setting weight related fields
    for methods where weights are never used. */
    is_weighted = false;
    weight_att = "";
  }

  if (is_weighted && !weights) {
    IGRAPH_FATAL("Weighted graph passed NULL for weight vector.");
  }

  if (mxIgraphIsGraph(p)) {
    IGRAPH_CHECK(
      get_graph_i(p, graph, weights, is_weighted, is_directed, weight_att));
  } else {
    IGRAPH_CHECK(get_adj_i(p, graph, weights, is_weighted, is_directed));
  }

  return IGRAPH_SUCCESS;
}

static mxArray* create_adj_full_double_i(
  igraph_t const* graph, igraph_vector_t const* weights)
{
  mwSize n_nodes = igraph_vcount(graph);
  mxArray* p = mxCreateDoubleMatrix(n_nodes, n_nodes, mxREAL);
  double* adj = mxGetDoubles(p);
  igraph_eit_t eit;
  igraph_integer_t eid;
  mwIndex idx;
  igraph_error_t rs;

  rs = igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);
  if (rs != IGRAPH_SUCCESS) {
    mxIgraphSetError(rs);
    mxIgraphSetErrorMsg("Failed to create edge iterator.");
    return p;
  }

  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    idx = IGRAPH_FROM(graph, eid) + (n_nodes * IGRAPH_TO(graph, eid));
    adj[idx] = weights ? VECTOR(*weights)[eid] : 1;

    IGRAPH_EIT_NEXT(eit);
  }
  igraph_eit_destroy(&eit);

  return p;
}

static mxArray* create_adj_full_logical_i(igraph_t const* graph)
{
  mwSize n_nodes = igraph_vcount(graph);
  mxArray* p = mxCreateLogicalMatrix(n_nodes, n_nodes);
  bool* adj = mxGetLogicals(p);
  igraph_eit_t eit;
  igraph_integer_t eid;
  mwIndex idx;
  igraph_error_t rs;

  rs = igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);
  if (rs != IGRAPH_SUCCESS) {
    mxIgraphSetError(rs);
    mxIgraphSetErrorMsg("Failed to create edge iterator.");
    return p;
  }

  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    idx = IGRAPH_FROM(graph, eid) + (n_nodes * IGRAPH_TO(graph, eid));
    adj[idx] = true;

    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);

  return p;
}

static mxArray* create_adj_sparse_double_i(
  igraph_t const* graph, igraph_vector_t const* weights)
{
  mwSize n_edges = igraph_ecount(graph);
  mwSize n_nodes = igraph_vcount(graph);
  mxArray* p = mxCreateSparse(n_nodes, n_nodes, n_edges, mxREAL);
  double* adj = mxGetDoubles(p);
  mwIndex* jc = mxGetJc(p);
  mwIndex* ir = mxGetIr(p);
  igraph_eit_t eit;
  igraph_integer_t eid;
  igraph_error_t rs;

  rs = igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_TO), &eit);
  if (rs != IGRAPH_SUCCESS) {
    mxIgraphSetError(rs);
    mxIgraphSetErrorMsg("Failed to create edge iterator.");
    return p;
  }

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

    ir[count] = IGRAPH_FROM(graph, eid);
    adj[count] = (weights ? VECTOR(*weights)[eid] : 1);

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

static mxArray* create_adj_sparse_logical_i(igraph_t const* graph)
{
  mwSize n_edges = igraph_ecount(graph);
  mwSize n_nodes = igraph_vcount(graph);
  mxArray* p = mxCreateSparseLogicalMatrix(n_nodes, n_nodes, n_edges);
  bool* adj = mxGetLogicals(p);
  mwIndex* jc = mxGetJc(p);
  mwIndex* ir = mxGetIr(p);
  igraph_eit_t eit;
  igraph_integer_t eid;
  igraph_error_t rs;

  rs = igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_TO), &eit);
  if (rs != IGRAPH_SUCCESS) {
    mxIgraphSetError(rs);
    mxIgraphSetErrorMsg("Failed to create edge iterator.");
    return p;
  }

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

    ir[count] = IGRAPH_FROM(graph, eid);
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

static mxArray* create_graph_i(igraph_t const* graph,
  igraph_vector_t const* weights, char const* weight_att)
{
  mxArray* ret;
  mxArray* in[2];
  igraph_integer_t const n_edges = igraph_ecount(graph);

  char const* top_fields[] = { "Nodes", "Edges" };
  mwSize nfields = sizeof(top_fields) / sizeof(top_fields[0]);
  in[0] = mxCreateStructMatrix(1, 1, nfields, top_fields);
  in[1] = mxCreateLogicalScalar(igraph_is_directed(graph));

  mxArray *mxNodes, *mxEdges;
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
  mexCallMATLAB(1, &ret, 2, in, "igutils.struct2graph");

  return ret;
}

/* Create a matlab adjacency matrix using an igraph graph and weight vector.

   See `mxIgraphFromArray` to convert an matlab adj into an igraph graph. */
mxArray* mxIgraphToArray(igraph_t const* graph, igraph_vector_t const* weights,
  mxArray const* graphOpts)
{
  mxArray* p = NULL;
  mxIgraphRepr_t repr = mxIgraphReprFromOptions(graphOpts);
  mxIgraphDType_t dtype = mxIgraphDTypeFromOptions(graphOpts);
  char const* weight_attr =
    mxGetField(graphOpts, 0, "weight") ?
      mxIgraphStringFromOptions(graphOpts, "weight") :
      "Weight";

  MXIGRAPH_CHECK_STATUS_RETURN(p);

  igraph_bool_t const is_weighted = !(
    !weights || igraph_vector_empty(weights) ||
    dtype == MXIGRAPH_DTYPE_LOGICAL);

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
    IGRAPH_FATAL("Received unexpected data type or representation.");
  }

  if (mxIgraphGetError() != IGRAPH_SUCCESS) {
    return NULL;
  }

  return p;
}

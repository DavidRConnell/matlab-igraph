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

#include <string.h>
#include <mxIgraph.h>
#include "utils.h"

typedef enum {
  MXIGRAPH_CLUSTER_OPTIMAL = 0,
  MXIGRAPH_CLUSTER_SPINGLASS,
  MXIGRAPH_CLUSTER_LEADINGEIGENVECTOR,
  MXIGRAPH_CLUSTER_WALKTRAP,
  MXIGRAPH_CLUSTER_EDGEBETWEENNESS,
  MXIGRAPH_CLUSTER_FASTGREEDY,
  MXIGRAPH_CLUSTER_MULTILEVEL,
  MXIGRAPH_CLUSTER_LEIDEN,
  MXIGRAPH_CLUSTER_FLUIDCOMMUNITIES,
  MXIGRAPH_CLUSTER_LABELPROPAGATION,
  MXIGRAPH_CLUSTER_INFOMAP,
  MXIGRAPH_CLUSTER_N
} mxIgraph_cluster_t;

static igraph_error_t mxIgraph_optimal_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *UNUSED(opts),
    igraph_vector_int_t *membership)
{
  igraph_error_t errcode;

  errcode =
    igraph_community_optimal_modularity(graph, NULL, membership, weights);

  return errcode;
}

static igraph_error_t mxIgraph_spinglass_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *opts,
    igraph_vector_int_t *membership)
{
  igraph_integer_t n_spins = mxIgraphGetInteger(opts, "nSpins");
  igraph_bool_t parallel = mxIgraphGetBool(opts, "parallel");
  igraph_real_t start_temp = mxIgraphGetReal(opts, "tempStart");
  igraph_real_t end_temp = mxIgraphGetReal(opts, "tempEnd");
  igraph_real_t cool_factor = mxIgraphGetReal(opts, "coolingFactor");
  char const *update_rule_str = mxIgraphGetString(opts, "updateRule");
  igraph_spincomm_update_t update_rule;
  igraph_real_t gamma = mxIgraphGetReal(opts, "resolution");
  igraph_real_t gamma_minus = mxIgraphGetReal(opts, "negResolution");
  igraph_spinglass_implementation_t implementation =
    igraph_vector_min(weights) < 0 ? IGRAPH_SPINCOMM_IMP_NEG
    : IGRAPH_SPINCOMM_IMP_ORIG;
  igraph_error_t errorcode;

  if (strcmp(update_rule_str, "simple") == 0) {
    update_rule = IGRAPH_SPINCOMM_UPDATE_SIMPLE;
  } else {
    update_rule = IGRAPH_SPINCOMM_UPDATE_CONFIG;
  }

  errorcode = igraph_community_spinglass(graph, weights, NULL, NULL, membership,
                                         NULL, n_spins, parallel, start_temp,
                                         end_temp, cool_factor, update_rule,
                                         gamma, implementation, gamma_minus);

  return errorcode;
}

static igraph_error_t mxIgraph_leading_eigenvector_i(
  igraph_t const *graph, igraph_vector_t const *weights, mxArray const *opts,
  igraph_vector_int_t *membership)
{
  igraph_integer_t steps = mxIgraphGetInteger(opts, "maxSteps");
  igraph_vector_int_t init;
  igraph_error_t errcode;

  mxIgraphGetVectorInt(opts, "initial", &init, true);
  for (igraph_integer_t i = 0; i < igraph_vcount(graph); i++) {
    VECTOR(*membership)[i] = VECTOR(init)[i];
  }
  igraph_vector_int_destroy(&init);

  errcode = igraph_community_leading_eigenvector(
              graph, weights, NULL, membership, steps, NULL, NULL, true, NULL, NULL,
              NULL, NULL, NULL);

  return errcode;
}

static igraph_error_t mxIgraph_walktrap_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *opts,
    igraph_vector_int_t *membership)
{
  igraph_integer_t steps = mxIgraphGetInteger(opts, "nSteps");
  igraph_error_t errcode;

  errcode =
    igraph_community_walktrap(graph, weights, steps, NULL, NULL, membership);

  return errcode;
}

static igraph_error_t mxIgraph_edge_betweenness_i(igraph_t const *graph,
    igraph_vector_t const *weights, mxArray const *opts,
    igraph_vector_int_t *membership)
{
  igraph_bool_t isweighted = mxIgraphGetBool(opts, "isweighted");
  igraph_vector_t const *weights_ptr = NULL;
  igraph_error_t errcode;

  if (isweighted) {
    /* Weights cause a warning so use NULL instead of vector of 1s for
       unweighted graphs. */
    weights_ptr = weights;
  }

  errcode = igraph_community_edge_betweenness(
              graph, NULL, NULL, NULL, NULL, NULL, membership,
              igraph_is_directed(graph), weights_ptr);

  return errcode;
}

static igraph_error_t mxIgraph_fastgreedy_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *UNUSED(opts),
    igraph_vector_int_t *membership)
{
  igraph_error_t errcode;

  errcode = igraph_community_fastgreedy(graph, weights, NULL, NULL, membership);

  return errcode;
}

static igraph_error_t mxIgraph_multilevel_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *opts,
    igraph_vector_int_t *membership)
{
  igraph_real_t resolution = mxIgraphGetReal(opts, "resolution");
  igraph_error_t errcode;

  errcode = igraph_community_multilevel(graph, weights, resolution, membership,
                                        NULL, NULL);

  return errcode;
}

static igraph_error_t mxIgraph_leiden_i(igraph_t const *graph,
                                        igraph_vector_t const *weights,
                                        mxArray const *opts,
                                        igraph_vector_int_t *membership)
{
  igraph_real_t resolution = mxIgraphGetReal(opts, "resolution");
  igraph_real_t beta = mxIgraphGetReal(opts, "randomness");
  igraph_vector_t node_weights;
  igraph_vector_t *node_weights_ptr = NULL;
  igraph_integer_t n_iterations = mxIgraphGetInteger(opts, "nIterations");
  igraph_bool_t use_modularity =
    strcmp(mxIgraphGetString(opts, "metric"), "modularity") == 0;
  igraph_vector_int_t init;
  igraph_error_t errcode;

  mxIgraphGetVectorInt(opts, "initial", &init, true);
  for (igraph_integer_t i = 0; i < igraph_vcount(graph); i++) {
    VECTOR(*membership)[i] = VECTOR(init)[i];
  }
  igraph_vector_int_destroy(&init);

  if (use_modularity) {
    igraph_vector_init(&node_weights, igraph_vcount(graph));
    igraph_strength(graph, &node_weights, igraph_vss_all(), IGRAPH_ALL, true,
                    weights);
    node_weights_ptr = &node_weights;
  }

  errcode =
    igraph_community_leiden(graph, weights, node_weights_ptr, resolution,
                            beta, true, n_iterations, membership, NULL, NULL);

  if (use_modularity) {
    igraph_vector_destroy(&node_weights);
  }

  return errcode;
}

static igraph_error_t mxIgraph_fluid_communities_i(
  igraph_t const *graph, igraph_vector_t const *UNUSED(weights),
  mxArray const *opts, igraph_vector_int_t *membership)
{
  igraph_integer_t n_communities = mxIgraphGetInteger(opts, "nCommunities");
  igraph_error_t errcode;

  errcode =
    igraph_community_fluid_communities(graph, n_communities, membership);

  return errcode;
}

static igraph_error_t mxIgraph_label_propagation_i(
  igraph_t const *graph, igraph_vector_t const *weights, mxArray const *opts,
  igraph_vector_int_t *membership)
{
  igraph_vector_int_t initial;
  igraph_neimode_t mode = mxIgraphSelectMode(opts);
  igraph_vector_bool_t fixed;
  igraph_error_t errcode;

  mxIgraphGetVectorInt(opts, "initial", &initial, true);
  mxIgraphGetVectorBool(opts, "fixed", &fixed, false);

  errcode = igraph_community_label_propagation(graph, membership, mode, weights,
            &initial, &fixed);

  igraph_vector_int_destroy(&initial);
  igraph_vector_bool_destroy(&fixed);

  return errcode;
}

static igraph_error_t mxIgraph_infomap_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *opts,
    igraph_vector_int_t *membership)
{
  igraph_integer_t n_trials = mxIgraphGetInteger(opts, "nTrials");
  igraph_vector_t v_weights;
  /* Don't think we need codelength, but crashes Matlab when NULL is used.*/
  igraph_real_t codelength = 0;
  igraph_error_t errcode;

  mxIgraphGetVector(opts, "nodeWeights", &v_weights, false);

  errcode = igraph_community_infomap(graph, weights, &v_weights, n_trials,
                                     membership, &codelength);

  igraph_vector_destroy(&v_weights);

  return errcode;
}

igraph_error_t mexIgraphCluster(int nlhs, mxArray *plhs[], int nrhs,
                                mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *graph_options = prhs[2];
  mxArray const *method_options = prhs[3];
  mxIgraph_cluster_t method;
  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_int_t membership;
  igraph_error_t errorcode = IGRAPH_SUCCESS;
  typedef igraph_error_t (*cluster_method_t)(
    igraph_t const *, igraph_vector_t const *, mxArray const *,
    igraph_vector_int_t *);
  cluster_method_t cluster_method;

  char const *methods[MXIGRAPH_CLUSTER_N] = {
    [MXIGRAPH_CLUSTER_OPTIMAL] = "optimal",
    [MXIGRAPH_CLUSTER_SPINGLASS] = "spinglass",
    [MXIGRAPH_CLUSTER_LEADINGEIGENVECTOR] = "leadingeigenvector",
    [MXIGRAPH_CLUSTER_WALKTRAP] = "walktrap",
    [MXIGRAPH_CLUSTER_EDGEBETWEENNESS] = "edgebetweenness",
    [MXIGRAPH_CLUSTER_FASTGREEDY] = "fastgreedy",
    [MXIGRAPH_CLUSTER_MULTILEVEL] = "multilevel",
    [MXIGRAPH_CLUSTER_LEIDEN] = "leiden",
    [MXIGRAPH_CLUSTER_FLUIDCOMMUNITIES] = "fluidcommunities",
    [MXIGRAPH_CLUSTER_LABELPROPAGATION] = "labelpropagation",
    [MXIGRAPH_CLUSTER_INFOMAP] = "infomap",
  };

  method = mxIgraphSelectMethod(prhs[1], methods, MXIGRAPH_CLUSTER_N);

  if (method == MXIGRAPH_CLUSTER_N) {
    mxIgraphErrorUnknownMethod(mexFunctionName(), mxArrayToString(prhs[1]));
    exit(1);
  }

  cluster_method_t method_table[MXIGRAPH_CLUSTER_N] = {
    [MXIGRAPH_CLUSTER_OPTIMAL] = mxIgraph_optimal_i,
    [MXIGRAPH_CLUSTER_SPINGLASS] = mxIgraph_spinglass_i,
    [MXIGRAPH_CLUSTER_LEADINGEIGENVECTOR] = mxIgraph_leading_eigenvector_i,
    [MXIGRAPH_CLUSTER_WALKTRAP] = mxIgraph_walktrap_i,
    [MXIGRAPH_CLUSTER_EDGEBETWEENNESS] = mxIgraph_edge_betweenness_i,
    [MXIGRAPH_CLUSTER_FASTGREEDY] = mxIgraph_fastgreedy_i,
    [MXIGRAPH_CLUSTER_MULTILEVEL] = mxIgraph_multilevel_i,
    [MXIGRAPH_CLUSTER_LEIDEN] = mxIgraph_leiden_i,
    [MXIGRAPH_CLUSTER_FLUIDCOMMUNITIES] = mxIgraph_fluid_communities_i,
    [MXIGRAPH_CLUSTER_LABELPROPAGATION] = mxIgraph_label_propagation_i,
    [MXIGRAPH_CLUSTER_INFOMAP] = mxIgraph_infomap_i
  };

  cluster_method = method_table[method];

  if (!cluster_method) {
    mxIgraphErrorNotImplemented("Generate", mxArrayToString(prhs[1]));
    exit(1);
  }

  mxIgraphFromArray(prhs[0], &graph, &weights, graph_options);
  igraph_vector_int_init(&membership, igraph_vcount(&graph));

  errorcode = cluster_method(&graph, MXIGRAPH_WEIGHTS(&weights), method_options,
                             &membership);
  igraph_destroy(&graph);
  igraph_vector_destroy(&weights);

  plhs[0] = mxIgraphVectorIntToArray(&membership, MXIGRAPH_IDX_SHIFT);
  igraph_vector_int_destroy(&membership);

  return errorcode;
}

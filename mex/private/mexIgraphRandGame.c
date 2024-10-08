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

#include "utils.h"
#include <mxIgraph.h>

typedef enum {
  MXIGRAPH_GAME_GRG = 0,
  MXIGRAPH_GAME_BARABASI_BAG,
  MXIGRAPH_GAME_BARABASI_PSUMTREE,
  MXIGRAPH_GAME_BARABASI_PSUMTREE_MULTIPLE,
  MXIGRAPH_GAME_ERDOS_RENYI,
  MXIGRAPH_GAME_WATTS_STROGATZ,
  MXIGRAPH_GAME_DEGSEQ_CONFIGURATION,
  MXIGRAPH_GAME_DEGSEQ_CONFIGURATION_SIMPLE,
  MXIGRAPH_GAME_DEGSEQ_FAST_HEUR_SIMPLE,
  MXIGRAPH_GAME_DEGSEQ_EDGE_SWITCHING_SIMPLE,
  MXIGRAPH_GAME_DEGSEQ_VL,
  MXIGRAPH_GAME_K_REGULAR,
  MXIGRAPH_GAME_STATIC_FITNESS,
  MXIGRAPH_GAME_STATIC_POWER_LAW,
  MXIGRAPH_GAME_FOREST_FIRE,
  MXIGRAPH_GAME_GROWING_RANDOM,
  MXIGRAPH_GAME_CALLAWAY_TRAITS,
  MXIGRAPH_GAME_ESTABLISHMENT,
  MXIGRAPH_GAME_PREFERENCE,
  MXIGRAPH_GAME_ASYMMETRIC_PREFERENCE,
  MXIGRAPH_GAME_RECENT_DEGREE,
  MXIGRAPH_GAME_BARABASI_AGING,
  MXIGRAPH_GAME_RECENT_DEGREE_AGING,
  MXIGRAPH_GAME_LASTCIT,
  MXIGRAPH_GAME_CITED_TYPE,
  MXIGRAPH_GAME_CITING_CITED_TYPE,
  MXIGRAPH_GAME_SBM,
  MXIGRAPH_GAME_HSBM,
  MXIGRAPH_GAME_HSBM_LIST,
  MXIGRAPH_GAME_DOT_PRODUCT,
  MXIGRAPH_GAME_TREE,
  MXIGRAPH_GAME_CORRELATED_PAIR,
  MXIGRAPH_GAME_SIMPLE_INTERCONNECTED_ISLANDS,
  MXIGRAPH_GAME_N
} mxIgraph_game_t;

static igraph_error_t mxIgraph_grg_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_real_t radius = mxIgraphRealFromOptions(opts, "radius");
  igraph_bool_t torus = mxIgraphBoolFromOptions(opts, "torus");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_grg_game(graph, n_nodes, radius, torus, NULL, NULL));

  return IGRAPH_SUCCESS;
}

#define BARABASI_BODY(algo)                                                    \
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");       \
  igraph_real_t power = mxIgraphRealFromOptions(opts, "power");                \
  igraph_integer_t m = 0;                                                      \
  igraph_vector_int_t outseq;                                                  \
  igraph_vector_int_t *outseq_ptr = NULL;                                      \
  igraph_bool_t outpref = mxIgraphBoolFromOptions(opts, "outPreference");      \
  igraph_real_t A = mxIgraphRealFromOptions(opts, "attractiveness");           \
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");        \
  mxArray *start_argument = mxIgraphGetArgument(opts, "startFrom");            \
  igraph_t start_from;                                                         \
  igraph_t *start_from_ptr = NULL;                                             \
  MXIGRAPH_CHECK_STATUS();                                                     \
                                                                               \
  if (mxIsScalar(mxIgraphGetArgument(opts, "nConnections"))) {                 \
    m = mxIgraphIntegerFromOptions(opts, "nConnections");                      \
    MXIGRAPH_CHECK_STATUS();                                                   \
  } else {                                                                     \
    IGRAPH_CHECK(                                                              \
        mxIgraphVectorIntFromOptions(opts, "nConnections", &outseq, false));   \
    IGRAPH_FINALLY(igraph_vector_int_destroy, &outseq);                        \
    outseq_ptr = &outseq;                                                      \
  }                                                                            \
                                                                               \
  if (!mxIsEmpty(start_argument)) {                                            \
    IGRAPH_CHECK(mxIgraphFromArray(start_argument, &start_from, NULL, opts));  \
    IGRAPH_FINALLY(igraph_destroy, &start_from);                               \
  }                                                                            \
                                                                               \
  IGRAPH_CHECK(igraph_barabasi_game(graph, n_nodes, power, m, outseq_ptr,      \
                                    outpref, A, directed, (algo),              \
                                    start_from_ptr));                          \
                                                                               \
  if (start_from_ptr) {                                                        \
    igraph_destroy(&start_from);                                               \
    IGRAPH_FINALLY_CLEAN(1);                                                   \
  }                                                                            \
                                                                               \
  if (outseq_ptr) {                                                            \
    igraph_vector_int_destroy(&outseq);                                        \
    IGRAPH_FINALLY_CLEAN(1);                                                   \
  }                                                                            \
                                                                               \
  return IGRAPH_SUCCESS

static igraph_error_t mxIgraph_barabasi_bag_i(mxArray const *opts,
    igraph_t *graph)
{
  BARABASI_BODY(IGRAPH_BARABASI_BAG);
}

static igraph_error_t mxIgraph_barabasi_psumtree_i(mxArray const *opts,
    igraph_t *graph)
{
  BARABASI_BODY(IGRAPH_BARABASI_PSUMTREE);
}

static igraph_error_t mxIgraph_barabasi_psumtree_multiple_i(
  mxArray const *opts,
  igraph_t *graph)
{
  BARABASI_BODY(IGRAPH_BARABASI_PSUMTREE_MULTIPLE);
}

static igraph_error_t mxIgraph_erdos_renyi_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_bool_t edge_method = mxIgraphBoolFromOptions(opts, "useEdges");
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_bool_t loops = mxIgraphBoolFromOptions(opts, "loops");
  MXIGRAPH_CHECK_STATUS();

  if (edge_method) {
    igraph_integer_t n_edges = mxIgraphIntegerFromOptions(opts, "nEdges");
    MXIGRAPH_CHECK_STATUS();
    IGRAPH_CHECK(
      igraph_erdos_renyi_game_gnm(graph, n_nodes, n_edges, directed, loops));
  } else {
    igraph_real_t probability = mxIgraphRealFromOptions(opts, "probability");
    MXIGRAPH_CHECK_STATUS();
    IGRAPH_CHECK(igraph_erdos_renyi_game_gnp(graph, n_nodes, probability,
                 directed, loops));
  }

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_watts_strogatz_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t dim = mxIgraphIntegerFromOptions(opts, "dim");
  igraph_integer_t size = mxIgraphIntegerFromOptions(opts, "size");
  igraph_integer_t n_neighbors = mxIgraphIntegerFromOptions(opts, "radius");
  igraph_real_t probability = mxIgraphRealFromOptions(opts, "probability");
  igraph_bool_t loops = mxIgraphBoolFromOptions(opts, "loops");
  igraph_bool_t multiple = mxIgraphBoolFromOptions(opts, "multiple");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_watts_strogatz_game(graph, dim, size, n_neighbors,
                                          probability, loops, multiple));

  return IGRAPH_SUCCESS;
}

#define DEGSEQ_BODY(algo)                                                      \
  igraph_vector_int_t outDegree;                                               \
  igraph_vector_int_t inDegree;                                                \
  igraph_error_t errcode;                                                      \
                                                                               \
  IGRAPH_CHECK(                                                                \
      mxIgraphVectorIntFromOptions(opts, "outDegree", &outDegree, false));     \
  IGRAPH_FINALLY(igraph_vector_int_destroy, &outDegree);                       \
  IGRAPH_CHECK(                                                                \
      mxIgraphVectorIntFromOptions(opts, "inDegree", &inDegree, false));       \
  IGRAPH_FINALLY(igraph_vector_int_destroy, &inDegree);                        \
  IGRAPH_CHECK(                                                                \
      igraph_degree_sequence_game(graph, &outDegree, &inDegree, (algo)));      \
                                                                               \
  igraph_vector_int_destroy(&outDegree);                                       \
  igraph_vector_int_destroy(&inDegree);                                        \
  IGRAPH_FINALLY_CLEAN(2);                                                     \
                                                                               \
  return IGRAPH_SUCCESS

static igraph_error_t mxIgraph_degseq_configuration_i(mxArray const *opts,
    igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_CONFIGURATION);
}

static igraph_error_t mxIgraph_degseq_configuration_simple_i(
  mxArray const *opts, igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_CONFIGURATION_SIMPLE);
}

static igraph_error_t mxIgraph_degseq_fast_heur_simple_i(mxArray const *opts,
    igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_FAST_HEUR_SIMPLE);
}

static igraph_error_t mxIgraph_degseq_edge_switching_i(mxArray const *opts,
    igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_EDGE_SWITCHING_SIMPLE);
}

static igraph_error_t mxIgraph_degseq_vl_i(mxArray const *opts,
    igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_VL);
}

static igraph_error_t mxIgraph_k_regular_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t degree = mxIgraphIntegerFromOptions(opts, "degree");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_bool_t multiple = mxIgraphBoolFromOptions(opts, "multiple");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(
    igraph_k_regular_game(graph, n_nodes, degree, directed, multiple));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_static_fitness_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_edges = mxIgraphIntegerFromOptions(opts, "nEdges");
  igraph_bool_t loops = mxIgraphBoolFromOptions(opts, "loops");
  igraph_bool_t multiple = mxIgraphBoolFromOptions(opts, "multiple");
  MXIGRAPH_CHECK_STATUS();
  igraph_vector_t fitness_out;
  igraph_vector_t fitness_in;
  igraph_vector_t *fitness_in_ptr = NULL;

  IGRAPH_CHECK(
    mxIgraphVectorFromOptions(opts, "outFitness", &fitness_out, false));
  IGRAPH_FINALLY(igraph_vector_destroy, &fitness_out);
  IGRAPH_CHECK(
    mxIgraphVectorFromOptions(opts, "inFitness", &fitness_in, false));
  IGRAPH_FINALLY(igraph_vector_destroy, &fitness_in);

  if (!mxIgraphIsEmpty(mxIgraphGetArgument(opts, "inFitness"))) {
    fitness_in_ptr = &fitness_in;
  }

  IGRAPH_CHECK(igraph_static_fitness_game(graph, n_edges, &fitness_out,
                                          fitness_in_ptr, loops, multiple));

  igraph_vector_destroy(&fitness_out);
  igraph_vector_destroy(&fitness_in);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_static_power_law_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t n_edges = mxIgraphIntegerFromOptions(opts, "nEdges");
  igraph_real_t exponent_out = mxIgraphRealFromOptions(opts, "outExponent");
  igraph_real_t exponent_in = mxIgraphRealFromOptions(opts, "inExponent");
  igraph_bool_t loops = mxIgraphBoolFromOptions(opts, "loops");
  igraph_bool_t multiple = mxIgraphBoolFromOptions(opts, "multiple");
  igraph_bool_t finite_size_correction =
    mxIgraphBoolFromOptions(opts, "finiteSizeCorrection");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_static_power_law_game(graph, n_nodes, n_edges,
               exponent_out, exponent_in, loops,
               multiple, finite_size_correction));

  return IGRAPH_SUCCESS;
}
static igraph_error_t mxIgraph_forest_fire_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_real_t fw_prob = mxIgraphRealFromOptions(opts, "forwardProbability");
  igraph_real_t bw_factor = mxIgraphRealFromOptions(opts, "backwardFactor");
  igraph_integer_t ambs = mxIgraphIntegerFromOptions(opts, "nAmbassadors");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_forest_fire_game(graph, n_nodes, fw_prob, bw_factor, ambs,
                                       directed));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_growing_random_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t n_edges = mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_bool_t citation = mxIgraphBoolFromOptions(opts, "citation");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(
    igraph_growing_random_game(graph, n_nodes, n_edges, directed, citation));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_callaway_traits_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t n_types = mxIgraphIntegerFromOptions(opts, "nTypes");
  igraph_integer_t edges_per_step =
    mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_t type_dist;
  igraph_vector_t *type_dist_ptr = NULL;
  igraph_matrix_t pref_matrix;

  IGRAPH_CHECK(mxIgraphVectorFromOptions(opts, "typeDistribution", &type_dist,
                                         MXIGRAPH_IDX_KEEP));
  IGRAPH_FINALLY(igraph_vector_destroy, &type_dist);
  IGRAPH_CHECK(mxIgraphMatrixFromOptions(opts, "preference", &pref_matrix,
                                         MXIGRAPH_IDX_KEEP));
  IGRAPH_FINALLY(igraph_matrix_destroy, &pref_matrix);
  MXIGRAPH_CHECK_STATUS();

  if (!mxIgraphIsEmpty(mxIgraphGetArgument(opts, "typeDistribution"))) {
    type_dist_ptr = &type_dist;
  }

  IGRAPH_CHECK(igraph_callaway_traits_game(graph, n_nodes, n_types,
               edges_per_step, type_dist_ptr,
               &pref_matrix, directed, NULL));

  igraph_vector_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_establishment_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t n_types = mxIgraphIntegerFromOptions(opts, "nTypes");
  igraph_integer_t edges_per_step =
    mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_t type_dist;
  igraph_vector_t *type_dist_ptr = NULL;
  igraph_matrix_t pref_matrix;

  IGRAPH_CHECK(
    mxIgraphVectorFromOptions(opts, "typeDistribution", &type_dist, false));
  IGRAPH_FINALLY(igraph_vector_destroy, &type_dist);
  IGRAPH_CHECK(
    mxIgraphMatrixFromOptions(opts, "preference", &pref_matrix, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &pref_matrix);
  MXIGRAPH_CHECK_STATUS();

  if (!mxIgraphIsEmpty(mxIgraphGetArgument(opts, "typeDistribution"))) {
    type_dist_ptr = &type_dist;
  }

  IGRAPH_CHECK(igraph_establishment_game(graph, n_nodes, n_types,
                                         edges_per_step, type_dist_ptr,
                                         &pref_matrix, directed, NULL));

  igraph_vector_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_preference_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t n_types = mxIgraphIntegerFromOptions(opts, "nTypes");
  igraph_bool_t fixed_sizes = mxIgraphIntegerFromOptions(opts, "fixedSizes");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_bool_t loops = mxIgraphBoolFromOptions(opts, "loops");
  igraph_vector_t type_dist;
  igraph_vector_t *type_dist_ptr = NULL;
  igraph_matrix_t pref_matrix;

  IGRAPH_CHECK(
    mxIgraphVectorFromOptions(opts, "typeDistribution", &type_dist, false));
  IGRAPH_FINALLY(igraph_vector_destroy, &type_dist);
  IGRAPH_CHECK(
    mxIgraphMatrixFromOptions(opts, "preference", &pref_matrix, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &pref_matrix);
  MXIGRAPH_CHECK_STATUS();

  if (!mxIgraphIsEmpty(mxIgraphGetArgument(opts, "typeDistribution"))) {
    type_dist_ptr = &type_dist;
  }

  IGRAPH_CHECK(igraph_preference_game(graph, n_nodes, n_types, type_dist_ptr,
                                      fixed_sizes, &pref_matrix, NULL, directed,
                                      loops));

  igraph_vector_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_asymmetric_preference_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t n_out_types = mxIgraphIntegerFromOptions(opts, "nOutTypes");
  igraph_integer_t n_in_types = mxIgraphIntegerFromOptions(opts, "nInTypes");
  igraph_bool_t loops = mxIgraphBoolFromOptions(opts, "loops");
  igraph_matrix_t type_dist;
  igraph_matrix_t *type_dist_ptr = NULL;
  igraph_matrix_t pref_matrix;

  IGRAPH_CHECK(
    mxIgraphMatrixFromOptions(opts, "typeDistribution", &type_dist, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &type_dist);
  IGRAPH_CHECK(
    mxIgraphMatrixFromOptions(opts, "preference", &pref_matrix, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &pref_matrix);

  MXIGRAPH_CHECK_STATUS();

  if (!mxIgraphIsEmpty(mxIgraphGetArgument(opts, "typeDistribution"))) {
    type_dist_ptr = &type_dist;
  }

  IGRAPH_CHECK(igraph_asymmetric_preference_game(
                 graph, n_nodes, n_out_types, n_in_types, type_dist_ptr, &pref_matrix,
                 NULL, NULL, loops));

  igraph_matrix_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_recent_degree_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_real_t power = mxIgraphRealFromOptions(opts, "degreeExp");
  igraph_integer_t time_window = mxIgraphIntegerFromOptions(opts, "timeWindow");
  igraph_integer_t edges_per_time_step =
    mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t outpref = mxIgraphBoolFromOptions(opts, "outPreference");
  igraph_real_t zero_appeal = mxIgraphRealFromOptions(opts, "zeroAppeal");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_int_t outseq;
  igraph_vector_int_t *outseq_ptr = NULL;

  IGRAPH_CHECK(mxIgraphVectorIntFromOptions(opts, "outSeq", &outseq, false));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &outseq);

  MXIGRAPH_CHECK_STATUS();

  if (!mxIgraphIsEmpty(mxIgraphGetArgument(opts, "outSeq"))) {
    outseq_ptr = &outseq;
  }

  IGRAPH_CHECK(igraph_recent_degree_game(graph, n_nodes, power, time_window,
                                         edges_per_time_step, outseq_ptr,
                                         outpref, zero_appeal, directed));

  igraph_vector_int_destroy(&outseq);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_barabasi_aging_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t edges_per_time_step =
    mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t outpref = mxIgraphBoolFromOptions(opts, "outPreference");
  igraph_real_t pa_exp = mxIgraphRealFromOptions(opts, "degreeExp");
  igraph_real_t aging_exp = mxIgraphRealFromOptions(opts, "ageExp");
  igraph_integer_t aging_bins = mxIgraphIntegerFromOptions(opts, "agingBins");
  igraph_real_t zero_deg_appeal =
    mxIgraphRealFromOptions(opts, "zeroDegreeAppeal");
  igraph_real_t zero_age_appeal =
    mxIgraphRealFromOptions(opts, "zeroAgeAppeal");
  igraph_real_t deg_coef = mxIgraphRealFromOptions(opts, "degreeCoef");
  igraph_real_t age_coef = mxIgraphRealFromOptions(opts, "ageCoef");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_int_t outseq;

  IGRAPH_CHECK(mxIgraphVectorIntFromOptions(opts, "outSeq", &outseq, false));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &outseq);
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_barabasi_aging_game(
                 graph, n_nodes, edges_per_time_step, &outseq, outpref, pa_exp, aging_exp,
                 aging_bins, zero_deg_appeal, zero_age_appeal, deg_coef, age_coef,
                 directed));
  igraph_vector_int_destroy(&outseq);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_recent_degree_aging_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t n_edges = mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t outpref = mxIgraphBoolFromOptions(opts, "outPreference");
  igraph_real_t pa_exp = mxIgraphRealFromOptions(opts, "degreeExp");
  igraph_real_t aging_exp = mxIgraphRealFromOptions(opts, "ageExp");
  igraph_integer_t aging_bins = mxIgraphIntegerFromOptions(opts, "agingBins");
  igraph_integer_t time_window = mxIgraphIntegerFromOptions(opts, "timeWindow");
  igraph_real_t zero_appeal = mxIgraphRealFromOptions(opts, "zeroAppeal");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_int_t outseq;

  IGRAPH_CHECK(mxIgraphVectorIntFromOptions(opts, "outSeq", &outseq, false));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &outseq);

  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_recent_degree_aging_game(
                 graph, n_nodes, n_edges, &outseq, outpref, pa_exp, aging_exp, aging_bins,
                 time_window, zero_appeal, directed));

  igraph_vector_int_destroy(&outseq);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_lastcit_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t edges_per_node =
    mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_integer_t aging_bins = mxIgraphIntegerFromOptions(opts, "agingBins");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_t preference;

  IGRAPH_CHECK(
    mxIgraphVectorFromOptions(opts, "preference", &preference, false));
  IGRAPH_FINALLY(igraph_vector_destroy, &preference);

  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_lastcit_game(graph, n_nodes, edges_per_node, aging_bins,
                                   &preference, directed));

  igraph_vector_destroy(&preference);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_cited_type_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t edges_per_step =
    mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_int_t types;
  igraph_vector_t preferences;

  IGRAPH_CHECK(mxIgraphVectorIntFromOptions(opts, "types", &types, true));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &types);
  IGRAPH_CHECK(
    mxIgraphVectorFromOptions(opts, "preference", &preferences, false));
  IGRAPH_FINALLY(igraph_vector_destroy, &preferences);

  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_cited_type_game(graph, n_nodes, &types, &preferences,
                                      edges_per_step, directed));

  igraph_vector_int_destroy(&types);
  igraph_vector_destroy(&preferences);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_citing_cited_type_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t edges_per_step =
    mxIgraphIntegerFromOptions(opts, "edgesPerStep");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_vector_int_t types;
  igraph_matrix_t pref_matrix;

  IGRAPH_CHECK(mxIgraphVectorIntFromOptions(opts, "types", &types, true));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &types);
  IGRAPH_CHECK(
    mxIgraphMatrixFromOptions(opts, "preference", &pref_matrix, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &pref_matrix);

  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_citing_cited_type_game(
                 graph, n_nodes, &types, &pref_matrix, edges_per_step, directed));

  igraph_vector_int_destroy(&types);
  igraph_matrix_destroy(&pref_matrix);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_sbm_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_bool_t loops = mxIgraphBoolFromOptions(opts, "loops");
  igraph_matrix_t pref_matrix;
  igraph_vector_int_t block_sizes;

  IGRAPH_CHECK(
    mxIgraphMatrixFromOptions(opts, "preference", &pref_matrix, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &pref_matrix);
  IGRAPH_CHECK(
    mxIgraphVectorIntFromOptions(opts, "blockSizes", &block_sizes, false));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &block_sizes);

  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_sbm_game(graph, n_nodes, &pref_matrix, &block_sizes,
                               directed, loops));

  igraph_matrix_destroy(&pref_matrix);
  igraph_vector_int_destroy(&block_sizes);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_hsbm_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t edges_per_block =
    mxIgraphIntegerFromOptions(opts, "blockSizes");
  igraph_real_t p = mxIgraphRealFromOptions(opts, "preference");
  igraph_vector_t rho;
  igraph_matrix_t c;

  IGRAPH_CHECK(mxIgraphVectorFromOptions(opts, "rho", &rho, false));
  IGRAPH_FINALLY(igraph_vector_destroy, &rho);
  IGRAPH_CHECK(mxIgraphMatrixFromOptions(opts, "preference", &c, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &c);

  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_hsbm_game(graph, n_nodes, edges_per_block, &rho, &c, p));

  igraph_vector_destroy(&rho);
  igraph_matrix_destroy(&c);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_dot_product_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_matrix_t vecs;
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");

  IGRAPH_CHECK(mxIgraphMatrixFromOptions(opts, "positions", &vecs, false));
  IGRAPH_FINALLY(igraph_matrix_destroy, &vecs);

  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_dot_product_game(graph, &vecs, directed));

  igraph_matrix_destroy(&vecs);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_tree_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_bool_t directed = mxIgraphBoolFromOptions(opts, "isdirected");
  MXIGRAPH_CHECK_STATUS();

  igraph_integer_t const n_methods = 2;
  const char *methods[2] = {[IGRAPH_RANDOM_TREE_PRUFER] = "prufer",
                            [IGRAPH_RANDOM_TREE_LERW] = "lerw"
                           };
  igraph_random_tree_t method = mxIgraphSelectMethod(
                                  mxIgraphGetArgument(opts, "method"), methods, n_methods);
  MXIGRAPH_CHECK_METHOD(method, mxIgraphGetArgument(opts, "method"));

  IGRAPH_CHECK(igraph_tree_game(graph, n_nodes, directed, method));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_simple_interconnected_islands_i(
  mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_islands = mxIgraphIntegerFromOptions(opts, "nIslands");
  igraph_integer_t islands_size =
    mxIgraphIntegerFromOptions(opts, "islandSize");
  igraph_real_t p_in = mxIgraphRealFromOptions(opts, "pIn");
  igraph_integer_t n_inter = mxIgraphIntegerFromOptions(opts, "nInterEdges");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_simple_interconnected_islands_game(
                 graph, n_islands, islands_size, p_in, n_inter));

  return IGRAPH_SUCCESS;
}

igraph_error_t mexIgraphRandGame(int nlhs, mxArray *plhs[], int nrhs,
                                 mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxIgraph_game_t method;
  mxArray const *graph_options = prhs[1];
  mxArray const *method_options = prhs[2];
  igraph_t graph;
  typedef igraph_error_t (*game_method_t)(mxArray const *prhs, igraph_t *g);
  game_method_t game_method;

  const char *games[MXIGRAPH_GAME_N] = {
    [MXIGRAPH_GAME_GRG] = "grg",
    [MXIGRAPH_GAME_BARABASI_BAG] = "barabasibag",
    [MXIGRAPH_GAME_BARABASI_PSUMTREE] = "barabasipsumtree",
    [MXIGRAPH_GAME_BARABASI_PSUMTREE_MULTIPLE] = "barabasipsumtreemultiple",
    [MXIGRAPH_GAME_ERDOS_RENYI] = "erdosrenyi",
    [MXIGRAPH_GAME_WATTS_STROGATZ] = "wattsstrogatz",
    [MXIGRAPH_GAME_DEGSEQ_CONFIGURATION] = "degseqconfiguration",
    [MXIGRAPH_GAME_DEGSEQ_CONFIGURATION_SIMPLE] = "degseqconfigurationsimple",
    [MXIGRAPH_GAME_DEGSEQ_FAST_HEUR_SIMPLE] = "degseqfastheursimple",
    [MXIGRAPH_GAME_DEGSEQ_EDGE_SWITCHING_SIMPLE] =
    "degseqedgeswitchingsimple",
    [MXIGRAPH_GAME_DEGSEQ_VL] = "degseqvl",
    [MXIGRAPH_GAME_K_REGULAR] = "kregular",
    [MXIGRAPH_GAME_STATIC_FITNESS] = "staticfitness",
    [MXIGRAPH_GAME_STATIC_POWER_LAW] = "staticpowerlaw",
    [MXIGRAPH_GAME_FOREST_FIRE] = "forestfire",
    [MXIGRAPH_GAME_GROWING_RANDOM] = "growingrandom",
    [MXIGRAPH_GAME_CALLAWAY_TRAITS] = "callawaytraits",
    [MXIGRAPH_GAME_ESTABLISHMENT] = "establishment",
    [MXIGRAPH_GAME_PREFERENCE] = "preference",
    [MXIGRAPH_GAME_ASYMMETRIC_PREFERENCE] = "asymmetricpreference",
    [MXIGRAPH_GAME_RECENT_DEGREE] = "recentdegree",
    [MXIGRAPH_GAME_BARABASI_AGING] = "barabasiaging",
    [MXIGRAPH_GAME_RECENT_DEGREE_AGING] = "recentdegreeaging",
    [MXIGRAPH_GAME_LASTCIT] = "lastcit",
    [MXIGRAPH_GAME_CITED_TYPE] = "citedtype",
    [MXIGRAPH_GAME_CITING_CITED_TYPE] = "citingcitedtype",
    [MXIGRAPH_GAME_SBM] = "sbm",
    [MXIGRAPH_GAME_HSBM] = "hsbm",
    [MXIGRAPH_GAME_HSBM_LIST] = "hsbmlist",
    [MXIGRAPH_GAME_DOT_PRODUCT] = "dotproduct",
    [MXIGRAPH_GAME_TREE] = "tree",
    [MXIGRAPH_GAME_CORRELATED_PAIR] = "correlatedpair",
    [MXIGRAPH_GAME_SIMPLE_INTERCONNECTED_ISLANDS] =
    "simpleinterconnectedislands",
  };

  method = mxIgraphSelectMethod(prhs[0], games, MXIGRAPH_GAME_N);
  MXIGRAPH_CHECK_METHOD(method, prhs[0]);

  game_method_t method_table[MXIGRAPH_GAME_N] = {
    [MXIGRAPH_GAME_GRG] = mxIgraph_grg_i,
    [MXIGRAPH_GAME_BARABASI_BAG] = mxIgraph_barabasi_bag_i,
    [MXIGRAPH_GAME_BARABASI_PSUMTREE] = mxIgraph_barabasi_psumtree_i,
    [MXIGRAPH_GAME_BARABASI_PSUMTREE_MULTIPLE] =
    mxIgraph_barabasi_psumtree_multiple_i,
    [MXIGRAPH_GAME_ERDOS_RENYI] = mxIgraph_erdos_renyi_i,
    [MXIGRAPH_GAME_WATTS_STROGATZ] = mxIgraph_watts_strogatz_i,
    [MXIGRAPH_GAME_DEGSEQ_CONFIGURATION] = mxIgraph_degseq_configuration_i,
    [MXIGRAPH_GAME_DEGSEQ_CONFIGURATION_SIMPLE] =
    mxIgraph_degseq_configuration_simple_i,
    [MXIGRAPH_GAME_DEGSEQ_FAST_HEUR_SIMPLE] =
    mxIgraph_degseq_fast_heur_simple_i,
    [MXIGRAPH_GAME_DEGSEQ_EDGE_SWITCHING_SIMPLE] =
    mxIgraph_degseq_edge_switching_i,
    [MXIGRAPH_GAME_DEGSEQ_VL] = mxIgraph_degseq_vl_i,
    [MXIGRAPH_GAME_K_REGULAR] = mxIgraph_k_regular_i,
    [MXIGRAPH_GAME_STATIC_FITNESS] = mxIgraph_static_fitness_i,
    [MXIGRAPH_GAME_STATIC_POWER_LAW] = mxIgraph_static_power_law_i,
    [MXIGRAPH_GAME_FOREST_FIRE] = mxIgraph_forest_fire_i,
    [MXIGRAPH_GAME_GROWING_RANDOM] = mxIgraph_growing_random_i,
    [MXIGRAPH_GAME_CALLAWAY_TRAITS] = mxIgraph_callaway_traits_i,
    [MXIGRAPH_GAME_ESTABLISHMENT] = mxIgraph_establishment_i,
    [MXIGRAPH_GAME_PREFERENCE] = mxIgraph_preference_i,
    [MXIGRAPH_GAME_ASYMMETRIC_PREFERENCE] = mxIgraph_asymmetric_preference_i,
    [MXIGRAPH_GAME_RECENT_DEGREE] = mxIgraph_recent_degree_i,
    [MXIGRAPH_GAME_BARABASI_AGING] = mxIgraph_barabasi_aging_i,
    [MXIGRAPH_GAME_RECENT_DEGREE_AGING] = mxIgraph_recent_degree_aging_i,
    [MXIGRAPH_GAME_LASTCIT] = mxIgraph_lastcit_i,
    [MXIGRAPH_GAME_CITED_TYPE] = mxIgraph_cited_type_i,
    [MXIGRAPH_GAME_CITING_CITED_TYPE] = mxIgraph_citing_cited_type_i,
    [MXIGRAPH_GAME_SBM] = mxIgraph_sbm_i,
    [MXIGRAPH_GAME_HSBM] = mxIgraph_hsbm_i,
    [MXIGRAPH_GAME_DOT_PRODUCT] = mxIgraph_dot_product_i,
    [MXIGRAPH_GAME_TREE] = mxIgraph_tree_i,
    [MXIGRAPH_GAME_SIMPLE_INTERCONNECTED_ISLANDS] =
    mxIgraph_simple_interconnected_islands_i,
  };

  game_method = method_table[method];
  if (!game_method) {
    IGRAPH_ERRORF("Random game \"%s\" not implemented.", IGRAPH_UNIMPLEMENTED,
                  mxArrayToString(prhs[0]));
  }

  IGRAPH_CHECK(game_method(method_options, &graph));

  plhs[0] = mxIgraphToArray(&graph, NULL, graph_options);
  igraph_destroy(&graph);

  return IGRAPH_SUCCESS;
}

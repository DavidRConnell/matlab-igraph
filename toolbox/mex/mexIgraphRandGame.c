#include "mxIgraph.h"
#include "utils.h"

#define VERIFY_N_OPTIONALS(expected)                            \
  do {                                                          \
    if (n_optionals != (expected)) {                            \
      mexErrMsgIdAndTxt("Igraph:internal:n_optionals",          \
			"%s expected %d optionals.",            \
			mxArrayToString(prhs[0]), (expected));	\
    }                                                           \
  } while (0)

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

static igraph_error_t mxIgraph_grg_i(const mxArray *p_optionals[],
                                     igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_real_t radius = mxGetScalar(p_optionals[1]);
  igraph_bool_t torus = mxGetScalar(p_optionals[2]);
  igraph_error_t errcode;

  errcode = igraph_grg_game(graph, n_nodes, radius, torus, NULL, NULL);

  return errcode;
}

#define BARABASI_BODY(algo)						\
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);		\
  igraph_real_t power = mxGetScalar(p_optionals[1]);			\
  igraph_integer_t m = 0;						\
  igraph_vector_int_t outseq;						\
  igraph_vector_int_t *outseq_ptr = NULL;				\
  igraph_bool_t outpref = mxGetScalar(p_optionals[3]);			\
  igraph_real_t A = mxGetScalar(p_optionals[4]);			\
  igraph_bool_t directed = mxGetScalar(p_optionals[5]);			\
  igraph_t start_from;							\
  igraph_t *start_from_ptr = NULL;					\
  igraph_error_t errcode;						\
									\
  if (mxIsScalar(p_optionals[2])) {					\
    m = mxGetScalar(p_optionals[2]);					\
  } else {								\
    mxIgraphGetVectorInt(p_optionals[2], &outseq);			\
    outseq_ptr = &outseq;						\
  }									\
									\
  if (!mxIsEmpty(p_optionals[6])) {					\
    mxIgraphGetGraph(p_optionals[6], &start_from, NULL, directed);	\
  }									\
									\
  errcode = igraph_barabasi_game(graph, n_nodes, power, m, outseq_ptr,	\
				 outpref, A, directed,			\
				 (algo), start_from_ptr);		\
									\
  if (outseq_ptr) {							\
    igraph_vector_int_destroy(&outseq);					\
  }									\
									\
  if (start_from_ptr) {							\
    igraph_destroy(&start_from);					\
  }									\
									\
  return errcode

static igraph_error_t mxIgraph_barabasi_bag_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  BARABASI_BODY(IGRAPH_BARABASI_BAG);
}

static igraph_error_t mxIgraph_barabasi_psumtree_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  BARABASI_BODY(IGRAPH_BARABASI_PSUMTREE);
}

static igraph_error_t mxIgraph_barabasi_psumtree_multiple_i(
  const mxArray *p_optionals[], igraph_t *graph)
{
  BARABASI_BODY(IGRAPH_BARABASI_PSUMTREE_MULTIPLE);
}

static igraph_error_t mxIgraph_erdos_renyi_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_bool_t edge_method = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[1]);
  igraph_bool_t directed = mxGetScalar(p_optionals[3]);
  igraph_bool_t loops = mxGetScalar(p_optionals[4]);
  igraph_error_t errcode;

  if (edge_method) {
    igraph_integer_t n_edges = mxGetScalar(p_optionals[2]);
    errcode = igraph_erdos_renyi_game_gnm(graph, n_nodes, n_edges, directed,
                                          loops);
  } else {
    igraph_real_t probability = mxGetScalar(p_optionals[2]);
    errcode = igraph_erdos_renyi_game_gnp(graph, n_nodes, probability, directed,
                                          loops);
  }

  return errcode;
}

static igraph_error_t mxIgraph_watts_strogatz_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t dim = mxGetScalar(p_optionals[0]);
  igraph_integer_t size = mxGetScalar(p_optionals[1]);
  igraph_integer_t n_neighbors = mxGetScalar(p_optionals[2]);
  igraph_real_t probability = mxGetScalar(p_optionals[3]);
  igraph_bool_t loops = mxGetScalar(p_optionals[4]);
  igraph_error_t errcode;

  errcode = igraph_watts_strogatz_game(graph, dim, size, n_neighbors,
                                       probability, loops, false);

  return errcode;
}

#define DEGSEQ_BODY(algo)						\
  igraph_vector_int_t outDegree;					\
  igraph_vector_int_t inDegree;						\
  igraph_error_t errcode;						\
									\
  mxIgraphGetVectorInt(p_optionals[0], &outDegree);			\
  mxIgraphGetVectorInt(p_optionals[1], &inDegree);			\
  errcode = igraph_degree_sequence_game(graph, &outDegree, &inDegree,	\
					(algo));			\
									\
  igraph_vector_int_destroy(&outDegree);				\
  igraph_vector_int_destroy(&inDegree);					\
									\
  return errcode

static igraph_error_t mxIgraph_degseq_configuration_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_CONFIGURATION);
}

static igraph_error_t mxIgraph_degseq_configuration_simple_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_CONFIGURATION_SIMPLE);
}

static igraph_error_t mxIgraph_degseq_fast_heur_simple_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_FAST_HEUR_SIMPLE);
}

static igraph_error_t mxIgraph_degseq_edge_switching_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_EDGE_SWITCHING_SIMPLE);
}

static igraph_error_t mxIgraph_degseq_vl_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  DEGSEQ_BODY(IGRAPH_DEGSEQ_VL);
}

static igraph_error_t mxIgraph_k_regular_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t degree = mxGetScalar(p_optionals[1]);
  igraph_bool_t directed = mxGetScalar(p_optionals[2]);
  igraph_error_t error;

  error = igraph_k_regular_game(graph, n_nodes, degree, directed, false);

  return error;
}

static igraph_error_t mxIgraph_static_fitness_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_edges = mxGetScalar(p_optionals[0]);
  igraph_vector_t fitness_out;
  igraph_vector_t fitness_in;
  igraph_vector_t *fitness_in_ptr = NULL;
  igraph_bool_t loops = mxGetScalar(p_optionals[3]);
  igraph_error_t error;

  mxIgraphGetVector(p_optionals[1], &fitness_out);
  mxIgraphGetVector(p_optionals[2], &fitness_in);

  if (!mxIgraphIsEmpty(p_optionals[2])) {
    fitness_in_ptr = &fitness_in;
  }

  error = igraph_static_fitness_game(graph, n_edges, &fitness_out,
                                     fitness_in_ptr, loops, false);

  igraph_vector_destroy(&fitness_out);
  igraph_vector_destroy(&fitness_in);

  return error;
}

static igraph_error_t mxIgraph_static_power_law_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_edges = mxGetScalar(p_optionals[1]);
  igraph_real_t exponent_out = mxGetScalar(p_optionals[2]);
  igraph_real_t exponent_in = mxGetScalar(p_optionals[3]);
  igraph_bool_t loops = mxGetScalar(p_optionals[4]);
  igraph_bool_t finite_size_correction = mxGetScalar(p_optionals[5]);
  igraph_error_t error;

  error = igraph_static_power_law_game(graph, n_nodes, n_edges, exponent_out,
                                       exponent_in, loops, false,
                                       finite_size_correction);

  return error;
}
static igraph_error_t mxIgraph_forest_fire_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_real_t fw_prob = mxGetScalar(p_optionals[1]);
  igraph_real_t bw_factor = mxGetScalar(p_optionals[2]);
  igraph_integer_t ambs = mxGetScalar(p_optionals[3]);
  igraph_bool_t directed = mxGetScalar(p_optionals[4]);
  igraph_error_t error;

  error = igraph_forest_fire_game(graph, n_nodes, fw_prob, bw_factor, ambs,
                                  directed);

  return error;
}

static igraph_error_t mxIgraph_growing_random_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_edges = mxGetScalar(p_optionals[1]);
  igraph_bool_t directed = mxGetScalar(p_optionals[2]);
  igraph_bool_t citation = mxGetScalar(p_optionals[3]);
  igraph_error_t error;

  error = igraph_growing_random_game(graph, n_nodes, n_edges, directed,
                                     citation);

  return error;
}

static igraph_error_t mxIgraph_callaway_traits_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_types = mxGetScalar(p_optionals[1]);
  igraph_integer_t edges_per_step = mxGetScalar(p_optionals[2]);
  igraph_vector_t type_dist;
  igraph_vector_t *type_dist_ptr = NULL;
  igraph_matrix_t pref_matrix;
  igraph_bool_t directed = mxGetScalar(p_optionals[5]);
  igraph_error_t error;

  mxIgraphGetVector(p_optionals[3], &type_dist);
  mxIgraphGetMatrix(p_optionals[4], &pref_matrix);

  if (!mxIgraphIsEmpty(p_optionals[3])) {
    type_dist_ptr = &type_dist;
  }

  error = igraph_callaway_traits_game(graph, n_nodes, n_types, edges_per_step,
                                      type_dist_ptr, &pref_matrix, directed, NULL);

  igraph_vector_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);

  return error;
}

static igraph_error_t mxIgraph_establishment_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_types = mxGetScalar(p_optionals[1]);
  igraph_integer_t edges_per_step = mxGetScalar(p_optionals[2]);
  igraph_vector_t type_dist;
  igraph_vector_t *type_dist_ptr = NULL;
  igraph_matrix_t pref_matrix;
  igraph_bool_t directed = mxGetScalar(p_optionals[5]);
  igraph_error_t error;

  mxIgraphGetVector(p_optionals[3], &type_dist);
  mxIgraphGetMatrix(p_optionals[4], &pref_matrix);

  if (!mxIgraphIsEmpty(p_optionals[3])) {
    type_dist_ptr = &type_dist;
  }

  error = igraph_establishment_game(graph, n_nodes, n_types, edges_per_step,
                                    type_dist_ptr, &pref_matrix, directed, NULL);

  igraph_vector_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);

  return error;
}

static igraph_error_t mxIgraph_preference_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_types = mxGetScalar(p_optionals[1]);
  igraph_vector_t type_dist;
  igraph_vector_t *type_dist_ptr = NULL;
  igraph_bool_t fixed_sizes = mxGetScalar(p_optionals[3]);
  igraph_matrix_t pref_matrix;
  igraph_bool_t directed = mxGetScalar(p_optionals[5]);
  igraph_bool_t loops = mxGetScalar(p_optionals[6]);
  igraph_error_t error;

  mxIgraphGetVector(p_optionals[2], &type_dist);
  mxIgraphGetMatrix(p_optionals[4], &pref_matrix);

  if (!mxIgraphIsEmpty(p_optionals[2])) {
    type_dist_ptr = &type_dist;
  }

  error = igraph_preference_game(graph, n_nodes, n_types, type_dist_ptr,
                                 fixed_sizes, &pref_matrix, NULL, directed,
                                 loops);

  igraph_vector_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);

  return error;
}

static igraph_error_t mxIgraph_asymmetric_preference_i(
  const mxArray *p_optionals[],
  igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_out_types = mxGetScalar(p_optionals[1]);
  igraph_integer_t n_in_types = mxGetScalar(p_optionals[2]);
  igraph_matrix_t type_dist;
  igraph_matrix_t *type_dist_ptr = NULL;
  igraph_matrix_t pref_matrix;
  igraph_bool_t loops = mxGetScalar(p_optionals[5]);
  igraph_error_t error;

  mxIgraphGetMatrix(p_optionals[3], &type_dist);
  mxIgraphGetMatrix(p_optionals[4], &pref_matrix);

  if (!mxIgraphIsEmpty(p_optionals[2])) {
    type_dist_ptr = &type_dist;
  }

  error = igraph_asymmetric_preference_game(graph, n_nodes, n_out_types,
          n_in_types, type_dist_ptr, &pref_matrix, NULL, NULL,
          loops);

  igraph_matrix_destroy(&type_dist);
  igraph_matrix_destroy(&pref_matrix);

  return error;
}

static igraph_error_t mxIgraph_recent_degree_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_real_t power = mxGetScalar(p_optionals[1]);
  igraph_integer_t time_window = mxGetScalar(p_optionals[2]);
  igraph_integer_t edges_per_time_step = mxGetScalar(p_optionals[3]);
  igraph_vector_int_t outseq;
  igraph_vector_int_t *outseq_ptr = NULL;
  igraph_bool_t outpref = mxGetScalar(p_optionals[5]);
  igraph_real_t zero_appeal = mxGetScalar(p_optionals[6]);
  igraph_bool_t directed = mxGetScalar(p_optionals[7]);
  igraph_error_t error;

  mxIgraphGetVectorInt(p_optionals[4], &outseq);

  if (!mxIgraphIsEmpty(p_optionals[4])) {
    outseq_ptr = &outseq;
  }

  error = igraph_recent_degree_game(graph, n_nodes, power, time_window,
                                    edges_per_time_step, outseq_ptr, outpref,
                                    zero_appeal, directed);

  igraph_vector_int_destroy(&outseq);

  return error;
}

static igraph_error_t mxIgraph_barabasi_aging_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t edges_per_time_step = mxGetScalar(p_optionals[1]);
  igraph_vector_int_t outseq;
  igraph_bool_t outpref = mxGetScalar(p_optionals[3]);
  igraph_real_t pa_exp = mxGetScalar(p_optionals[4]);
  igraph_real_t aging_exp = mxGetScalar(p_optionals[5]);
  igraph_integer_t aging_bins = mxGetScalar(p_optionals[6]);
  igraph_real_t zero_deg_appeal = mxGetScalar(p_optionals[7]);
  igraph_real_t zero_age_appeal = mxGetScalar(p_optionals[8]);
  igraph_real_t deg_coef = mxGetScalar(p_optionals[9]);
  igraph_real_t age_coef = mxGetScalar(p_optionals[10]);
  igraph_bool_t directed = mxGetScalar(p_optionals[11]);
  igraph_error_t error;

  mxIgraphGetVectorInt(p_optionals[2], &outseq);

  error = igraph_barabasi_aging_game(graph, n_nodes, edges_per_time_step,
                                     &outseq, outpref, pa_exp, aging_exp,
                                     aging_bins, zero_deg_appeal,
                                     zero_age_appeal, deg_coef, age_coef,
                                     directed);
  igraph_vector_int_destroy(&outseq);

  return error;
}

static igraph_error_t mxIgraph_recent_degree_aging_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t n_edges = mxGetScalar(p_optionals[1]);
  igraph_vector_int_t outseq;
  igraph_bool_t outpref = mxGetScalar(p_optionals[3]);
  igraph_real_t pa_exp = mxGetScalar(p_optionals[4]);
  igraph_real_t aging_exp = mxGetScalar(p_optionals[5]);
  igraph_integer_t aging_bins = mxGetScalar(p_optionals[6]);
  igraph_integer_t time_window = mxGetScalar(p_optionals[7]);
  igraph_real_t zero_appeal = mxGetScalar(p_optionals[8]);
  igraph_bool_t directed = mxGetScalar(p_optionals[9]);
  igraph_error_t error;

  mxIgraphGetVectorInt(p_optionals[2], &outseq);

  error = igraph_recent_degree_aging_game(graph, n_nodes, n_edges, &outseq,
                                          outpref, pa_exp, aging_exp,
                                          aging_bins, time_window, zero_appeal,
                                          directed);

  igraph_vector_int_destroy(&outseq);

  return error;
}

static igraph_error_t mxIgraph_lastcit_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t edges_per_node = mxGetScalar(p_optionals[1]);
  igraph_integer_t aging_bins = mxGetScalar(p_optionals[2]);
  igraph_vector_t preference;
  igraph_bool_t directed = mxGetScalar(p_optionals[4]);
  igraph_error_t error;

  mxIgraphGetVector(p_optionals[3], &preference);

  error = igraph_lastcit_game(graph, n_nodes, edges_per_node, aging_bins,
                              &preference, directed);

  igraph_vector_destroy(&preference);

  return error;
}

static igraph_error_t mxIgraph_cited_type_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_vector_int_t types;
  igraph_vector_t preferences;
  igraph_integer_t edges_per_step = mxGetScalar(p_optionals[3]);
  igraph_bool_t directed = mxGetScalar(p_optionals[4]);
  igraph_error_t error;

  mxIgraphGetVectorInt(p_optionals[1], &types);
  mxIgraphGetVector(p_optionals[2], &preferences);

  error = igraph_cited_type_game(graph, n_nodes, &types, &preferences,
                                 edges_per_step, directed);

  igraph_vector_int_destroy(&types);
  igraph_vector_destroy(&preferences);

  return error;
}

static igraph_error_t mxIgraph_citing_cited_type_i(const mxArray
    *p_optionals[], igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_vector_int_t types;
  igraph_matrix_t pref_matrix;
  igraph_integer_t edges_per_step = mxGetScalar(p_optionals[3]);
  igraph_bool_t directed = mxGetScalar(p_optionals[4]);
  igraph_error_t error;

  mxIgraphGetVectorInt(p_optionals[1], &types);
  mxIgraphGetMatrix(p_optionals[2], &pref_matrix);

  error = igraph_citing_cited_type_game(graph, n_nodes, &types, &pref_matrix,
                                        edges_per_step, directed);

  igraph_vector_int_destroy(&types);
  igraph_matrix_destroy(&pref_matrix);

  return error;
}

static igraph_error_t mxIgraph_sbm_i(const mxArray *p_optionals[],
                                     igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_matrix_t pref_matrix;
  igraph_vector_int_t block_sizes;
  igraph_bool_t directed = mxGetScalar(p_optionals[3]);
  igraph_bool_t loops = mxGetScalar(p_optionals[4]);
  igraph_error_t error;

  mxIgraphGetMatrix(p_optionals[1], &pref_matrix);
  mxIgraphGetVectorInt(p_optionals[2], &block_sizes);

  error = igraph_sbm_game(graph, n_nodes, &pref_matrix, &block_sizes, directed,
                          loops);

  igraph_matrix_destroy(&pref_matrix);
  igraph_vector_int_destroy(&block_sizes);

  return error;
}

static igraph_error_t mxIgraph_hsbm_i(const mxArray *p_optionals[],
                                      igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t edges_per_block = mxGetScalar(p_optionals[1]);
  igraph_vector_t rho;
  igraph_matrix_t c;
  igraph_real_t p = mxGetScalar(p_optionals[4]);
  igraph_error_t error;

  mxIgraphGetVector(p_optionals[2], &rho);
  mxIgraphGetMatrix(p_optionals[3], &c);

  error = igraph_hsbm_game(graph, n_nodes, edges_per_block, &rho, &c, p);

  igraph_vector_destroy(&rho);
  igraph_matrix_destroy(&c);

  return error;
}

static igraph_error_t mxIgraph_dot_product_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_matrix_t vecs;
  igraph_bool_t directed = mxGetScalar(p_optionals[1]);
  igraph_error_t error;

  mxIgraphGetMatrix(p_optionals[0], &vecs);

  error = igraph_dot_product_game(graph, &vecs, directed);

  igraph_matrix_destroy(&vecs);

  return error;
}

static igraph_error_t mxIgraph_tree_i(const mxArray *p_optionals[],
                                      igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_bool_t directed = mxGetScalar(p_optionals[1]);
  igraph_integer_t const n_methods = 2;
  const char *methods[2] = {
    [IGRAPH_RANDOM_TREE_PRUFER] = "prufer",
    [IGRAPH_RANDOM_TREE_LERW] = "lerw"
  };
  igraph_random_tree_t method = mxIgraphSelectMethod(p_optionals[2], methods,
                                n_methods);

  if (method >= n_methods) {
    mexErrMsgIdAndTxt("igraph:internal:unknownMethod",
                      "Unknown method passed to tree game.");
  }

  return igraph_tree_game(graph, n_nodes, directed, method);
}

static igraph_error_t mxIgraph_simple_interconnected_islands_i(
  const mxArray *p_optionals[], igraph_t *graph)
{
  igraph_integer_t n_islands = mxGetScalar(p_optionals[0]);
  igraph_integer_t islands_size = mxGetScalar(p_optionals[1]);
  igraph_real_t p_in = mxGetScalar(p_optionals[2]);
  igraph_integer_t n_inter = mxGetScalar(p_optionals[3]);

  return igraph_simple_interconnected_islands_game(graph, n_islands,
         islands_size, p_in, n_inter);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  igraph_integer_t n_required = 3;
  igraph_integer_t n_optionals = nrhs - n_required;
  const mxArray **p_optionals = prhs + n_required;

  VERIFY_N_INPUTS_ATLEAST(n_required);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxIgraph_game_t method;
  igraph_bool_t make_sparse = mxGetScalar(prhs[1]);
  mxIgraphDType_t dtype = mxIgraphSelectDType(prhs[2]);
  igraph_t graph;
  igraph_error_t errorcode;
  igraph_error_t (*game_func)(const mxArray **, igraph_t *);

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
    [MXIGRAPH_GAME_DEGSEQ_EDGE_SWITCHING_SIMPLE] = "degseqedgeswitchingsimple",
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
    [MXIGRAPH_GAME_SIMPLE_INTERCONNECTED_ISLANDS] = "simpleinterconnectedislands",
  };

  method = mxIgraphSelectMethod(prhs[0], games, MXIGRAPH_GAME_N);

  switch (method) {
  case MXIGRAPH_GAME_GRG:
    VERIFY_N_OPTIONALS(3);
    game_func = mxIgraph_grg_i;
    break;
  case MXIGRAPH_GAME_BARABASI_BAG:
    VERIFY_N_OPTIONALS(7);
    game_func = mxIgraph_barabasi_bag_i;
    break;
  case MXIGRAPH_GAME_BARABASI_PSUMTREE:
    VERIFY_N_OPTIONALS(7);
    game_func = mxIgraph_barabasi_psumtree_i;
    break;
  case MXIGRAPH_GAME_BARABASI_PSUMTREE_MULTIPLE:
    VERIFY_N_OPTIONALS(7);
    game_func = mxIgraph_barabasi_psumtree_multiple_i;
    break;
  case MXIGRAPH_GAME_ERDOS_RENYI:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_erdos_renyi_i;
    break;
  case MXIGRAPH_GAME_WATTS_STROGATZ:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_watts_strogatz_i;
    break;
  case MXIGRAPH_GAME_DEGSEQ_CONFIGURATION:
    VERIFY_N_OPTIONALS(2);
    game_func = mxIgraph_degseq_configuration_i;
    break;
  case MXIGRAPH_GAME_DEGSEQ_CONFIGURATION_SIMPLE:
    VERIFY_N_OPTIONALS(2);
    game_func = mxIgraph_degseq_configuration_simple_i;
    break;
  case MXIGRAPH_GAME_DEGSEQ_FAST_HEUR_SIMPLE:
    VERIFY_N_OPTIONALS(2);
    game_func = mxIgraph_degseq_fast_heur_simple_i;
    break;
  case MXIGRAPH_GAME_DEGSEQ_EDGE_SWITCHING_SIMPLE:
    VERIFY_N_OPTIONALS(2);
    game_func = mxIgraph_degseq_edge_switching_i;
    break;
  case MXIGRAPH_GAME_DEGSEQ_VL:
    VERIFY_N_OPTIONALS(2);
    game_func = mxIgraph_degseq_vl_i;
    break;
  case MXIGRAPH_GAME_K_REGULAR:
    VERIFY_N_OPTIONALS(3);
    game_func = mxIgraph_k_regular_i;
    break;
  case MXIGRAPH_GAME_STATIC_FITNESS:
    VERIFY_N_OPTIONALS(4);
    game_func = mxIgraph_static_fitness_i;
    break;
  case MXIGRAPH_GAME_STATIC_POWER_LAW:
    VERIFY_N_OPTIONALS(6);
    game_func = mxIgraph_static_power_law_i;
    break;
  case MXIGRAPH_GAME_FOREST_FIRE:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_forest_fire_i;
    break;
  case MXIGRAPH_GAME_GROWING_RANDOM:
    VERIFY_N_OPTIONALS(4);
    game_func = mxIgraph_growing_random_i;
    break;
  case MXIGRAPH_GAME_CALLAWAY_TRAITS:
    VERIFY_N_OPTIONALS(6);
    game_func = mxIgraph_callaway_traits_i;
    break;
  case MXIGRAPH_GAME_ESTABLISHMENT:
    VERIFY_N_OPTIONALS(6);
    game_func = mxIgraph_establishment_i;
    break;
  case MXIGRAPH_GAME_PREFERENCE:
    VERIFY_N_OPTIONALS(7);
    game_func = mxIgraph_preference_i;
    break;
  case MXIGRAPH_GAME_ASYMMETRIC_PREFERENCE:
    VERIFY_N_OPTIONALS(6);
    game_func = mxIgraph_asymmetric_preference_i;
    break;
  case MXIGRAPH_GAME_RECENT_DEGREE:
    VERIFY_N_OPTIONALS(8);
    game_func = mxIgraph_recent_degree_i;
    break;
  case MXIGRAPH_GAME_BARABASI_AGING:
    VERIFY_N_OPTIONALS(12);
    game_func = mxIgraph_barabasi_aging_i;
    break;
  case MXIGRAPH_GAME_RECENT_DEGREE_AGING:
    VERIFY_N_OPTIONALS(10);
    game_func = mxIgraph_recent_degree_aging_i;
    break;
  case MXIGRAPH_GAME_LASTCIT:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_lastcit_i;
    break;
  case MXIGRAPH_GAME_CITED_TYPE:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_cited_type_i;
    break;
  case MXIGRAPH_GAME_CITING_CITED_TYPE:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_citing_cited_type_i;
    break;
  case MXIGRAPH_GAME_SBM:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_sbm_i;
    break;
  case MXIGRAPH_GAME_HSBM:
    VERIFY_N_OPTIONALS(5);
    game_func = mxIgraph_hsbm_i;
    break;
  case MXIGRAPH_GAME_DOT_PRODUCT:
    VERIFY_N_OPTIONALS(2);
    game_func = mxIgraph_dot_product_i;
    break;
  case MXIGRAPH_GAME_TREE:
    VERIFY_N_OPTIONALS(3);
    game_func = mxIgraph_tree_i;
    break;
  case MXIGRAPH_GAME_SIMPLE_INTERCONNECTED_ISLANDS:
    VERIFY_N_OPTIONALS(4);
    game_func = mxIgraph_simple_interconnected_islands_i;
    break;
  case MXIGRAPH_GAME_N:
    mxIgraphErrorUnknownMethod(mexFunctionName(), mxArrayToString(prhs[2]));
    exit(1);
  default:
    mxIgraphErrorNotImplemented("Game", mxArrayToString(prhs[2]));
    exit(1);
  }

  if ((errorcode = game_func(p_optionals, &graph)) != IGRAPH_SUCCESS) {
    igraph_destroy(&graph);
    switch (errorcode) {
    case IGRAPH_ENOMEM:
      mexErrMsgIdAndTxt("Igraph:outOfMemory",
                        "Failed to generate graph due to insufficient memory.");
      break;
    case IGRAPH_EINVAL:
      mexErrMsgIdAndTxt("Igraph:invalidParameter", "Invalid method parameter.");
      break;
    default:
      mexErrMsgIdAndTxt("Igraph:internal:generate",
                        "Failed to generate a %s graph.", games[method]);
    }
  }

  plhs[0] = mxIgraphCreateAdj(&graph, NULL, make_sparse, dtype);

  igraph_destroy(&graph);
}

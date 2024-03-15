#include "mxIgraph.h"

static inline igraph_real_t harmonic_number_i(size_t const n,
    igraph_real_t const alpha)
{
  igraph_real_t h = 0;
  for (size_t k = 1; k <= n; k++) {
    h += 1.0 / pow(k, alpha);
  }

  return h;
}

static inline igraph_real_t harmonic_number_weighted_i(size_t const n,
    size_t const weight, igraph_real_t const alpha)
{
  igraph_real_t h = 0;
  for (size_t k = 1; k <= n; k++) {
    h += (double)(k * weight) / pow(k, alpha);
  }

  return h;
}

/* Everywhere Zipf's distribution is used for the power law distribution. This
   is a discrete distribution with an lower bound of 1 and a variable upper
   bound. */
static inline igraph_real_t power_law_mean_i(size_t const k_min,
    size_t const k_max, igraph_real_t const alpha)
{
  size_t const n = k_max / k_min;
  return harmonic_number_weighted_i(n, k_min, alpha) /
         harmonic_number_i(n, alpha);
}

/* The derivative of the power law's mean with respect to k_min.
   Since k_max is fixed N is a function of k_min (i.e. k_max - k_min + 1). */
static inline igraph_real_t dpower_law_mean_i(size_t const k_min,
    size_t const k_max, igraph_real_t const alpha)
{
  size_t const n = k_max / k_min;
  igraph_real_t mean_back, mean_forward;

  if ((n > 1) && (n < k_max)) {
    size_t const k_back = k_max / (n + 1);
    size_t const k_forward = k_max / (n - 1);
    mean_back = power_law_mean_i(k_forward, k_max, alpha);
    mean_forward = power_law_mean_i(k_back, k_max, alpha);
    return (mean_forward - mean_back) / (k_forward - k_back);
  }

  if (n > 1) {
    size_t const k_forward = k_max / (n - 1);
    mean_back = power_law_mean_i(k_min, k_max, alpha);
    mean_forward = power_law_mean_i(k_forward, k_max, alpha);
    return (mean_forward - mean_back) / (k_forward - k_min);
  }

  size_t const k_back = k_max / (n + 1);
  mean_back = power_law_mean_i(k_back, k_max, alpha);
  mean_forward = power_law_mean_i(k_min, k_max, alpha);

  return (mean_forward - mean_back) / (k_min - k_back);
}

// Calculate minimum degree such that that average of the distribution
// approaches the provided average.
static size_t power_law_find_min_i(igraph_real_t const expected,
                                   size_t const k_max,
                                   igraph_real_t const exponent)
{
  size_t k_min = expected;
  igraph_real_t avg_actual[2] = { 0, 0 };
  igraph_real_t avg_slope, avg_error;
  igraph_real_t delta;

  if (power_law_mean_i(1, k_max, exponent) > expected) {
    mexErrMsgIdAndTxt("igraph:doesNotConverge",
                      "Requested mean degree's is too small. "
                      "The minimum average degrees a power law distribution "
                      "can produce for a graph with the given number of "
                      "nodes is %g.",
                      ceil(power_law_mean_i(1, k_max, exponent)));
  }

  if (power_law_mean_i(k_max - 1, k_max, exponent) < expected) {
    mexErrMsgIdAndTxt("igraph:doesNotConverge",
                      "Requested mean degree's is too large. "
                      "The maximum average degrees a power law distribution "
                      "can produce for a graph with the given number of "
                      "nodes is %g.",
                      floor(power_law_mean_i(k_max - 1, k_max, exponent)));
  }

  avg_actual[0] = power_law_mean_i(k_min - 1, k_max, exponent);
  avg_actual[1] = power_law_mean_i(k_min, k_max, exponent);

  while (!((expected >= avg_actual[0]) && (expected <= avg_actual[1]))) {
    avg_slope = dpower_law_mean_i(k_min, k_max, exponent);
    avg_error = expected - avg_actual[1];
    delta = avg_error / avg_slope;

    if ((k_min - 2) < delta) {
      k_min = 2;
    } else if (ceil((double)k_min - delta) >= k_max) {
      k_min = k_max - 1;
    } else {
      k_min = ceil((double)k_min - delta);
    }

    avg_actual[0] = power_law_mean_i(k_min - 1, k_max, exponent);
    avg_actual[1] = power_law_mean_i(k_min, k_max, exponent);
  }

  if ((expected - avg_actual[0]) < (avg_actual[1] - expected)) {
    return k_min - 1;
  }
  return k_min;

}

static void power_law_cum_distribution_i(igraph_vector_t* cum_probabilities,
    size_t const k_min, size_t const k_max,
    igraph_real_t const alpha)
{
  size_t const n = k_max / k_min;
  igraph_real_t const h = harmonic_number_i(n, alpha);

  igraph_vector_init(cum_probabilities, n);
  for (size_t k = 1; k <= n; k++) {
    VECTOR(*cum_probabilities)[k - 1] = 1.0 / (h * pow(k, alpha));
  }

  for (size_t k = 1; k < n; k++) {
    VECTOR(*cum_probabilities)[k] += VECTOR(*cum_probabilities)[k - 1];
  }
}

static inline size_t sample_power_law_distribution_i(
  igraph_vector_t const* cum_probabilities, size_t const k_min)
{
  igraph_real_t sample = igraph_rng_get_unif01(igraph_rng_default());
  size_t loc = 0;
  while (VECTOR(*cum_probabilities)[loc] < sample) loc++;
  return (loc + 1) * k_min;
}

static void sample_node_degrees_i(igraph_vector_int_t* degrees,
                                  igraph_integer_t const n_nodes, size_t const k_min,
                                  size_t const k_max, igraph_real_t const alpha)
{
  igraph_vector_t cum_probabilities;
  power_law_cum_distribution_i(&cum_probabilities, k_min, k_max, alpha);
  for (igraph_integer_t i = 0; i < n_nodes; i++) {
    VECTOR(*degrees)[i] = sample_power_law_distribution_i(&cum_probabilities,
                          k_min);
  }

  igraph_vector_destroy(&cum_probabilities);
}

igraph_integer_t static sample_communities_sizes_i(
  igraph_vector_int_t* community_sizes, igraph_integer_t n_communities,
  igraph_integer_t const n_nodes, size_t const k_min, size_t const k_max,
  size_t const alpha)
{
  igraph_integer_t graph_size = 0;
  igraph_vector_t cum_probabilities;
  power_law_cum_distribution_i(&cum_probabilities, k_min, k_max, alpha);
  size_t loc = 0;
  while (graph_size < n_nodes) {
    if (loc == n_communities) {
      n_communities = ceil((double)n_communities * 1.2);
      igraph_vector_int_resize(community_sizes, n_communities);
    }

    VECTOR(*community_sizes)[loc] = sample_power_law_distribution_i(
                                      &cum_probabilities, k_min);
    graph_size += VECTOR(*community_sizes)[loc];
    loc++;
  }

  // Last community must always be at least as big as the difference.
  VECTOR(*community_sizes)[loc] -= (graph_size - n_nodes);

  igraph_vector_destroy(&cum_probabilities);
  igraph_vector_int_resize(community_sizes, loc);

  return loc;
}

static igraph_error_t place_nodes_i(igraph_vector_int_t const* degrees,
                                    igraph_integer_t const n_nodes,
                                    igraph_vector_int_t const* community_sizes,
                                    igraph_integer_t const n_communities,
                                    igraph_vector_int_list_t* comm_list)
{
  igraph_error_t errorcode = IGRAPH_SUCCESS;
  igraph_vector_int_t community_space;

  igraph_vector_int_init(&community_space, n_communities);
  for (igraph_integer_t i = 0; i < n_communities; i++) {
    VECTOR(community_space)[i] = VECTOR(*community_sizes)[i];
  }

  if ((errorcode = igraph_vector_int_list_init(comm_list, n_communities))) {
    return errorcode;
  }

  for (size_t i = 0; i < n_communities; i++) {
    igraph_vector_int_t* ptr = igraph_vector_int_list_get_ptr(comm_list, i);
    igraph_vector_int_init(ptr, VECTOR(community_space)[i]);
  }

  igraph_vector_int_t order_idx;
  igraph_vector_int_init(&order_idx, n_nodes);
  igraph_vector_int_qsort_ind(degrees, &order_idx, IGRAPH_DESCENDING);

  for (igraph_integer_t i = 0; i < n_nodes; i++) {
    igraph_integer_t rand_comm;
    igraph_integer_t degree = VECTOR(*degrees)[VECTOR(order_idx)[i]];
    do {
      rand_comm = igraph_rng_get_integer(igraph_rng_default(), 0,
                                         n_communities - 1);
    } while ((degree > VECTOR(*community_sizes)[rand_comm]) ||
             (VECTOR(community_space)[rand_comm] == 0));

    igraph_vector_int_t* comm = igraph_vector_int_list_get_ptr(comm_list,
                                rand_comm);
    VECTOR(*comm)[--VECTOR(community_space)[rand_comm]] = VECTOR(order_idx)[i];
  }

  igraph_vector_int_destroy(&community_space);

  return errorcode;
}

static void list_to_membership_i(igraph_vector_int_list_t const* comm_list,
                                 igraph_integer_t const n_communities,
                                 igraph_vector_int_t* memb,
                                 igraph_integer_t const n_nodes)
{
  igraph_vector_int_init(memb, n_nodes);
  for (igraph_integer_t i = 0; i < n_communities; i++) {
    igraph_vector_int_t* comm = igraph_vector_int_list_get_ptr(comm_list, i);
    for (igraph_integer_t j = 0; j < igraph_vector_int_size(comm); j++) {
      VECTOR(*memb)[VECTOR(*comm)[j]] = i;
    }
  }
}

static inline igraph_integer_t internal_links(
  igraph_integer_t const total_degree, igraph_real_t const mu)
{
  return floor((igraph_real_t)total_degree * mu);
}

static inline igraph_integer_t external_links(
  igraph_integer_t const total_degree, igraph_real_t const mu)
{
  return total_degree - floor((igraph_real_t)total_degree * mu);
}

static inline void shuffle_nodes_i(igraph_vector_int_t* nodes, size_t const i,
                                   size_t const j)
{
  size_t tmp = VECTOR(*nodes)[i];
  VECTOR(*nodes)[i] = VECTOR(*nodes)[j];
  VECTOR(*nodes)[j] = tmp;
}

static inline igraph_bool_t in_edges_i(igraph_integer_t const start,
                                       igraph_integer_t const end,
                                       igraph_vector_int_t const* edges,
                                       igraph_integer_t const head,
                                       igraph_integer_t const tail)
{
  for (igraph_integer_t i = start; i < end; i++) {
    if (VECTOR(*edges)[i] == head) {
      if ((i & 1) && (VECTOR(*edges)[i - 1] == tail)) {
        return true;
      }

      if (!(i & 1) && (VECTOR(*edges)[i + 1] == tail)) {
        return true;
      }
    }
  }

  return false;
}

static inline igraph_integer_t add_internal_edges_i(
  igraph_vector_int_t* edges,
  igraph_integer_t n_edges,
  igraph_vector_int_t const* members,
  igraph_vector_int_t const* degrees,
  igraph_real_t const mu)
{
  igraph_integer_t const edge_start = n_edges;
  igraph_integer_t n_nodes = igraph_vector_int_size(members);
  igraph_vector_int_t comm_degrees;
  igraph_vector_int_t idx;

  igraph_vector_int_init(&comm_degrees, n_nodes);
  for (igraph_integer_t i = 0; i < n_nodes; i++) {
    VECTOR(comm_degrees)[i] = internal_links(
                                VECTOR(*degrees)[VECTOR(*members)[i]], mu);
  }

  igraph_vector_int_init(&idx, n_nodes);
  for (igraph_integer_t i = 0; i < n_nodes; i++) {
    VECTOR(idx)[i] = i;
  }

  while (n_nodes > 1) {
    igraph_integer_t head_idx = igraph_rng_get_integer(igraph_rng_default(), 0,
                                n_nodes);
    igraph_integer_t tail_idx = igraph_rng_get_integer(igraph_rng_default(), 0,
                                n_nodes);

    igraph_integer_t head = VECTOR(idx)[head_idx];
    igraph_integer_t tail = VECTOR(idx)[tail_idx];

    if (VECTOR(comm_degrees)[head] && VECTOR(comm_degrees)[tail]) {
      VECTOR(*edges)[n_edges++] = VECTOR(*members)[head];
      VECTOR(*edges)[n_edges++] = VECTOR(*members)[tail];
      VECTOR(comm_degrees)[head]--;
      VECTOR(comm_degrees)[tail]--;

      if (VECTOR(comm_degrees)[head] == 0) {
        shuffle_nodes_i(&idx, head_idx, n_nodes - 1);
        n_nodes--;
      }

      if (VECTOR(comm_degrees)[tail] == 0) {
        shuffle_nodes_i(&idx, tail_idx, n_nodes - 1);
        n_nodes--;
      }
    }
  }

  igraph_vector_int_destroy(&idx);
  igraph_vector_int_destroy(&comm_degrees);

  return n_edges;
}

/* static inline void add_external_edges_i(); */

static void add_edges_i(igraph_vector_int_t* edges,
                        igraph_vector_int_t* degrees,
                        igraph_vector_int_list_t* const comm_list,
                        igraph_vector_int_t* const comm_sizes,
                        igraph_real_t const mu)
{
  igraph_integer_t const n_comms = igraph_vector_int_size(comm_sizes);
  igraph_integer_t n_edges = 0;

  for (igraph_integer_t i = 0; i < n_comms; i++) {
    n_edges = add_internal_edges_i(edges, n_edges,
                                   igraph_vector_int_list_get_ptr(comm_list, i),
                                   degrees, mu);
  }

  /* for (igraph_integer_t i = 0; i < n_comms; i++) { */
  /*   add_external_edges_i(); */
  /* } */
}

static igraph_error_t benchmark_lfr(igraph_t* graph,
                                    igraph_vector_int_t* memb,
                                    mxArray const* options)
{
  igraph_integer_t const n_nodes = mxIgraphGetInteger(options, "nNodes");
  igraph_real_t const mu = mxIgraphGetReal(options, "mixing");
  igraph_real_t const degree_mean = mxIgraphGetReal(options, "meanDegree");
  igraph_real_t const gamma = mxIgraphGetReal(options, "degreeExponent");
  igraph_real_t const beta = mxIgraphGetReal(options, "communityExponent");
  igraph_bool_t const dry_run = mxIgraphGetBool(options, "dryRun");

  size_t const degree_max = n_nodes - 1;
  size_t const degree_min = power_law_find_min_i(degree_mean, degree_max,
                            gamma);
  size_t const community_size_min = degree_min * 4 / 3;
  size_t const community_size_max = degree_max;
  size_t const max_iters = 5;

  igraph_vector_int_t degrees;
  igraph_vector_int_t community_sizes;
  igraph_vector_int_list_t comm_list;
  igraph_vector_int_t edges;
  igraph_integer_t n_edges = 0;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  if ((errorcode = igraph_vector_int_init(&degrees, n_nodes))) {
    return errorcode;
  }

  igraph_integer_t n_communities = ceil((double)n_nodes / degree_min);
  if ((errorcode = igraph_vector_int_init(&community_sizes,
                                          ceil((double)n_nodes / degree_min)))) {
    return errorcode;
  }

  sample_node_degrees_i(&degrees, n_nodes, degree_min, degree_max, gamma);
  size_t iter = 0;
  do {
    iter++;
    n_communities = sample_communities_sizes_i(&community_sizes, n_communities,
                    n_nodes, community_size_min, community_size_max, beta);
  } while ((igraph_vector_int_max(&community_sizes) <
            igraph_vector_int_max(&degrees)) && iter < max_iters);

  if (iter == max_iters) {
    mexErrMsgIdAndTxt("igraph:tooManyAttempts",
                      "Failed to create a set of communities big enough "
                      "to hold highest degree node after %ld attempts. "
                      "Try decreesing the community exponent to increase "
                      "the liklihood of sampling a larger community or "
                      "decrease the mean degree to reduce the maximum "
                      "degree sampled.", max_iters);
  }

  if (dry_run) {
    mexPrintf("Goal mean degree: %g, Actual mean degree: %g\n\n", degree_mean,
              (igraph_real_t)igraph_vector_int_sum(&degrees) / n_nodes);

    mexPrintf("Node degrees: [\n    ");
    for (igraph_integer_t i = 0; i < n_nodes; i++) {
      mexPrintf("%ld ", VECTOR(degrees)[i]);
    }
    mexPrintf("\n]\n\n");

    mexPrintf("Community sizes: [\n    ");
    for (igraph_integer_t i = 0; i < n_communities; i++) {
      mexPrintf("%ld ", VECTOR(community_sizes)[i]);
    }
    mexPrintf("\n]\n");

    goto cleanup;
  }

  place_nodes_i(&degrees, n_nodes, &community_sizes, n_communities, &comm_list);
  list_to_membership_i(&comm_list, n_communities, memb, n_nodes);

  n_edges = igraph_vector_int_sum(&degrees);
  if ((errorcode = igraph_vector_int_init(&edges, n_edges))) {
    return errorcode;
  }

  add_edges_i(&edges, &degrees, &comm_list, &community_sizes, mu);

  if ((errorcode = igraph_empty(graph, n_nodes, IGRAPH_UNDIRECTED))) {
    return errorcode;
  }
  igraph_add_vertices(graph, n_nodes, NULL);
  igraph_add_edges(graph, &edges, NULL);

  igraph_vector_int_destroy(&edges);
  for (size_t i = 0; i < n_communities; i++) {
    igraph_vector_int_destroy(igraph_vector_int_list_get_ptr(&comm_list, i));
  }
  igraph_vector_int_list_destroy(&comm_list);

cleanup:
  igraph_vector_int_destroy(&community_sizes);
  igraph_vector_int_destroy(&degrees);

  return errorcode;
}

igraph_error_t mexIgraphBenchmark(int nlhs, mxArray* plhs[], int nrhs,
                                  mxArray const* prhs[])
{
  mxArray const* adj_options = prhs[0];
  mxArray const* method_options = prhs[1];

  igraph_t graph;
  igraph_vector_int_t memb;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  if ((errorcode = benchmark_lfr(&graph, &memb, method_options))) {
    return errorcode;
  }

  plhs[0] = mxIgraphCreateAdj(&graph, NULL, adj_options);
  igraph_destroy(&graph);

  if (nlhs == 2) {
    plhs[1] = mxIgraphMembershipToArray(&memb);
    igraph_vector_int_destroy(&memb);
  }

  return errorcode;
}

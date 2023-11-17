#include <string.h>
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

static igraph_error_t mxIgraph_optimal_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *UNUSED(p_optionals[]),
    igraph_vector_int_t *membership)
{
  igraph_error_t errcode;

  errcode = igraph_community_optimal_modularity(graph, NULL, membership,
            weights);

  return errcode;
}

static igraph_error_t mxIgraph_spinglass_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_integer_t n_spins = mxGetScalar(p_optionals[0]);
  igraph_bool_t parallel = mxGetScalar(p_optionals[1]);
  igraph_real_t start_temp = mxGetScalar(p_optionals[2]);
  igraph_real_t end_temp = mxGetScalar(p_optionals[3]);
  igraph_real_t cool_factor = mxGetScalar(p_optionals[4]);
  igraph_spincomm_update_t update_rule;
  igraph_real_t gamma = mxGetScalar(p_optionals[6]);
  igraph_real_t gamma_minus = mxGetScalar(p_optionals[7]);
  igraph_spinglass_implementation_t implementation =
    igraph_vector_min(weights) < 0 ? IGRAPH_SPINCOMM_IMP_NEG :
    IGRAPH_SPINCOMM_IMP_ORIG;
  igraph_error_t errorcode;

  if (strcmp(mxArrayToString(p_optionals[5]), "simple") == 0) {
    update_rule = IGRAPH_SPINCOMM_UPDATE_SIMPLE;
  } else {
    update_rule = IGRAPH_SPINCOMM_UPDATE_CONFIG;
  }

  errorcode = igraph_community_spinglass(graph, weights, NULL, NULL,
                                         membership, NULL, n_spins, parallel,
                                         start_temp, end_temp, cool_factor,
                                         update_rule, gamma, implementation,
                                         gamma_minus);

  return errorcode;
}

static igraph_error_t mxIgraph_leading_eigenvector_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_integer_t steps = mxGetScalar(p_optionals[0]);
  igraph_vector_int_t init;
  igraph_error_t errcode;

  mxIgraphGetVectorInt(p_optionals[1], &init);
  for (igraph_integer_t i = 0; i < igraph_vcount(graph); i++) {
    VECTOR(*membership)[i] = VECTOR(init)[i];
  }
  igraph_vector_int_destroy(&init);

  errcode = igraph_community_leading_eigenvector(graph, weights, NULL,
            membership, steps, NULL, NULL, true, NULL, NULL, NULL, NULL, NULL);

  return errcode;
}

static igraph_error_t mxIgraph_walktrap_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_integer_t steps = mxGetScalar(p_optionals[0]);
  igraph_error_t errcode;

  errcode = igraph_community_walktrap(graph, weights, steps, NULL, NULL,
                                      membership);

  return errcode;
}

static igraph_error_t mxIgraph_edge_betweenness_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_bool_t isweighted = mxGetScalar(p_optionals[0]);
  igraph_vector_t const *weights_ptr = NULL;
  igraph_error_t errcode;

  if (isweighted) {
    /* Weights cause a warning so sue NULL instead of vector of 1s for
       unweighted graphs. */
    weights_ptr = weights;
  }

  errcode = igraph_community_edge_betweenness(graph, NULL, NULL, NULL,
            NULL, NULL, membership, igraph_is_directed(graph), weights_ptr);

  return errcode;
}

static igraph_error_t mxIgraph_fastgreedy_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *UNUSED(p_optionals[]),
    igraph_vector_int_t *membership)
{
  igraph_error_t errcode;

  errcode = igraph_community_fastgreedy(graph, weights, NULL, NULL,
                                        membership);

  return errcode;
}

static igraph_error_t mxIgraph_multilevel_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_real_t resolution = mxGetScalar(p_optionals[0]);
  igraph_error_t errcode;

  errcode = igraph_community_multilevel(graph, weights, resolution, membership,
                                        NULL, NULL);

  return errcode;
}

static igraph_error_t mxIgraph_leiden_i(const igraph_t *graph,
                                        const igraph_vector_t *weights,
                                        const mxArray *p_optionals[],
                                        igraph_vector_int_t *membership)
{
  igraph_real_t resolution = mxGetScalar(p_optionals[0]);
  igraph_real_t beta = mxGetScalar(p_optionals[1]);
  igraph_vector_t node_weights;
  igraph_vector_t *node_weights_ptr = NULL;
  igraph_integer_t n_iterations = mxGetScalar(p_optionals[2]);
  igraph_bool_t use_modularity = strcmp(mxArrayToString(p_optionals[3]),
                                        "modularity") == 0;
  igraph_vector_int_t init;
  igraph_error_t errcode;

  mxIgraphGetVectorInt(p_optionals[4], &init);
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

  errcode = igraph_community_leiden(graph, weights, node_weights_ptr,
                                    resolution, beta, true, n_iterations,
                                    membership, NULL, NULL);

  if (use_modularity) {
    igraph_vector_destroy(&node_weights);
  }

  return errcode;
}

static igraph_error_t mxIgraph_fluid_communities_i(const igraph_t *graph,
    const igraph_vector_t *UNUSED(weights), const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_integer_t n_communities = mxGetScalar(p_optionals[0]);
  igraph_error_t errcode;

  errcode = igraph_community_fluid_communities(graph, n_communities,
            membership);

  return errcode;
}

static igraph_error_t mxIgraph_label_propagation_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_vector_int_t initial;
  igraph_neimode_t mode = mxIgraphSelectMode(p_optionals[0]);
  igraph_vector_bool_t fixed;
  igraph_error_t errcode;

  mxIgraphGetVectorInt(p_optionals[1], &initial);
  mxIgraphGetVectorBool(p_optionals[2], &fixed);

  errcode = igraph_community_label_propagation(graph, membership, mode, weights,
            &initial, &fixed);

  igraph_vector_int_destroy(&initial);
  igraph_vector_bool_destroy(&fixed);

  return errcode;
}

static igraph_error_t mxIgraph_infomap_i(const igraph_t *graph,
    const igraph_vector_t *weights, const mxArray *p_optionals[],
    igraph_vector_int_t *membership)
{
  igraph_integer_t n_trials = mxGetScalar(p_optionals[0]);
  igraph_vector_t v_weights;
  /* Don't think we need codelength, but crashes Matlab when NULL is used.*/
  igraph_real_t codelength = 0;
  igraph_error_t errcode;

  mxIgraphGetVector(p_optionals[1], &v_weights);

  errcode = igraph_community_infomap(graph, weights, &v_weights, n_trials,
                                     membership, &codelength);

  igraph_vector_destroy(&v_weights);

  return errcode;
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  igraph_integer_t n_required = 3;
  igraph_integer_t n_optionals = nrhs - n_required;
  const mxArray **p_optionals = prhs + n_required;

  VERIFY_N_INPUTS_ATLEAST(n_required);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxIgraph_cluster_t method;
  igraph_t graph;
  igraph_vector_t weights;
  igraph_bool_t directed = mxGetScalar(prhs[2]);
  igraph_vector_int_t membership;
  igraph_error_t errorcode;
  igraph_error_t (*cluster_func)(const igraph_t *, const igraph_vector_t *,
                                 const mxArray **, igraph_vector_int_t *);

  const char *methods[MXIGRAPH_CLUSTER_N] = {
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

  method = mxIgraphSelectMethod(prhs[0], methods, MXIGRAPH_CLUSTER_N);

  mxIgraphGetGraph(prhs[1], &graph, &weights, directed);
  igraph_vector_int_init(&membership, igraph_vcount(&graph));

  switch (method) {
  case MXIGRAPH_CLUSTER_OPTIMAL:
    VERIFY_N_OPTIONALS(0);
    cluster_func = mxIgraph_optimal_i;
    break;
  case MXIGRAPH_CLUSTER_SPINGLASS:
    VERIFY_N_OPTIONALS(8);
    cluster_func = mxIgraph_spinglass_i;
    break;
  case MXIGRAPH_CLUSTER_LEADINGEIGENVECTOR:
    VERIFY_N_OPTIONALS(2);
    cluster_func = mxIgraph_leading_eigenvector_i;
    break;
  case MXIGRAPH_CLUSTER_WALKTRAP:
    VERIFY_N_OPTIONALS(1);
    cluster_func = mxIgraph_walktrap_i;
    break;
  case MXIGRAPH_CLUSTER_EDGEBETWEENNESS:
    VERIFY_N_OPTIONALS(1);
    cluster_func = mxIgraph_edge_betweenness_i;
    break;
  case MXIGRAPH_CLUSTER_FASTGREEDY:
    VERIFY_N_OPTIONALS(0);
    cluster_func = mxIgraph_fastgreedy_i;
    break;
  case MXIGRAPH_CLUSTER_MULTILEVEL:
    VERIFY_N_OPTIONALS(1);
    cluster_func = mxIgraph_multilevel_i;
    break;
  case MXIGRAPH_CLUSTER_LEIDEN:
    VERIFY_N_OPTIONALS(5);
    cluster_func = mxIgraph_leiden_i;
    break;
  case MXIGRAPH_CLUSTER_FLUIDCOMMUNITIES:
    VERIFY_N_OPTIONALS(1);
    cluster_func = mxIgraph_fluid_communities_i;
    break;
  case MXIGRAPH_CLUSTER_LABELPROPAGATION:
    VERIFY_N_OPTIONALS(3);
    cluster_func = mxIgraph_label_propagation_i;
    break;
  case MXIGRAPH_CLUSTER_INFOMAP:
    VERIFY_N_OPTIONALS(2);
    cluster_func = mxIgraph_infomap_i;
    break;
  case MXIGRAPH_CLUSTER_N:
    mxIgraphErrorUnknownMethod(mexFunctionName(), mxArrayToString(prhs[0]));
    exit(1);
  default:
    mexErrMsgIdAndTxt("Igraph:internal:methodNotImplemented",
                      "The clustering method \"%s\" has not been implemented.",
                      mxArrayToString(prhs[0]));
    exit(1);
  }

  errorcode = cluster_func(&graph, &weights, p_optionals, &membership);
  igraph_destroy(&graph);
  igraph_vector_destroy(&weights);

  if (errorcode != IGRAPH_SUCCESS) {
    igraph_vector_int_destroy(&membership);
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
                        "Failed to generate a %s graph.", methods[method]);
    }
  }

  plhs[0] = mxIgraphCreatePartition(&membership);
  igraph_vector_int_destroy(&membership);
}

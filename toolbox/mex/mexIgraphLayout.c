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
  MXIGRAPH_LAYOUT_RANDOM = 0,
  MXIGRAPH_LAYOUT_CIRCLE,
  MXIGRAPH_LAYOUT_STAR,
  MXIGRAPH_LAYOUT_GRID,
  MXIGRAPH_LAYOUT_GRAPHOPT,
  MXIGRAPH_LAYOUT_BIPARTITE,
  MXIGRAPH_LAYOUT_FRUCHTERMANREINGOLD,
  MXIGRAPH_LAYOUT_KAMADAKAWAI,
  MXIGRAPH_LAYOUT_GEM,
  MXIGRAPH_LAYOUT_DAVIDSONHAREL,
  MXIGRAPH_LAYOUT_MDS,
  MXIGRAPH_LAYOUT_LGL,
  MXIGRAPH_LAYOUT_REINGOLDTILFORD,
  MXIGRAPH_LAYOUT_REINGOLDTILFORDCIRCULAR,
  MXIGRAPH_LAYOUT_N
} mxIgraph_layout_t;

static void set_pos_i(const mxArray *p, igraph_matrix_t *pos)
{
  igraph_matrix_t init;
  igraph_integer_t n_nodes = mxGetM(p);
  mxIgraphGetMatrix(p, &init);
  igraph_matrix_resize(pos, n_nodes, 2);
  for (igraph_integer_t j = 0; j < 2; j++) {
    for (igraph_integer_t i = 0; i < n_nodes; i++) {
      MATRIX(*pos, i, j) = MATRIX(init, i, j);
    }
  }
  igraph_matrix_destroy(&init);
}

static igraph_error_t mxIgraph_random_i(const igraph_t *graph,
                                        const igraph_vector_t *UNUSED(weights),
                                        const mxArray *UNUSED(p_optionals[]),
                                        igraph_matrix_t *pos)
{
  igraph_error_t errcode;

  errcode = igraph_layout_random(graph, pos);

  return errcode;
}

static igraph_error_t mxIgraph_circle_i(const igraph_t *graph,
                                        const igraph_vector_t *UNUSED(weights),
                                        const mxArray *p_optionals[],
                                        igraph_matrix_t *pos)
{
  igraph_vector_int_t order;
  igraph_vs_t vs;
  igraph_error_t errcode;

  mxIgraphArrayToPartition(p_optionals[0], &order);
  igraph_vs_vector(&vs, &order);

  errcode = igraph_layout_circle(graph, pos, vs);

  igraph_vs_destroy(&vs);
  igraph_vector_int_destroy(&order);

  return errcode;
}

static igraph_error_t mxIgraph_star_i(const igraph_t *graph,
                                      const igraph_vector_t *UNUSED(weights),
                                      const mxArray *p_optionals[],
                                      igraph_matrix_t *pos)
{
  igraph_integer_t center = mxGetScalar(p_optionals[0]);
  igraph_vector_int_t order;
  igraph_error_t errcode;

  mxIgraphArrayToPartition(p_optionals[1], &order);

  errcode = igraph_layout_star(graph, pos, center, &order);

  igraph_vector_int_destroy(&order);

  return errcode;
}

static igraph_error_t mxIgraph_grid_i(const igraph_t *graph,
                                      const igraph_vector_t *UNUSED(weights),
                                      const mxArray *p_optionals[],
                                      igraph_matrix_t *pos)
{
  igraph_integer_t width = mxGetScalar(p_optionals[0]);

  return igraph_layout_grid(graph, pos, width);
}

static igraph_error_t mxIgraph_graphopt_i(const igraph_t *graph,
    const igraph_vector_t *UNUSED(weights),
    const mxArray *p_optionals[],
    igraph_matrix_t *pos)
{
  igraph_integer_t n_iterations = mxGetScalar(p_optionals[0]);
  igraph_real_t node_charge = mxGetScalar(p_optionals[1]);
  igraph_real_t node_mass = mxGetScalar(p_optionals[2]);
  igraph_real_t spring_length = mxGetScalar(p_optionals[3]);
  igraph_real_t spring_constant = mxGetScalar(p_optionals[4]);
  igraph_real_t max_sa_movement = mxGetScalar(p_optionals[5]);
  igraph_bool_t use_seed = !mxIgraphIsEmpty(p_optionals[6]);

  if (use_seed) {
    set_pos_i(p_optionals[6], pos);
  }

  return igraph_layout_graphopt(graph, pos, n_iterations, node_charge,
                                node_mass, spring_length, spring_constant,
                                max_sa_movement, use_seed);
}

static igraph_error_t mxIgraph_bipartite_i(const igraph_t *graph,
    const igraph_vector_t *UNUSED(weights),
    const mxArray *p_optionals[],
    igraph_matrix_t *pos)
{
  igraph_vector_bool_t types;
  igraph_real_t hgap = mxGetScalar(p_optionals[1]);
  igraph_real_t vgap = mxGetScalar(p_optionals[2]);
  igraph_real_t max_iterations = mxGetScalar(p_optionals[3]);
  igraph_error_t errcode;

  mxIgraphGetVectorBool(p_optionals[0], &types);

  errcode = igraph_layout_bipartite(graph, &types, pos, hgap, vgap,
                                    max_iterations);

  igraph_vector_bool_destroy(&types);

  return errcode;
}

static igraph_error_t mxIgraph_fruchterman_reingold_i(const igraph_t *graph,
    const igraph_vector_t *weights,
    const mxArray *p_optionals[],
    igraph_matrix_t *pos)
{
  igraph_integer_t n_iter = mxGetScalar(p_optionals[0]);
  igraph_real_t start_temp = mxGetScalar(p_optionals[1]);
  igraph_bool_t use_seed = !mxIgraphIsEmpty(p_optionals[2]);
  const char *grid_method = mxArrayToString(p_optionals[3]);
  igraph_layout_grid_t grid;

  if (strcmp(grid_method, "auto") == 0) {
    grid = IGRAPH_LAYOUT_AUTOGRID;
  } else if (strcmp(grid_method, "grid") == 0) {
    grid = IGRAPH_LAYOUT_GRID;
  } else if (strcmp(grid_method, "nogrid") == 0) {
    grid = IGRAPH_LAYOUT_NOGRID;
  } else {
    mexErrMsgIdAndTxt("Igraph:internal:unKnownMethod",
                      "Fruchtermen-Reingold layout got unknown grid method %s."
                      " Please report to github.",
                      grid_method);
    exit(1);
  }

  if (use_seed) {
    set_pos_i(p_optionals[2], pos);
  }

  return igraph_layout_fruchterman_reingold(graph, pos, use_seed, n_iter,
         start_temp, grid, weights, NULL, NULL, NULL, NULL);
}

static igraph_error_t mxIgraph_kamada_kawai_i(const igraph_t *graph,
    const igraph_vector_t *weights,
    const mxArray *p_optionals[],
    igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxGetScalar(p_optionals[0]);
  igraph_real_t epsilon = mxGetScalar(p_optionals[1]);
  igraph_real_t kkconst = mxGetScalar(p_optionals[2]);
  igraph_bool_t use_seed = !mxIgraphIsEmpty(p_optionals[3]);

  if (use_seed) {
    set_pos_i(p_optionals[3], pos);
  }

  return igraph_layout_kamada_kawai(graph, pos, use_seed, max_iter, epsilon,
                                    kkconst, weights, NULL, NULL, NULL, NULL);
}

static igraph_error_t mxIgraph_gem_i(const igraph_t *graph,
                                     const igraph_vector_t *UNUSED(weights),
                                     const mxArray *p_optionals[],
                                     igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxGetScalar(p_optionals[0]);
  igraph_real_t temp_max = mxGetScalar(p_optionals[1]);
  igraph_real_t temp_min = mxGetScalar(p_optionals[2]);
  igraph_real_t temp_init = mxGetScalar(p_optionals[3]);
  igraph_bool_t use_seed = !mxIgraphIsEmpty(p_optionals[4]);

  if (use_seed) {
    set_pos_i(p_optionals[4], pos);
  }

  return igraph_layout_gem(graph, pos, use_seed, max_iter, temp_max, temp_min,
                           temp_init);
}

static igraph_error_t mxIgraph_davidson_harel_i(const igraph_t *graph,
    const igraph_vector_t *UNUSED(weights),
    const mxArray *p_optionals[],
    igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxGetScalar(p_optionals[0]);
  igraph_integer_t fine_iter = mxGetScalar(p_optionals[1]);
  igraph_real_t cool_factor = mxGetScalar(p_optionals[2]);
  igraph_bool_t use_seed = !mxIgraphIsEmpty(p_optionals[3]);
  igraph_real_t weight_node_dist = mxGetScalar(p_optionals[4]);
  igraph_real_t weight_border = mxGetScalar(p_optionals[5]);
  igraph_real_t weight_edge_lengths = mxGetScalar(p_optionals[6]);
  igraph_real_t weight_edge_crossings = mxGetScalar(p_optionals[7]);
  igraph_real_t weight_node_edge_dist = mxGetScalar(p_optionals[8]);

  if (use_seed) {
    set_pos_i(p_optionals[3], pos);
  }

  return igraph_layout_davidson_harel(graph, pos, use_seed, max_iter,
                                      fine_iter, cool_factor, weight_node_dist,
                                      weight_border, weight_edge_lengths,
                                      weight_edge_crossings,
                                      weight_node_edge_dist);
}

static igraph_error_t mxIgraph_mds_i(const igraph_t *graph,
                                     const igraph_vector_t *UNUSED(weights),
                                     const mxArray *p_optionals[],
                                     igraph_matrix_t *pos)
{
  igraph_matrix_t distance;
  igraph_matrix_t *distance_ptr = NULL;
  igraph_integer_t dim = 2;
  igraph_error_t errcode;

  if (!mxIgraphIsEmpty(p_optionals[0])) {
    mxIgraphGetMatrix(p_optionals[0], &distance);
    distance_ptr = &distance;
  }

  errcode = igraph_layout_mds(graph, pos, distance_ptr, dim);

  igraph_matrix_destroy(&distance);

  return errcode;
}

static igraph_error_t mxIgraph_lgl_i(const igraph_t *graph,
                                     const igraph_vector_t *UNUSED(weights),
                                     const mxArray *p_optionals[],
                                     igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxGetScalar(p_optionals[0]);
  igraph_real_t max_delta = mxGetScalar(p_optionals[1]);
  igraph_real_t area = mxGetScalar(p_optionals[2]);
  igraph_real_t coolexp = mxGetScalar(p_optionals[3]);
  igraph_real_t repulserad = mxGetScalar(p_optionals[4]);
  igraph_real_t cellsize = mxGetScalar(p_optionals[5]);
  igraph_integer_t root = mxGetScalar(p_optionals[6]);

  return igraph_layout_lgl(graph, pos, max_iter, max_delta, area, coolexp,
                           repulserad, cellsize, root);
}

static igraph_error_t mxIgraph_reingold_tilford_i(const igraph_t *graph,
    const igraph_vector_t *UNUSED(weights),
    const mxArray *p_optionals[],
    igraph_matrix_t *pos)
{
  igraph_neimode_t mode;
  igraph_vector_int_t roots;
  igraph_error_t errcode;

  mode = mxIgraphSelectMode(p_optionals[0]);
  mxIgraphArrayToPartition(p_optionals[1], &roots);

  errcode = igraph_layout_reingold_tilford(graph, pos, mode, &roots, NULL);

  igraph_vector_int_destroy(&roots);

  return errcode;
}

static igraph_error_t mxIgraph_reingold_tilford_circular_i(
  const igraph_t *graph,
  const igraph_vector_t *UNUSED(weights),
  const mxArray *p_optionals[],
  igraph_matrix_t *pos)
{
  igraph_neimode_t mode;
  igraph_vector_int_t roots;
  igraph_error_t errcode;

  mode = mxIgraphSelectMode(p_optionals[0]);
  mxIgraphArrayToPartition(p_optionals[1], &roots);

  errcode = igraph_layout_reingold_tilford_circular(graph, pos, mode, &roots,
            NULL);

  igraph_vector_int_destroy(&roots);

  return errcode;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetupHook();

  igraph_integer_t n_required = 2;
  igraph_integer_t n_optionals = nrhs - n_required;
  const mxArray **p_optionals = prhs + n_required;

  VERIFY_N_INPUTS_ATLEAST(n_required);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxIgraph_layout_t method;
  igraph_t graph;
  igraph_vector_t weights;
  igraph_bool_t directed = mxGetScalar(prhs[1]);
  igraph_matrix_t pos;
  igraph_error_t errorcode;
  igraph_error_t (*layout_func)(const igraph_t *, const igraph_vector_t *,
                                const mxArray **, igraph_matrix_t *);

  const char *methods[MXIGRAPH_LAYOUT_N] = {
    [MXIGRAPH_LAYOUT_RANDOM] = "random",
    [MXIGRAPH_LAYOUT_CIRCLE] = "circle",
    [MXIGRAPH_LAYOUT_STAR] = "star",
    [MXIGRAPH_LAYOUT_GRID] = "grid",
    [MXIGRAPH_LAYOUT_GRAPHOPT] = "graphopt",
    [MXIGRAPH_LAYOUT_BIPARTITE] = "bipartite",
    [MXIGRAPH_LAYOUT_FRUCHTERMANREINGOLD] = "fruchtermanreingold",
    [MXIGRAPH_LAYOUT_KAMADAKAWAI] = "kamadakawai",
    [MXIGRAPH_LAYOUT_GEM] = "gem",
    [MXIGRAPH_LAYOUT_DAVIDSONHAREL] = "davidsonharel",
    [MXIGRAPH_LAYOUT_MDS] = "mds",
    [MXIGRAPH_LAYOUT_LGL] = "lgl",
    [MXIGRAPH_LAYOUT_REINGOLDTILFORD] = "reingoldtilford",
    [MXIGRAPH_LAYOUT_REINGOLDTILFORDCIRCULAR] = "reingoldtilfordcircular",
  };

  method = mxIgraphSelectMethod(prhs[0], methods, MXIGRAPH_LAYOUT_N);

  mxIgraphGetGraph(prhs[1], &graph, &weights, directed);
  igraph_matrix_init(&pos, 0, 0);

  switch (method) {
  case MXIGRAPH_LAYOUT_RANDOM:
    VERIFY_N_OPTIONALS(0);
    layout_func = mxIgraph_random_i;
    break;
  case MXIGRAPH_LAYOUT_CIRCLE:
    VERIFY_N_OPTIONALS(1);
    layout_func = mxIgraph_circle_i;
    break;
  case MXIGRAPH_LAYOUT_STAR:
    VERIFY_N_OPTIONALS(2);
    layout_func = mxIgraph_star_i;
    break;
  case MXIGRAPH_LAYOUT_GRID:
    VERIFY_N_OPTIONALS(1);
    layout_func = mxIgraph_grid_i;
    break;
  case MXIGRAPH_LAYOUT_GRAPHOPT:
    VERIFY_N_OPTIONALS(7);
    layout_func = mxIgraph_graphopt_i;
    break;
  case MXIGRAPH_LAYOUT_BIPARTITE:
    VERIFY_N_OPTIONALS(4);
    layout_func = mxIgraph_bipartite_i;
    break;
  case MXIGRAPH_LAYOUT_FRUCHTERMANREINGOLD:
    VERIFY_N_OPTIONALS(4);
    layout_func = mxIgraph_fruchterman_reingold_i;
    break;
  case MXIGRAPH_LAYOUT_KAMADAKAWAI:
    VERIFY_N_OPTIONALS(4);
    layout_func = mxIgraph_kamada_kawai_i;
    break;
  case MXIGRAPH_LAYOUT_GEM:
    VERIFY_N_OPTIONALS(5);
    layout_func = mxIgraph_gem_i;
    break;
  case MXIGRAPH_LAYOUT_DAVIDSONHAREL:
    VERIFY_N_OPTIONALS(9);
    layout_func = mxIgraph_davidson_harel_i;
    break;
  case MXIGRAPH_LAYOUT_MDS:
    VERIFY_N_OPTIONALS(1);
    layout_func = mxIgraph_mds_i;
    break;
  case MXIGRAPH_LAYOUT_LGL:
    VERIFY_N_OPTIONALS(7);
    layout_func = mxIgraph_lgl_i;
    break;
  case MXIGRAPH_LAYOUT_REINGOLDTILFORD:
    VERIFY_N_OPTIONALS(2);
    layout_func = mxIgraph_reingold_tilford_i;
    break;
  case MXIGRAPH_LAYOUT_REINGOLDTILFORDCIRCULAR:
    VERIFY_N_OPTIONALS(2);
    layout_func = mxIgraph_reingold_tilford_circular_i;
    break;
  case MXIGRAPH_LAYOUT_N:
    mxIgraphErrorUnknownMethod(mexFunctionName(), mxArrayToString(prhs[0]));
    exit(1);
  }

  errorcode = layout_func(&graph, &weights, p_optionals, &pos);
  igraph_destroy(&graph);
  igraph_vector_destroy(&weights);

  if (errorcode != IGRAPH_SUCCESS) {
    igraph_matrix_destroy(&pos);
    switch (errorcode) {
    case IGRAPH_ENOMEM:
      mexErrMsgIdAndTxt("Igraph:outOfMemory",
                        "Failed to generate layout due to insufficient memory.");
      break;
    case IGRAPH_EINVAL:
      mexErrMsgIdAndTxt("Igraph:invalidParameter", "Invalid method parameter.");
      break;
    default:
      mexErrMsgIdAndTxt("Igraph:internal:layout",
                        "Failed to generate layout using %s.", methods[method]);
    }
  }

  plhs[0] = mxIgraphCreateMatrix(&pos);
  igraph_matrix_destroy(&pos);
}

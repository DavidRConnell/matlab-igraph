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

#include <mxIgraph.h>
#include <string.h>

#include "utils.h"

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

static igraph_error_t set_pos_i(mxArray const *opts, igraph_bool_t *use_seed,
                                igraph_matrix_t *pos)
{
  mxArray const *p = mxIgraphGetArgument(opts, "initial");
  MXIGRAPH_CHECK_STATUS();
  *use_seed = !mxIsEmpty(p);

  if (!(*use_seed)) {
    return IGRAPH_SUCCESS;
  }

  igraph_matrix_t init;
  igraph_integer_t n_nodes = mxGetM(p);
  IGRAPH_CHECK(mxIgraphMatrixFromArray(p, &init, MXIGRAPH_IDX_KEEP));
  IGRAPH_FINALLY(igraph_matrix_destroy, &init);
  IGRAPH_CHECK(igraph_matrix_resize(pos, n_nodes, 2));
  for (igraph_integer_t j = 0; j < 2; j++) {
    for (igraph_integer_t i = 0; i < n_nodes; i++) {
      MATRIX(*pos, i, j) = MATRIX(init, i, j);
    }
  }
  igraph_matrix_destroy(&init);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_random_i(igraph_t const *graph,
                                        igraph_vector_t const *UNUSED(weights),
                                        mxArray const *UNUSED(opts),
                                        igraph_matrix_t *pos)
{
  IGRAPH_CHECK(igraph_layout_random(graph, pos));
  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_circle_i(igraph_t const *graph,
                                        igraph_vector_t const *UNUSED(weights),
                                        mxArray const *opts,
                                        igraph_matrix_t *pos)
{
  igraph_vector_int_t order;
  igraph_vs_t vs;

  IGRAPH_CHECK(mxIgraphVectorIntFromArray(opts, &order, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &order);
  IGRAPH_CHECK(igraph_vs_vector(&vs, &order));
  IGRAPH_FINALLY(igraph_vs_destroy, &vs);

  IGRAPH_CHECK(igraph_layout_circle(graph, pos, vs));

  igraph_vs_destroy(&vs);
  igraph_vector_int_destroy(&order);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_star_i(igraph_t const *graph,
                                      igraph_vector_t const *UNUSED(weights),
                                      mxArray const *opts,
                                      igraph_matrix_t *pos)
{
  igraph_integer_t center = mxIgraphIntegerFromOptions(opts, "center");
  MXIGRAPH_CHECK_STATUS();
  igraph_vector_int_t order;

  IGRAPH_CHECK(mxIgraphVectorIntFromArray(opts, &order, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &order);

  IGRAPH_CHECK(igraph_layout_star(graph, pos, center, &order));

  igraph_vector_int_destroy(&order);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_grid_i(igraph_t const *graph,
                                      igraph_vector_t const *UNUSED(weights),
                                      mxArray const *opts,
                                      igraph_matrix_t *pos)
{
  igraph_integer_t width = mxIgraphIntegerFromOptions(opts, "width");
  MXIGRAPH_CHECK_STATUS();
  IGRAPH_CHECK(igraph_layout_grid(graph, pos, width));
  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_graphopt_i(igraph_t const *graph,
    igraph_vector_t const *UNUSED(weights), mxArray const *opts,
    igraph_matrix_t *pos)
{
  igraph_integer_t n_iterations =
    mxIgraphIntegerFromOptions(opts, "nIterations");
  igraph_real_t node_charge = mxIgraphRealFromOptions(opts, "charge");
  igraph_real_t node_mass = mxIgraphRealFromOptions(opts, "mass");
  igraph_real_t spring_length = mxIgraphRealFromOptions(opts, "springLength");
  igraph_real_t spring_constant =
    mxIgraphRealFromOptions(opts, "springConstant");
  igraph_real_t max_sa_movement = mxIgraphRealFromOptions(opts, "stepMax");
  MXIGRAPH_CHECK_STATUS();
  igraph_bool_t use_seed;

  IGRAPH_CHECK(set_pos_i(opts, &use_seed, pos));
  IGRAPH_CHECK(igraph_layout_graphopt(graph, pos, n_iterations, node_charge,
                                      node_mass, spring_length, spring_constant,
                                      max_sa_movement, use_seed));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_bipartite_i(igraph_t const *graph,
    igraph_vector_t const *UNUSED(weights),
    mxArray const *opts, igraph_matrix_t *pos)
{
  igraph_vector_bool_t types;
  igraph_real_t hgap = mxIgraphRealFromOptions(opts, "hgap");
  igraph_real_t vgap = mxIgraphRealFromOptions(opts, "vgap");
  igraph_real_t max_iterations =
    mxIgraphIntegerFromOptions(opts, "maxIterations");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(mxIgraphVectorBoolFromOptions(opts, "types", &types, false));
  IGRAPH_FINALLY(igraph_vector_bool_destroy, &types);

  IGRAPH_CHECK(
    igraph_layout_bipartite(graph, &types, pos, hgap, vgap, max_iterations));

  igraph_vector_bool_destroy(&types);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_fruchterman_reingold_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *opts, igraph_matrix_t *pos)
{
  igraph_integer_t n_iter = mxIgraphIntegerFromOptions(opts, "nIterations");
  igraph_real_t start_temp = mxIgraphRealFromOptions(opts, "startTemp");
  char const *grid_method = mxIgraphStringFromOptions(opts, "grid");
  MXIGRAPH_CHECK_STATUS();

  igraph_bool_t use_seed;
  igraph_layout_grid_t grid;

  IGRAPH_CHECK(set_pos_i(opts, &use_seed, pos));

  if (strcmp(grid_method, "auto") == 0) {
    grid = IGRAPH_LAYOUT_AUTOGRID;
  } else if (strcmp(grid_method, "grid") == 0) {
    grid = IGRAPH_LAYOUT_GRID;
  } else if (strcmp(grid_method, "nogrid") == 0) {
    grid = IGRAPH_LAYOUT_NOGRID;
  } else {
    IGRAPH_FATALF(
      "Fruchtermen-Reingold layout got unexpected grid method \"%s\"",
      grid_method);
  }

  IGRAPH_CHECK(igraph_layout_fruchterman_reingold(graph, pos, use_seed, n_iter,
               start_temp, grid, weights,
               NULL, NULL, NULL, NULL));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_kamada_kawai_i(igraph_t const *graph,
    igraph_vector_t const *weights,
    mxArray const *opts,
    igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxIgraphIntegerFromOptions(opts, "maxIterations");
  igraph_real_t epsilon = mxIgraphRealFromOptions(opts, "epsilon");
  igraph_real_t kkconst = mxIgraphRealFromOptions(opts, "constant");
  MXIGRAPH_CHECK_STATUS();
  igraph_bool_t use_seed;

  IGRAPH_CHECK(set_pos_i(opts, &use_seed, pos));

  IGRAPH_CHECK(igraph_layout_kamada_kawai(graph, pos, use_seed, max_iter,
                                          epsilon, kkconst, weights, NULL, NULL,
                                          NULL, NULL));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_gem_i(igraph_t const *graph,
                                     igraph_vector_t const *UNUSED(weights),
                                     mxArray const *opts,
                                     igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxIgraphIntegerFromOptions(opts, "maxIterations");
  igraph_real_t temp_max = mxIgraphRealFromOptions(opts, "tempMax");
  igraph_real_t temp_min = mxIgraphRealFromOptions(opts, "tempMin");
  igraph_real_t temp_init = mxIgraphRealFromOptions(opts, "tempInit");
  MXIGRAPH_CHECK_STATUS();
  igraph_bool_t use_seed;

  IGRAPH_CHECK(set_pos_i(opts, &use_seed, pos));
  IGRAPH_CHECK(igraph_layout_gem(graph, pos, use_seed, max_iter, temp_max,
                                 temp_min, temp_init));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_davidson_harel_i(igraph_t const *graph,
    igraph_vector_t const *UNUSED(weights),
    mxArray const *opts, igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxIgraphIntegerFromOptions(opts, "maxIterations");
  igraph_integer_t fine_iter =
    mxIgraphIntegerFromOptions(opts, "fineIterations");
  igraph_real_t cool_factor = mxIgraphRealFromOptions(opts, "coolingFactor");
  igraph_real_t weight_node_dist =
    mxIgraphRealFromOptions(opts, "weightNodeDist");
  igraph_real_t weight_border = mxIgraphRealFromOptions(opts, "weightBorder");
  igraph_real_t weight_edge_lengths =
    mxIgraphRealFromOptions(opts, "weightEdgeLen");
  igraph_real_t weight_edge_crossings =
    mxIgraphRealFromOptions(opts, "weightEdgeCross");
  igraph_real_t weight_node_edge_dist =
    mxIgraphRealFromOptions(opts, "weightNodeEdgeDist");
  MXIGRAPH_CHECK_STATUS();

  igraph_bool_t use_seed;

  IGRAPH_CHECK(set_pos_i(opts, &use_seed, pos));

  IGRAPH_CHECK(igraph_layout_davidson_harel(
                 graph, pos, use_seed, max_iter, fine_iter, cool_factor, weight_node_dist,
                 weight_border, weight_edge_lengths, weight_edge_crossings,
                 weight_node_edge_dist));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_mds_i(igraph_t const *graph,
                                     igraph_vector_t const *UNUSED(weights),
                                     mxArray const *opts,
                                     igraph_matrix_t *pos)
{
  igraph_matrix_t distance;
  igraph_matrix_t *distance_ptr = NULL;
  igraph_integer_t dim = 2;

  if (!mxIgraphIsEmpty(opts)) {
    IGRAPH_CHECK(mxIgraphMatrixFromArray(opts, &distance, MXIGRAPH_IDX_KEEP));
    IGRAPH_FINALLY(igraph_matrix_destroy, &distance);
    distance_ptr = &distance;
  }

  IGRAPH_CHECK(igraph_layout_mds(graph, pos, distance_ptr, dim));
  igraph_matrix_destroy(&distance);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_lgl_i(igraph_t const *graph,
                                     igraph_vector_t const *UNUSED(weights),
                                     mxArray const *opts,
                                     igraph_matrix_t *pos)
{
  igraph_integer_t max_iter = mxIgraphIntegerFromOptions(opts, "maxIterations");
  igraph_real_t max_delta = mxIgraphRealFromOptions(opts, "stepMax");
  igraph_real_t area = mxIgraphRealFromOptions(opts, "area");
  igraph_real_t coolexp = mxIgraphRealFromOptions(opts, "coolExp");
  igraph_real_t repulserad = mxIgraphRealFromOptions(opts, "repulseRadius");
  igraph_real_t cellsize = mxIgraphRealFromOptions(opts, "cellSize");
  igraph_integer_t root = mxIgraphIntegerFromOptions(opts, "root");
  MXIGRAPH_CHECK_STATUS();

  IGRAPH_CHECK(igraph_layout_lgl(graph, pos, max_iter, max_delta, area, coolexp,
                                 repulserad, cellsize, root));

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_reingold_tilford_i(igraph_t const *graph,
    igraph_vector_t const *UNUSED(weights),
    mxArray const *opts, igraph_matrix_t *pos)
{
  igraph_neimode_t mode = mxIgraphModeFromOptions(opts);
  MXIGRAPH_CHECK_STATUS();
  igraph_vector_int_t roots;

  IGRAPH_CHECK(mxIgraphVectorIntFromArray(opts, &roots, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &roots);

  IGRAPH_CHECK(igraph_layout_reingold_tilford(graph, pos, mode, &roots, NULL));
  igraph_vector_int_destroy(&roots);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

static igraph_error_t mxIgraph_reingold_tilford_circular_i(
  igraph_t const *graph, igraph_vector_t const *UNUSED(weights),
  mxArray const *opts, igraph_matrix_t *pos)
{
  igraph_neimode_t mode = mxIgraphModeFromOptions(opts);
  MXIGRAPH_CHECK_STATUS();
  igraph_vector_int_t roots;

  IGRAPH_CHECK(mxIgraphVectorIntFromArray(opts, &roots, MXIGRAPH_IDX_SHIFT));
  IGRAPH_FINALLY(igraph_vector_int_destroy, &roots);

  IGRAPH_CHECK(
    igraph_layout_reingold_tilford_circular(graph, pos, mode, &roots, NULL));
  igraph_vector_int_destroy(&roots);
  IGRAPH_FINALLY_CLEAN(1);

  return IGRAPH_SUCCESS;
}

igraph_error_t mexIgraphLayout(int nlhs, mxArray *plhs[], int nrhs,
                               mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_N_OUTPUTS_EQUAL(1);

  igraph_t graph;
  igraph_vector_t weights;
  mxArray const *graph_options = prhs[2];
  mxIgraph_layout_t method;
  mxArray const *method_options = prhs[3];
  igraph_matrix_t pos;
  typedef igraph_error_t (*layout_method_t)(igraph_t const *,
      igraph_vector_t const *,
      mxArray const *, igraph_matrix_t *);
  layout_method_t layout_method;

  char const *methods[MXIGRAPH_LAYOUT_N] = {
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

  method = mxIgraphSelectMethod(prhs[1], methods, MXIGRAPH_LAYOUT_N);
  MXIGRAPH_CHECK_METHOD(method, prhs[1]);

  layout_method_t method_table[MXIGRAPH_LAYOUT_N] = {
    [MXIGRAPH_LAYOUT_RANDOM] = mxIgraph_random_i,
    [MXIGRAPH_LAYOUT_CIRCLE] = mxIgraph_circle_i,
    [MXIGRAPH_LAYOUT_STAR] = mxIgraph_star_i,
    [MXIGRAPH_LAYOUT_GRID] = mxIgraph_grid_i,
    [MXIGRAPH_LAYOUT_GRAPHOPT] = mxIgraph_graphopt_i,
    [MXIGRAPH_LAYOUT_BIPARTITE] = mxIgraph_bipartite_i,
    [MXIGRAPH_LAYOUT_FRUCHTERMANREINGOLD] = mxIgraph_fruchterman_reingold_i,
    [MXIGRAPH_LAYOUT_KAMADAKAWAI] = mxIgraph_kamada_kawai_i,
    [MXIGRAPH_LAYOUT_GEM] = mxIgraph_gem_i,
    [MXIGRAPH_LAYOUT_DAVIDSONHAREL] = mxIgraph_davidson_harel_i,
    [MXIGRAPH_LAYOUT_MDS] = mxIgraph_mds_i,
    [MXIGRAPH_LAYOUT_LGL] = mxIgraph_lgl_i,
    [MXIGRAPH_LAYOUT_REINGOLDTILFORD] = mxIgraph_reingold_tilford_i,
    [MXIGRAPH_LAYOUT_REINGOLDTILFORDCIRCULAR] =
    mxIgraph_reingold_tilford_circular_i
  };

  layout_method = method_table[method];
  if (!layout_method) {
    IGRAPH_ERRORF("Layout method \"%s\" not implemented.", IGRAPH_UNIMPLEMENTED,
                  mxArrayToString(prhs[1]));
  }

  IGRAPH_CHECK(mxIgraphFromArray(prhs[0], &graph, &weights, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph);
  IGRAPH_FINALLY(igraph_vector_destroy, &weights);
  IGRAPH_CHECK(igraph_matrix_init(&pos, 0, 0));
  IGRAPH_FINALLY(igraph_matrix_destroy, &pos);

  IGRAPH_CHECK(
    layout_method(&graph, MXIGRAPH_WEIGHTS(&weights), method_options, &pos));
  igraph_destroy(&graph);
  igraph_vector_destroy(&weights);

  plhs[0] = mxIgraphMatrixToArray(&pos, MXIGRAPH_IDX_KEEP);
  igraph_matrix_destroy(&pos);
  IGRAPH_FINALLY_CLEAN(3);

  return IGRAPH_SUCCESS;
}

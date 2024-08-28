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
  MXIGRAPH_GENERATOR_STAR = 0,
  MXIGRAPH_GENERATOR_WHEEL,
  MXIGRAPH_GENERATOR_RING,
  MXIGRAPH_GENERATOR_SQUARE_LATTICE,
  MXIGRAPH_GENERATOR_TRIANGULAR_LATTICE,
  MXIGRAPH_GENERATOR_HEXAGONAL_LATTICE,
  MXIGRAPH_GENERATOR_KARY_TREE,
  MXIGRAPH_GENERATOR_SYMMETRIC_TREE,
  MXIGRAPH_GENERATOR_REGULAR_TREE,
  MXIGRAPH_GENERATOR_FULL,
  MXIGRAPH_GENERATOR_CITATION,
  MXIGRAPH_GENERATOR_MULTIPARTITE,
  MXIGRAPH_GENERATOR_TURAN,
  MXIGRAPH_GENERATOR_REALIZE_DEGREE_SEQUENCE,
  MXIGRAPH_GENERATOR_PRUFER,
  MXIGRAPH_GENERATOR_ATLAS,
  MXIGRAPH_GENERATOR_DE_BRUIJN,
  MXIGRAPH_GENERATOR_KAUTZ,
  MXIGRAPH_GENERATOR_CIRCULANT,
  MXIGRAPH_GENERATOR_PETERSEN,
  MXIGRAPH_GENERATOR_CHORDAL_RING,
  MXIGRAPH_GENERATOR_N
} mxIgraph_generator_t;

static igraph_error_t mxIgraph_star_i(mxArray const *opts, igraph_t *graph)
{
  const char *modes[] = {[IGRAPH_STAR_OUT] = "out",
                         [IGRAPH_STAR_IN] = "in",
                         [IGRAPH_STAR_MUTUAL] = "mutual",
                         [IGRAPH_STAR_UNDIRECTED] = "undirected"
                        };
  const igraph_integer_t n_modes = 4;
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_star_mode_t mode =
    mxIgraphSelectMethod(mxIgraphGetArgument(opts, "mode"), modes, n_modes);
  igraph_integer_t center = mxIgraphIntegerFromOptions(opts, "center");

  if (mode == -1) {
    IGRAPH_ERROR("Invalid mode.", IGRAPH_EINVMODE);
  }
  MXIGRAPH_CHECK_STATUS();

  return igraph_star(graph, n_nodes, mode, center);
}

static igraph_error_t mxIgraph_wheel_i(mxArray const *opts, igraph_t *graph)
{
  const char *modes[] = {[IGRAPH_WHEEL_OUT] = "out",
                         [IGRAPH_WHEEL_IN] = "in",
                         [IGRAPH_WHEEL_MUTUAL] = "mutual",
                         [IGRAPH_WHEEL_UNDIRECTED] = "undirected"
                        };
  const igraph_integer_t n_modes = 4;
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_wheel_mode_t mode =
    mxIgraphSelectMethod(mxIgraphGetArgument(opts, "mode"), modes, n_modes);
  igraph_integer_t center = mxIgraphIntegerFromOptions(opts, "center");

  if (mode == -1) {
    IGRAPH_ERROR("Invalid mode.", IGRAPH_EINVMODE);
  }
  MXIGRAPH_CHECK_STATUS();

  return igraph_wheel(graph, n_nodes, mode, center);
}

static igraph_error_t mxIgraph_ring_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_bool_t is_directed = mxIgraphBoolFromOptions(opts, "isdirected");
  igraph_bool_t is_mutual = mxIgraphBoolFromOptions(opts, "ismutual");
  igraph_bool_t is_circular = mxIgraphBoolFromOptions(opts, "iscircular");
  MXIGRAPH_CHECK_STATUS();

  return igraph_ring(graph, n_nodes, is_directed, is_mutual, is_circular);
}

static igraph_error_t mxIgraph_kary_tree_i(mxArray const *opts,
    igraph_t *graph)
{
  const char *types[] = {[IGRAPH_TREE_OUT] = "out",
                         [IGRAPH_TREE_IN] = "in",
                         [IGRAPH_TREE_UNDIRECTED] = "undirected"
                        };
  const igraph_integer_t n_types = 3;
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t children = mxIgraphIntegerFromOptions(opts, "children");
  igraph_tree_mode_t type =
    mxIgraphSelectMethod(mxIgraphGetArgument(opts, "mode"), types, n_types);

  if (type == -1) {
    IGRAPH_ERROR("Unknown tree mode.", IGRAPH_EINVMODE);
  }
  MXIGRAPH_CHECK_STATUS();

  return igraph_kary_tree(graph, n_nodes, children, type);
}

static igraph_error_t mxIgraph_regular_tree_i(mxArray const *opts,
    igraph_t *graph)
{
  const char *types[] = {[IGRAPH_TREE_OUT] = "out",
                         [IGRAPH_TREE_IN] = "in",
                         [IGRAPH_TREE_UNDIRECTED] = "undirected"
                        };
  const igraph_integer_t n_types = 3;
  igraph_integer_t height = mxIgraphIntegerFromOptions(opts, "height");
  igraph_integer_t degree = mxIgraphIntegerFromOptions(opts, "degree");
  igraph_tree_mode_t type =
    mxIgraphSelectMethod(mxIgraphGetArgument(opts, "mode"), types, n_types);

  if (type == -1) {
    IGRAPH_ERROR("Unknown tree mode.", IGRAPH_EINVMODE);
  }
  MXIGRAPH_CHECK_STATUS();

  return igraph_regular_tree(graph, height, degree, type);
}

static igraph_error_t mxIgraph_full_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_bool_t is_directed = mxIgraphIntegerFromOptions(opts, "isdirected");
  igraph_bool_t loops = mxIgraphIntegerFromOptions(opts, "loops");
  MXIGRAPH_CHECK_STATUS();

  return igraph_full(graph, n_nodes, is_directed, loops);
}

static igraph_error_t mxIgraph_citation_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_bool_t is_directed = mxIgraphIntegerFromOptions(opts, "isdirected");
  MXIGRAPH_CHECK_STATUS();

  return igraph_full_citation(graph, n_nodes, is_directed);
}

static igraph_error_t mxIgraph_prufer_i(mxArray const *opts, igraph_t *graph)
{
  igraph_vector_int_t prufer;
  igraph_error_t errcode;

  mxIgraphVectorIntFromOptions(opts, "prufer", &prufer, true);

  errcode = igraph_from_prufer(graph, &prufer);
  igraph_vector_int_destroy(&prufer);

  return errcode;
}

static igraph_error_t mxIgraph_atlas_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t atlas_id = mxIgraphIntegerFromOptions(opts, "atlasId");
  MXIGRAPH_CHECK_STATUS();

  return igraph_atlas(graph, atlas_id);
}

static igraph_error_t mxIgraph_de_bruijn_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_letters = mxIgraphIntegerFromOptions(opts, "nLetters");
  igraph_integer_t str_len = mxIgraphIntegerFromOptions(opts, "stringLength");
  MXIGRAPH_CHECK_STATUS();

  return igraph_de_bruijn(graph, n_letters, str_len);
}

static igraph_error_t mxIgraph_kautz_i(mxArray const *opts, igraph_t *graph)
{
  igraph_integer_t n_letters = mxIgraphIntegerFromOptions(opts, "nLetters");
  igraph_integer_t str_len = mxIgraphIntegerFromOptions(opts, "stringLength");
  MXIGRAPH_CHECK_STATUS();

  return igraph_kautz(graph, n_letters, str_len);
}

static igraph_error_t mxIgraph_circulant_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_vector_int_t shifts;
  igraph_bool_t is_directed = mxIgraphBoolFromOptions(opts, "isdirected");
  MXIGRAPH_CHECK_STATUS();
  igraph_error_t errcode;

  mxIgraphVectorIntFromOptions(opts, "shifts", &shifts, false);

  errcode = igraph_circulant(graph, n_nodes, &shifts, is_directed);
  igraph_vector_int_destroy(&shifts);

  return errcode;
}

static igraph_error_t mxIgraph_petersen_i(mxArray const *opts,
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxIgraphIntegerFromOptions(opts, "nNodes");
  igraph_integer_t shift = mxIgraphIntegerFromOptions(opts, "shift");
  MXIGRAPH_CHECK_STATUS();

  return igraph_generalized_petersen(graph, n_nodes, shift);
}

igraph_error_t mexIgraphGenerate(int nlhs, mxArray *plhs[], int nrhs,
                                 mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxIgraph_generator_t method;
  mxArray const *graph_options = prhs[1];
  mxArray const *method_options = prhs[2];
  igraph_t graph;
  igraph_error_t errorcode;
  typedef igraph_error_t (*generator_method_t)(mxArray const *prhs, igraph_t *);
  generator_method_t generator_method;

  const char *generators[MXIGRAPH_GENERATOR_N] = {
    [MXIGRAPH_GENERATOR_STAR] = "star",
    [MXIGRAPH_GENERATOR_WHEEL] = "wheel",
    [MXIGRAPH_GENERATOR_RING] = "ring",
    [MXIGRAPH_GENERATOR_SQUARE_LATTICE] = "squarelattice",
    [MXIGRAPH_GENERATOR_TRIANGULAR_LATTICE] = "triangularlattice",
    [MXIGRAPH_GENERATOR_HEXAGONAL_LATTICE] = "hexagonallattice",
    [MXIGRAPH_GENERATOR_KARY_TREE] = "karytree",
    [MXIGRAPH_GENERATOR_SYMMETRIC_TREE] = "symmetrictree",
    [MXIGRAPH_GENERATOR_REGULAR_TREE] = "regulartree",
    [MXIGRAPH_GENERATOR_FULL] = "full",
    [MXIGRAPH_GENERATOR_CITATION] = "citation",
    [MXIGRAPH_GENERATOR_MULTIPARTITE] = "multipartite",
    [MXIGRAPH_GENERATOR_TURAN] = "turan",
    [MXIGRAPH_GENERATOR_REALIZE_DEGREE_SEQUENCE] = "realizedegreesequence",
    [MXIGRAPH_GENERATOR_PRUFER] = "prufer",
    [MXIGRAPH_GENERATOR_ATLAS] = "atlas",
    [MXIGRAPH_GENERATOR_DE_BRUIJN] = "debruijn",
    [MXIGRAPH_GENERATOR_KAUTZ] = "kautz",
    [MXIGRAPH_GENERATOR_CIRCULANT] = "circulant",
    [MXIGRAPH_GENERATOR_PETERSEN] = "petersen",
    [MXIGRAPH_GENERATOR_CHORDAL_RING] = "chordalring"
  };

  MXIGRAPH_CHECK_METHOD((method = mxIgraphSelectMethod(prhs[0], generators,
                                  MXIGRAPH_GENERATOR_N)),
                        prhs[0]);

  generator_method_t method_table[MXIGRAPH_GENERATOR_N] = {
    [MXIGRAPH_GENERATOR_STAR] = mxIgraph_star_i,
    [MXIGRAPH_GENERATOR_WHEEL] = mxIgraph_wheel_i,
    [MXIGRAPH_GENERATOR_RING] = mxIgraph_ring_i,
    [MXIGRAPH_GENERATOR_KARY_TREE] = mxIgraph_kary_tree_i,
    [MXIGRAPH_GENERATOR_REGULAR_TREE] = mxIgraph_regular_tree_i,
    [MXIGRAPH_GENERATOR_FULL] = mxIgraph_full_i,
    [MXIGRAPH_GENERATOR_CITATION] = mxIgraph_citation_i,
    [MXIGRAPH_GENERATOR_PRUFER] = mxIgraph_prufer_i,
    [MXIGRAPH_GENERATOR_ATLAS] = mxIgraph_atlas_i,
    [MXIGRAPH_GENERATOR_DE_BRUIJN] = mxIgraph_de_bruijn_i,
    [MXIGRAPH_GENERATOR_KAUTZ] = mxIgraph_kautz_i,
    [MXIGRAPH_GENERATOR_CIRCULANT] = mxIgraph_circulant_i,
    [MXIGRAPH_GENERATOR_PETERSEN] = mxIgraph_petersen_i
  };

  generator_method = method_table[method];
  if (!generator_method) {
    IGRAPH_ERRORF("Generator method %s not implemented.", IGRAPH_UNIMPLEMENTED,
                  mxArrayToString(prhs[0]));
  }

  generator_method(method_options, &graph);

  plhs[0] = mxIgraphToArray(&graph, NULL, graph_options);
  igraph_destroy(&graph);

  return errorcode;
}

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

igraph_error_t mexIgraphCorrelateWith(int nlhs, mxArray *plhs[], int nrhs,
                                      const mxArray *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *graph_options = prhs[1];
  mxArray const *method_options = prhs[2];

  igraph_real_t correlation = mxIgraphGetReal(method_options, "correlation");
  igraph_real_t probability = mxIgraphGetReal(method_options, "density");
  igraph_error_t errorcode;

  igraph_t template;
  igraph_t result;

  mxIgraphFromArray(prhs[0], &template, NULL, graph_options);

  errorcode = igraph_correlated_game(&template, &result, correlation,
                                     probability, NULL);

  if (errorcode != IGRAPH_SUCCESS) {
    return errorcode;
  }

  igraph_destroy(&template);

  plhs[0] = mxIgraphToArray(&result, NULL, graph_options);
  igraph_destroy(&result);

  return errorcode;
}

igraph_error_t mexIgraphGeneratePair(int nlhs, mxArray *plhs[], int nrhs,
                                     const mxArray *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(2);

  mxArray const *graph_options = prhs[1];
  mxArray const *method_options = prhs[2];

  igraph_t graph1;
  igraph_t graph2;
  igraph_integer_t n_nodes = mxGetScalar(prhs[0]);
  igraph_real_t correlation = mxIgraphGetReal(method_options, "correlation");
  igraph_real_t probability = mxIgraphGetInteger(method_options, "density");
  igraph_bool_t directed = mxIgraphGetBool(graph_options, "directed");
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  errorcode = igraph_correlated_pair_game(
                &graph1, &graph2, n_nodes, correlation, probability, directed, NULL);

  if (errorcode != IGRAPH_SUCCESS) {
    return errorcode;
  }

  plhs[0] = mxIgraphToArray(&graph1, NULL, graph_options);
  plhs[1] = mxIgraphToArray(&graph2, NULL, graph_options);

  igraph_destroy(&graph1);
  igraph_destroy(&graph2);
  return errorcode;
}

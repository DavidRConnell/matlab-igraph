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
#include <stdio.h>

igraph_error_t mexIgraphWrite(
  int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_NO_OUTPUTS;

  mxArray const* graph_options = prhs[3];
  igraph_t graph;
  igraph_vector_t weights;
  char* filename = mxArrayToString(prhs[0]);
  mxIgraphFileFormat_t format = mxIgraphSelectFileFormat(prhs[2]);
  igraph_bool_t is_weighted =
    mxIgraphBoolFromOptions(graph_options, "isweighted");
  MXIGRAPH_CHECK_STATUS();

  FILE* fptr;
  if (!(fptr = fopen(filename, "w"))) {
    IGRAPH_ERRORF(
      "Could not open file \"%s\" for writing.", IGRAPH_EFILE, filename);
  }
  IGRAPH_FINALLY(fclose, fptr);

  if (is_weighted) {
    igraph_set_attribute_table(&igraph_cattribute_table);
  }

  IGRAPH_CHECK(mxIgraphFromArray(
    prhs[1], &graph, is_weighted ? &weights : NULL, graph_options));
  IGRAPH_FINALLY(igraph_destroy, &graph);

  if (is_weighted) {
    SETEANV(&graph, "weight", &weights);
    igraph_vector_destroy(&weights);
  }

  switch (format) {
    case MXIGRAPH_FORMAT_EDGELIST:
      IGRAPH_CHECK(igraph_write_graph_edgelist(&graph, fptr));
      break;
    case MXIGRAPH_FORMAT_NCOL:
      if (is_weighted) {
        IGRAPH_CHECK(igraph_write_graph_ncol(&graph, fptr, NULL, "weight"));
      } else {
        IGRAPH_CHECK(igraph_write_graph_ncol(&graph, fptr, NULL, NULL));
      }
      break;
    case MXIGRAPH_FORMAT_LGL:
      if (is_weighted) {
        IGRAPH_CHECK(
          igraph_write_graph_lgl(&graph, fptr, NULL, "weight", true));
      } else {
        IGRAPH_CHECK(igraph_write_graph_lgl(&graph, fptr, NULL, NULL, true));
      }
      break;
    case MXIGRAPH_FORMAT_DIMACS:
      IGRAPH_ERROR(
        "The DIMACS format has not been implemented in matlab-igraph.",
        IGRAPH_UNIMPLEMENTED);
      break;
    case MXIGRAPH_FORMAT_GRAPHML:
      IGRAPH_CHECK(igraph_write_graph_graphml(&graph, fptr, true));
      break;
    case MXIGRAPH_FORMAT_GML:
      IGRAPH_CHECK(igraph_write_graph_gml(
        &graph, fptr, IGRAPH_WRITE_GML_DEFAULT_SW, 0, NULL));
      break;
    case MXIGRAPH_FORMAT_DOT:
      IGRAPH_CHECK(igraph_write_graph_dot(&graph, fptr));
      break;
    case MXIGRAPH_FORMAT_LEDA:
      if (is_weighted) {
        IGRAPH_CHECK(igraph_write_graph_leda(&graph, fptr, NULL, "weight"));
      } else {
        IGRAPH_CHECK(igraph_write_graph_leda(&graph, fptr, NULL, NULL));
      }
      break;
    default:
      IGRAPH_FATAL("Received an unknown or not implemented file format.");
  }

  fclose(fptr);
  igraph_destroy(&graph);
  IGRAPH_FINALLY_CLEAN(2);

  return IGRAPH_SUCCESS;
}

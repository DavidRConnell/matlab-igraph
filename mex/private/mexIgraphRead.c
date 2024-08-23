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

#include <stdio.h>

#include <mxIgraph.h>
#include "utils.h"

#define mexIgraphError(eid, msg)                                               \
  fclose(fptr);                                                                \
  igraph_destroy(&graph);                                                      \
  igraph_vector_destroy(&weights);                                             \
  mexErrMsgIdAndTxt((eid), (msg));                                             \
  exit(1)

igraph_error_t mexIgraphRead(int nlhs, mxArray *plhs[], int nrhs,
                             mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const *method_options = prhs[1];
  mxArray const *graph_options = prhs[2];
  igraph_t graph;
  igraph_vector_t weights;
  char *filename = mxArrayToString(prhs[0]);
  mxIgraphFileFormat_t format =
    mxIgraphSelectFileFormat(mxIgraphGetArgument(method_options, "format"));
  igraph_integer_t index = mxIgraphGetInteger(method_options, "index");

  igraph_bool_t is_weighted = mxIgraphGetBool(graph_options, "isweighted");
  igraph_bool_t is_directed = mxIgraphGetBool(graph_options, "isdirected");
  FILE *fptr;
  igraph_error_t errorcode = IGRAPH_SUCCESS;
  igraph_error_handler_t *oldhandler;

  if (!(fptr = fopen(filename, "r"))) {
    mexErrMsgIdAndTxt("igraph:IOError", "Could not open file %s for reading.",
                      filename);
  }

  igraph_set_attribute_table(&igraph_cattribute_table);

  switch (format) {
  case MXIGRAPH_FORMAT_EDGELIST:
    errorcode = igraph_read_graph_edgelist(&graph, fptr, 0, is_directed);
    break;
  case MXIGRAPH_FORMAT_NCOL:
    errorcode = igraph_read_graph_ncol(
                  &graph, fptr, NULL, NULL, IGRAPH_ADD_WEIGHTS_IF_PRESENT, is_directed);
    break;
  case MXIGRAPH_FORMAT_LGL:
    errorcode = igraph_read_graph_lgl(
                  &graph, fptr, NULL, IGRAPH_ADD_WEIGHTS_IF_PRESENT, is_directed);
    break;
  case MXIGRAPH_FORMAT_DIMACS:
    mexIgraphError(
      "igraph:notImplemented",
      "The DIMACS format has not been implemented in matlab-igraph");
    break;
  case MXIGRAPH_FORMAT_GRAPHDB:
    errorcode = igraph_read_graph_graphdb(&graph, fptr, is_directed);
    break;
  case MXIGRAPH_FORMAT_GRAPHML:
    oldhandler = igraph_set_error_handler(igraph_error_handler_ignore);
    errorcode = igraph_read_graph_graphml(&graph, fptr, index);
    if (errorcode == IGRAPH_UNIMPLEMENTED) {
      mexIgraphError("igraph:notSupported",
                     "igraph was compiled without GraphML support.");
    }
    igraph_set_error_handler(oldhandler);
    break;
  case MXIGRAPH_FORMAT_GML:
    errorcode = igraph_read_graph_gml(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_PAJEK:
    errorcode = igraph_read_graph_pajek(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_DL:
    errorcode = igraph_read_graph_dl(&graph, fptr, is_directed);
    break;
  case MXIGRAPH_FORMAT_DOT:
    mexIgraphError("igraph:notImplemented",
                   "Upstream igraph does not support reading DOT files.");
    break;
  case MXIGRAPH_FORMAT_LEDA:
    mexIgraphError("igraph:notImplemented",
                   "Upstream igraph does not support reading LEDA files.");
    break;
  default:
    fclose(fptr);
    mexErrMsgIdAndTxt("Igraph:unknownFileType",
                      "Unrecognized file format %s. See `help igraph.load` for "
                      "available file formats.",
                      format);
    exit(1);
  }

  igraph_vector_init(&weights, igraph_ecount(&graph));
  if (is_weighted &&
      igraph_cattribute_has_attr(&graph, IGRAPH_ATTRIBUTE_EDGE, "weight")) {
    EANV(&graph, "weight", &weights);
  } else {
    for (igraph_integer_t i = 0; i < igraph_ecount(&graph); i++) {
      VECTOR(weights)[i] = 1;
    }
  }

  plhs[0] = mxIgraphCreateGraph(&graph, &weights, graph_options);

  fclose(fptr);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);

  return errorcode;
}

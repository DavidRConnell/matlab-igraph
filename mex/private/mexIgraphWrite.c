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

#define mexIgraphError(id, msg)                                                \
  fclose(fptr);                                                                \
  igraph_destroy(&graph);                                                      \
  mexErrMsgIdAndTxt((id), (msg));                                              \
  exit(1)

igraph_error_t mexIgraphWrite(int nlhs, mxArray *plhs[], int nrhs,
                              mxArray const *prhs[])
{
  VERIFY_N_INPUTS_EQUAL(4);
  VERIFY_NO_OUTPUTS;

  mxArray const *graph_options = prhs[3];
  igraph_t graph;
  igraph_vector_t weights;
  char *filename = mxArrayToString(prhs[0]);
  mxIgraphFileFormat_t format = mxIgraphSelectFileFormat(prhs[2]);
  igraph_bool_t is_weighted = mxIgraphGetBool(graph_options, "isweighted");
  FILE *fptr;
  igraph_error_t errorcode = IGRAPH_SUCCESS;
  igraph_error_handler_t *oldhandler;

  if (!(fptr = fopen(filename, "w"))) {
    mexErrMsgIdAndTxt("Igraph:IOError", "Could not open file %s for writing.",
                      filename);
  }

  if (is_weighted) {
    igraph_set_attribute_table(&igraph_cattribute_table);
  }

  mxIgraphGetGraph(prhs[1], &graph, is_weighted ? &weights : NULL,
                   graph_options);

  if (is_weighted) {
    SETEANV(&graph, "weight", &weights);
    igraph_vector_destroy(&weights);
  }

  switch (format) {
  case MXIGRAPH_FORMAT_EDGELIST:
    errorcode = igraph_write_graph_edgelist(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_NCOL:
    if (is_weighted) {
      errorcode = igraph_write_graph_ncol(&graph, fptr, NULL, "weight");
    } else {
      errorcode = igraph_write_graph_ncol(&graph, fptr, NULL, NULL);
    }
    break;
  case MXIGRAPH_FORMAT_LGL:
    if (is_weighted) {
      errorcode = igraph_write_graph_lgl(&graph, fptr, NULL, "weight", true);
    } else {
      errorcode = igraph_write_graph_lgl(&graph, fptr, NULL, NULL, true);
    }
    break;
  case MXIGRAPH_FORMAT_DIMACS:
    mexIgraphError(
      "igraph:notImplemented",
      "The DIMACS format has not been implemented in matlab-igraph.");
    break;
  case MXIGRAPH_FORMAT_GRAPHDB:
    mexIgraphError(
      "igraph:notImplemented",
      "Upstream igraph does not support writing to graphdb format.");
    break;
  case MXIGRAPH_FORMAT_GRAPHML:
    oldhandler = igraph_set_error_handler(igraph_error_handler_ignore);
    errorcode = igraph_write_graph_graphml(&graph, fptr, true);
    if (errorcode == IGRAPH_UNIMPLEMENTED) {
      mexIgraphError("igraph:notSupported",
                     "igraph was compiled without GraphML support.");
    }
    igraph_set_error_handler(oldhandler);
    break;
  case MXIGRAPH_FORMAT_GML:
    errorcode = igraph_write_graph_gml(&graph, fptr,
                                       IGRAPH_WRITE_GML_DEFAULT_SW, 0, NULL);
    break;
  case MXIGRAPH_FORMAT_PAJEK:
    mexIgraphError("igraph:notImplemented",
                   "Upstream igraph does not support writing to dl format.");
    break;
  case MXIGRAPH_FORMAT_DOT:
    errorcode = igraph_write_graph_dot(&graph, fptr);
    break;
  case MXIGRAPH_FORMAT_LEDA:
    if (is_weighted) {
      errorcode = igraph_write_graph_leda(&graph, fptr, NULL, "weight");
    } else {
      errorcode = igraph_write_graph_leda(&graph, fptr, NULL, NULL);
    }
    break;
  default:
    fclose(fptr);
    igraph_destroy(&graph);
    mexErrMsgIdAndTxt("igraph:unknownFileType",
                      "Unrecognized file format %s. See `help igraph.save` for "
                      "available file formats.",
                      format);
    exit(1);
  }

  fclose(fptr);
  igraph_destroy(&graph);

  return errorcode;
}

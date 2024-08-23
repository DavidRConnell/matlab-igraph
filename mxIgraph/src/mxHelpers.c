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

/* Functions useful for debugging / understanding mxIgraph. */

#include <mxIgraph.h>

void mxIgraphPrintGraph(const igraph_t *graph,
                        const igraph_vector_t *weights)
{
  igraph_eit_t eit;
  igraph_integer_t eid;
  char idxstr[20];

  igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_FROM), &eit);

  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    sprintf(idxstr, "(%" IGRAPH_PRId ",%" IGRAPH_PRId ")",
            IGRAPH_FROM(graph, eid) + 1, IGRAPH_TO(graph, eid) + 1);
    mexPrintf("%*s      %g\n", 8, idxstr,
              weights ? (igraph_real_t)VECTOR(*weights)[eid] : 1.0);
    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);
}

// Functions useful for debugging / understanding mxIgraph.

#include <mxIgraph.h>

void mxIgraphPrintGraph(const igraph_t* graph,
                        const igraph_vector_t* weights)
{
  igraph_eit_t eit;
  igraph_integer_t eid;
  char idxstr[20];

  igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_FROM), &eit);

  while (!IGRAPH_EIT_END(eit)) {
    eid = IGRAPH_EIT_GET(eit);
    sprintf(idxstr, "(%"IGRAPH_PRId",%"IGRAPH_PRId")",
            IGRAPH_FROM(graph, eid) + 1, IGRAPH_TO(graph, eid) + 1);
    mexPrintf("%*s      %g\n", 8, idxstr,
              weights ? (igraph_real_t)VECTOR(*weights)[eid] : 1.0);
    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);
}

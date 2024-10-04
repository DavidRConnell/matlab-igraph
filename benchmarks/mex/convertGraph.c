#include <igraph.h>
#include <mxIgraph.h>

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  mxArray const* graph_options = prhs[1];
  igraph_t graph;
  igraph_vector_t weights;

  mxIgraphFromArray(prhs[0], &graph, &weights, graph_options);
  plhs[0] = mxIgraphToArray(&graph, &weights, graph_options);

  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
}

#include <mxIgraph.h>
#include <utils.h>

#include <igraph_community.h>

igraph_error_t mexIgraphModularity(int nlhs, mxArray* plhs[], int nrhs,
                                   mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(3);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxArray const* opts = prhs[2];

  igraph_t graph;
  igraph_vector_t weights;
  igraph_vector_int_t membership;
  igraph_real_t resolution = mxIgraphGetReal(opts, "resolution");
  igraph_bool_t directed = mxIgraphGetBool(opts, "isdirected");
  igraph_real_t modularity;
  igraph_error_t errorcode = IGRAPH_SUCCESS;

  mxIgraphGetGraph(prhs[0], &graph, &weights, directed);
  mxIgraphMembershipFromArray(prhs[1], &membership);

  igraph_modularity(&graph, &membership, &weights, resolution,
                    directed, &modularity);

  plhs[0] = mxCreateDoubleScalar(modularity);

  igraph_vector_int_destroy(&membership);
  igraph_vector_destroy(&weights);
  igraph_destroy(&graph);
}

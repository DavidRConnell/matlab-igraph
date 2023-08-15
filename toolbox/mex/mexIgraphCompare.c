#include <string.h>
#include "igraph.h"
#include "mxIgraph.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  igraph_vector_int_t comm1;
  igraph_vector_int_t comm2;
  igraph_community_comparison_t method;
  igraph_real_t res;

  if (nrhs != 3) {
    mexErrMsgIdAndTxt("Igraph:internal:wrongNumberOfArguments",
                      "%s requires 3 arguments.", mexFunctionName());
  }

  if (nlhs != 1) {
    mexErrMsgIdAndTxt("Igraph:internal:wrongNumberOfOutputsl",
                      "%s produces 1 output", mexFunctionName());
  }

  const char *methods[] = {
    [IGRAPH_COMMCMP_VI] = "vi",
    [IGRAPH_COMMCMP_NMI] = "nmi",
    [IGRAPH_COMMCMP_RAND] = "rand",
    [IGRAPH_COMMCMP_SPLIT_JOIN] = "split_join",
    [IGRAPH_COMMCMP_ADJUSTED_RAND] = "adjusted_rand"
  };
  igraph_integer_t n_methods = sizeof(methods) / sizeof(*methods);
  method = mxIgraphSelectMethod(prhs[2], methods, n_methods);

  if (method == n_methods) {
    mexErrMsgIdAndTxt("Igraph:internal:unknownMethod",
                      "%s is not a known method.", mxArrayToString(prhs[2]));
  }

  mxIgraphArrayToPartition(prhs[0], &comm1);
  mxIgraphArrayToPartition(prhs[1], &comm2);

  igraph_reindex_membership(&comm1, NULL, NULL);
  igraph_reindex_membership(&comm2, NULL, NULL);

  igraph_compare_communities(&comm1, &comm2, &res, method);
  igraph_vector_int_destroy(&comm1);
  igraph_vector_int_destroy(&comm2);

  plhs[0] = mxCreateDoubleScalar((double)res);
}

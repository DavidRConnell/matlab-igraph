#include "mxIgraph.h"
#include "utils.h"

void mexFunction(int UNUSED(nlhs), mxArray* plhs[], int UNUSED(nrhs),
                 mxArray const* UNUSED(prhs[]))
{
  const char* version;

  igraph_version(&version, NULL, NULL, NULL);
  plhs[0] = mxCreateString(version);
}

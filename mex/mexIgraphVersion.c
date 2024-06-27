#include <string.h>

#include <mxIgraph.h>
#include <utils.h>
#include <mexIgraphVersion.h>

#include <igraph_version.h>

void mexFunction(int UNUSED(nlhs), mxArray* plhs[], int nrhs,
                 mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(1);

  char const* type = mxArrayToString(prhs[0]);
  char const* lib_version;
  char const* toolbox_version = MEX_IGRAPH_VERSION;
  igraph_version(&lib_version, NULL, NULL, NULL);

  if (strcmp(type, "library") == 0) {
    plhs[0] = mxCreateString(lib_version);
  } else if (strcmp(type, "toolbox") == 0) {
    // Add 1 to drop the "v" prefix.
    plhs[0] = mxCreateString(toolbox_version + 1);
  } else {
    mwSize ndims = { 1 };
    char const* names[] = {"library", "toolbox"};
    mxArray* version = mxCreateStructArray(1, &ndims, 2, names);

    mxSetField(version, 0, "library", mxCreateString(lib_version));
    mxSetField(version, 0, "toolbox", mxCreateString(toolbox_version + 1));
    plhs[0] = version;
  }
}

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

#include <string.h>
#include <mxIgraph.h>

#include "utils.h"
#include "mexIgraphVersion.h"

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

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

#include "mexIgraphFunctions.h"
#include <mxIgraph.h>

static igraph_bool_t setup_ran = false;

typedef enum {
  MXIGRAPH_FUNC_CENTRALITY = 0,
  MXIGRAPH_FUNC_CLUSTER,
  MXIGRAPH_FUNC_COMPARE,
  MXIGRAPH_FUNC_CORRELATEWITH,
  MXIGRAPH_FUNC_FAMOUS,
  MXIGRAPH_FUNC_GENERATE,
  MXIGRAPH_FUNC_GENERATEPAIR,
  MXIGRAPH_FUNC_ISISOMORPHIC,
  MXIGRAPH_FUNC_ISSUBISOMORPHIC,
  MXIGRAPH_FUNC_ISTREE,
  MXIGRAPH_FUNC_LAYOUT,
  MXIGRAPH_FUNC_MODULARITY,
  MXIGRAPH_FUNC_OPTIMALMODULARITY,
  MXIGRAPH_FUNC_RANDGAME,
  MXIGRAPH_FUNC_READ,
  MXIGRAPH_FUNC_REINDEX_MEMBERSHIP,
  MXIGRAPH_FUNC_REWIRE,
  MXIGRAPH_FUNC_RNG,
  MXIGRAPH_FUNC_VERBOSE,
  MXIGRAPH_FUNC_WRITE,
  MXIGRAPH_FUNC_N
} mexIgraph_funcname_t;

static void mexIgraphSetupHook()
{
  if (!setup_ran) {
    setup_ran = true;

    mxIgraphSetRNG();
    mxIgraphSetDefaultHandlers();
  }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mexIgraphSetupHook();

  mexIgraph_funcname_t function_name;
  mexIgraphFunction_t function;

  const char *function_names[MXIGRAPH_FUNC_N] = {
    [MXIGRAPH_FUNC_CENTRALITY] = "centrality",
    [MXIGRAPH_FUNC_CLUSTER] = "cluster",
    [MXIGRAPH_FUNC_COMPARE] = "compare",
    [MXIGRAPH_FUNC_CORRELATEWITH] = "correlateWith",
    [MXIGRAPH_FUNC_FAMOUS] = "famous",
    [MXIGRAPH_FUNC_GENERATE] = "generate",
    [MXIGRAPH_FUNC_GENERATEPAIR] = "generatePair",
    [MXIGRAPH_FUNC_ISISOMORPHIC] = "isisomorphic",
    [MXIGRAPH_FUNC_ISSUBISOMORPHIC] = "issubisomorphic",
    [MXIGRAPH_FUNC_ISTREE] = "istree",
    [MXIGRAPH_FUNC_LAYOUT] = "layout",
    [MXIGRAPH_FUNC_MODULARITY] = "modularity",
    [MXIGRAPH_FUNC_OPTIMALMODULARITY] = "optimalModularity",
    [MXIGRAPH_FUNC_RANDGAME] = "randgame",
    [MXIGRAPH_FUNC_READ] = "load",
    [MXIGRAPH_FUNC_REINDEX_MEMBERSHIP] = "reindexMembership",
    [MXIGRAPH_FUNC_REWIRE] = "rewire",
    [MXIGRAPH_FUNC_RNG] = "rng",
    [MXIGRAPH_FUNC_VERBOSE] = "verbose",
    [MXIGRAPH_FUNC_WRITE] = "save"
  };

  function_name =
    mxIgraphSelectMethod(prhs[0], function_names, MXIGRAPH_FUNC_N);
  MXIGRAPH_CHECK_METHOD(function_name, prhs[0]);

  mexIgraphFunction_t function_table[MXIGRAPH_FUNC_N] = {
    [MXIGRAPH_FUNC_CENTRALITY] = mexIgraphCentrality,
    [MXIGRAPH_FUNC_CLUSTER] = mexIgraphCluster,
    [MXIGRAPH_FUNC_COMPARE] = mexIgraphCompare,
    [MXIGRAPH_FUNC_CORRELATEWITH] = mexIgraphCorrelateWith,
    [MXIGRAPH_FUNC_FAMOUS] = mexIgraphFamous,
    [MXIGRAPH_FUNC_GENERATE] = mexIgraphGenerate,
    [MXIGRAPH_FUNC_GENERATEPAIR] = mexIgraphGeneratePair,
    [MXIGRAPH_FUNC_ISISOMORPHIC] = mexIgraphIsIsomorphic,
    [MXIGRAPH_FUNC_ISSUBISOMORPHIC] = mexIgraphIsSubIsomorphic,
    [MXIGRAPH_FUNC_ISTREE] = mexIgraphIsTree,
    [MXIGRAPH_FUNC_LAYOUT] = mexIgraphLayout,
    [MXIGRAPH_FUNC_MODULARITY] = mexIgraphModularity,
    [MXIGRAPH_FUNC_OPTIMALMODULARITY] = mexIgraphOptimalModularity,
    [MXIGRAPH_FUNC_RANDGAME] = mexIgraphRandGame,
    [MXIGRAPH_FUNC_READ] = mexIgraphRead,
    [MXIGRAPH_FUNC_REINDEX_MEMBERSHIP] = mexIgraphReindexMembership,
    [MXIGRAPH_FUNC_REWIRE] = mexIgraphRewire,
    [MXIGRAPH_FUNC_RNG] = mexIgraphRNG,
    [MXIGRAPH_FUNC_VERBOSE] = mexIgraphSetHandler,
    [MXIGRAPH_FUNC_WRITE] = mexIgraphWrite
  };

  function = function_table[function_name];

  if (!function) {
    IGRAPH_ERROR_NO_RETURN("Function not implemented", IGRAPH_UNIMPLEMENTED);
  }

  // prhs[0] is function name, do not need to pass it on.
  igraph_error_t rs = function(nlhs, plhs, nrhs - 1, prhs + 1);
  if (rs != IGRAPH_SUCCESS) {
    IGRAPH_ERROR_NO_RETURN("", rs);
  };
}

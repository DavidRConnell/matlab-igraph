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

#ifndef MEXIGRAPHFUNCTIONS_H
#define MEXIGRAPHFUNCTIONS_H

#include <mxIgraph.h>

typedef igraph_error_t (*mexIgraphFunction_t)(int nlhs, mxArray *plhs[],
    int nrhs, mxArray const *prhs[]);

igraph_error_t mexIgraphCentrality(int nlhs, mxArray *plhs[], int nrhs,
                                   mxArray const *prhs[]);
igraph_error_t mexIgraphCluster(int nlhs, mxArray *plhs[], int nrhs,
                                mxArray const *prhs[]);
igraph_error_t mexIgraphCompare(int nlhs, mxArray *plhs[], int nrhs,
                                mxArray const *prhs[]);
igraph_error_t mexIgraphCorrelateWith(int nlhs, mxArray *plhs[], int nrhs,
                                      mxArray const *prhs[]);
igraph_error_t mexIgraphFamous(int nlhs, mxArray *plhs[], int nrhs,
                               mxArray const *prhs[]);
igraph_error_t mexIgraphGenerate(int nlhs, mxArray *plhs[], int nrhs,
                                 mxArray const *prhs[]);
igraph_error_t mexIgraphGeneratePair(int nlhs, mxArray *plhs[], int nrhs,
                                     mxArray const *prhs[]);
igraph_error_t mexIgraphIsIsomorphic(int nlhs, mxArray *plhs[], int nrhs,
                                     mxArray const *prhs[]);
igraph_error_t mexIgraphIsSubIsomorphic(int nlhs, mxArray *plhs[], int nrhs,
                                        mxArray const *prhs[]);
igraph_error_t mexIgraphIsTree(int nlhs, mxArray *plhs[], int nrhs,
                               mxArray const *prhs[]);
igraph_error_t mexIgraphLayout(int nlhs, mxArray *plhs[], int nrhs,
                               mxArray const *prhs[]);
igraph_error_t mexIgraphModularity(int nlhs, mxArray *plhs[], int nrhs,
                                   mxArray const *prhs[]);
igraph_error_t mexIgraphOptimalModularity(int nlhs, mxArray *plhs[], int nrhs,
    mxArray const *prhs[]);
igraph_error_t mexIgraphRandGame(int nlhs, mxArray *plhs[], int nrhs,
                                 mxArray const *prhs[]);
igraph_error_t mexIgraphRead(int nlhs, mxArray *plhs[], int nrhs,
                             mxArray const *prhs[]);
igraph_error_t mexIgraphReindexMembership(int nlhs, mxArray *plhs[], int nrhs,
    mxArray const *prhs[]);
igraph_error_t mexIgraphRewire(int nlhs, mxArray *plhs[], int nrhs,
                               mxArray const *prhs[]);
igraph_error_t mexIgraphRNG(int nlhs, mxArray *plhs[], int nrhs,
                            mxArray const *prhs[]);
igraph_error_t mexIgraphSetHandler(int nlhs, mxArray *plhs[], int nrhs,
                                   mxArray const *prhs[]);
igraph_error_t mexIgraphWrite(int nlhs, mxArray *plhs[], int nrhs,
                              mxArray const *prhs[]);

#endif

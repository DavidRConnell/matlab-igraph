#ifndef MEXIGRAPHFUNCTIONS_H
#define MEXIGRAPHFUNCTIONS_H

#include "mxIgraph.h"

typedef igraph_error_t (*mexIgraphFunction_t)(int nlhs, mxArray* plhs[],
    int nrhs, mxArray const* prhs[]);

igraph_error_t mexIgraphCentrality(int nlhs, mxArray* plhs[], int nrhs,
                                   mxArray const* prhs[]);
igraph_error_t mexIgraphCluster(int nlhs, mxArray* plhs[], int nrhs,
                                mxArray const* prhs[]);
igraph_error_t mexIgraphCompare(int nlhs, mxArray* plhs[], int nrhs,
                                mxArray const* prhs[]);
igraph_error_t mexIgraphCorrelateWith(int nlhs, mxArray* plhs[], int nrhs,
                                      mxArray const* prhs[]);
igraph_error_t mexIgraphFamous(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[]);
igraph_error_t mexIgraphGenerate(int nlhs, mxArray* plhs[], int nrhs,
                                 mxArray const* prhs[]);
igraph_error_t mexIgraphGeneratePair(int nlhs, mxArray* plhs[], int nrhs,
                                     mxArray const* prhs[]);
igraph_error_t mexIgraphIsIsomorphic(int nlhs, mxArray* plhs[], int nrhs,
                                     mxArray const* prhs[]);
igraph_error_t mexIgraphIsSubIsomorphic(int nlhs, mxArray* plhs[], int nrhs,
                                        mxArray const* prhs[]);
igraph_error_t mexIgraphIsTree(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[]);
igraph_error_t mexIgraphLayout(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[]);
igraph_error_t mexIgraphModularity(int nlhs, mxArray* plhs[], int nrhs,
                                   mxArray const* prhs[]);
igraph_error_t mexIgraphOptimalModularity(int nlhs, mxArray* plhs[], int nrhs,
    mxArray const* prhs[]);
igraph_error_t mexIgraphRandGame(int nlhs, mxArray* plhs[], int nrhs,
                                 mxArray const* prhs[]);
igraph_error_t mexIgraphRead(int nlhs, mxArray* plhs[], int nrhs,
                             mxArray const* prhs[]);
igraph_error_t mexIgraphReindexMembership(int nlhs, mxArray* plhs[], int nrhs,
    mxArray const* prhs[]);
igraph_error_t mexIgraphRewire(int nlhs, mxArray* plhs[], int nrhs,
                               mxArray const* prhs[]);
igraph_error_t mexIgraphRNG(int nlhs, mxArray* plhs[], int nrhs,
                            mxArray const* prhs[]);
igraph_error_t mexIgraphSetHandler(int nlhs, mxArray* plhs[], int nrhs,
                                   mxArray const* prhs[]);
igraph_error_t mexIgraphWrite(int nlhs, mxArray* plhs[], int nrhs,
                              mxArray const* prhs[]);

#endif

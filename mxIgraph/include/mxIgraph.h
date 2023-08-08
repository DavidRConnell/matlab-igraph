#ifndef MEX_IGRAPH_H
#define MEX_IGRAPH_H

#include "mex.h"

#include "igraph_vector.h"
#include "igraph_datatype.h"
#include "igraph_error.h"

// mxError
igraph_error_handler_t mxIgraphErrorHandlerMex;
void mxIgraphSetErrorHandler();

// mxPredicate
igraph_bool_t mxIgraphIsSquare(const mxArray *p);
igraph_bool_t mxIgraphIsVector(const mxArray *p);
igraph_bool_t mxIgraphIsWeighted(const mxArray *p);
igraph_bool_t mxIgraphIsDirected(const mxArray *p);
igraph_bool_t mxIgraphIsTriU(const mxArray *p);
igraph_bool_t mxIgraphIsTriL(const mxArray *p);
igraph_bool_t mxIgraphIsSymmetric(const mxArray *p);

// mxPartition
igraph_integer_t mxIgraphVectorLength(const mxArray *p);
int mxIgraphGetVectorInt(const mxArray *p, igraph_vector_int_t *vec);
int mxIgraphArrayToPartition(const mxArray *p,
                             igraph_vector_int_t *membership);
mxArray *mxIgraphCreatePartition(igraph_vector_int_t const *membership);

// mxGraph
igraph_integer_t mxIgraphVCount(const mxArray *p);
igraph_integer_t mxIgraphECount(const mxArray *p,
                                const igraph_bool_t directed);
void mxIgraphGetGraph(const mxArray *p, igraph_t *graph,
                      igraph_vector_t *weights, const igraph_bool_t directed);
mxArray *mxIgraphCreateAdj(const igraph_t *graph,
                           const igraph_vector_t *weights,
                           const igraph_bool_t sparse);

// mxHelpers
void mxIgraphPrintGraph(const igraph_t *graph,
                        const igraph_vector_t *weights);

#endif

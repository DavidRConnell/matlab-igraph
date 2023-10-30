#ifndef MEX_IGRAPH_H
#define MEX_IGRAPH_H

#include "mex.h"
#include "igraph.h"

// types
typedef enum {
  MXIGRAPH_DTYPE_LOGICAL = 0,
  MXIGRAPH_DTYPE_DOUBLE,
  MXIGRAPH_DTYPE_N
} mxIgraphDType_t;

typedef enum {
  MXIGRAPH_FORMAT_EDGELIST = 0,
  MXIGRAPH_FORMAT_NCOL,
  MXIGRAPH_FORMAT_LGL,
  MXIGRAPH_FORMAT_DIMACS,
  MXIGRAPH_FORMAT_GRAPHDB,
  MXIGRAPH_FORMAT_GRAPHML,
  MXIGRAPH_FORMAT_GML,
  MXIGRAPH_FORMAT_PAJEK,
  MXIGRAPH_FORMAT_DL,
  MXIGRAPH_FORMAT_DOT,
  MXIGRAPH_FORMAT_LEDA,
  MXIGRAPH_FORMAT_N
} mxIgraphFileFormat_t;

// mxError
igraph_error_handler_t mxIgraphErrorHandlerMex;
void mxIgraphSetErrorHandler();
void mxIgraphErrorNotImplemented(const char *caller, const char *method);
void mxIgraphErrorUnknownMethod(const char *caller, const char *method);

// mxPredicate
igraph_bool_t mxIgraphIsSquare(const mxArray *p);
igraph_bool_t mxIgraphIsVector(const mxArray *p);
igraph_bool_t mxIgraphIsEmpty(const mxArray *p);
igraph_bool_t mxIgraphIsWeighted(const mxArray *p);
igraph_bool_t mxIgraphIsDirected(const mxArray *p);
igraph_bool_t mxIgraphIsTriU(const mxArray *p);
igraph_bool_t mxIgraphIsTriL(const mxArray *p);
igraph_bool_t mxIgraphIsSymmetric(const mxArray *p);

// mxPartition
igraph_integer_t mxIgraphVectorLength(const mxArray *p);
int mxIgraphArrayToPartition(const mxArray *p,
                             igraph_vector_int_t *membership);
mxArray *mxIgraphCreatePartition(igraph_vector_int_t const *membership);

// mxGraph
igraph_integer_t mxIgraphVCount(const mxArray *p);
igraph_integer_t mxIgraphECount(const mxArray *p,
                                const igraph_bool_t is_directed);
void mxIgraphGetGraph(const mxArray *p, igraph_t *graph,
                      igraph_vector_t *weights, const igraph_bool_t is_directed);
mxArray *mxIgraphCreateAdj(const igraph_t *graph,
                           const igraph_vector_t *weights,
                           const igraph_bool_t sparse,
                           const mxIgraphDType_t type);

// mxStructures
int mxIgraphGetVector(const mxArray *p, igraph_vector_t *vec);
int mxIgraphGetVectorInt(const mxArray *p, igraph_vector_int_t *vec);
int mxIgraphGetMatrix(const mxArray *p, igraph_matrix_t *mat);
mxArray *mxIgraphCreateCellFromVectorIntList(const igraph_vector_int_list_t
    *list);
mxArray *mxIgraphCreateMatrixFromVectorIntList(const igraph_vector_int_list_t
    *list);

// mxHelpers
void mxIgraphPrintGraph(const igraph_t *graph,
                        const igraph_vector_t *weights);

// mxArgumentParsers
igraph_integer_t mxIgraphSelectMethod(const mxArray *p, const char *methods[],
                                      const igraph_integer_t n_methods);
mxIgraphFileFormat_t mxIgraphSelectFileFormat(const mxArray *p);
mxIgraphDType_t mxIgraphSelectDType(const mxArray *p);

#endif

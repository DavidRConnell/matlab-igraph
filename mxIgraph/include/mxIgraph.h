#ifndef MEX_IGRAPH_H
#define MEX_IGRAPH_H

#include "mex.h"
#if defined(__GNUC__)
#  undef printf
#  define printf(...) __attribute__((format(mexPrintf, ...)))
#endif

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

// mxHandler
igraph_error_handler_t mxIgraphErrorHandlerMex;
igraph_warning_handler_t mxIgraphWarningHandlerMex;
igraph_warning_handler_t mxIgraphWarningHandlerIgnoreMex;
igraph_progress_handler_t mxIgraphProgressHandlerMex;
igraph_progress_handler_t mxIgraphProgressHandlerIgnoreMex;
igraph_status_handler_t mxIgraphStatusHandlerMex;
igraph_status_handler_t mxIgraphStatusHandlerIgnoreMex;

// mxError
void mxIgraphErrorNotImplemented(const char* caller, const char* method);
void mxIgraphErrorUnknownMethod(const char* caller, const char* method);

// mxPredicate
igraph_bool_t mxIgraphIsSquare(const mxArray* p);
igraph_bool_t mxIgraphIsVector(const mxArray* p);
igraph_bool_t mxIgraphIsEmpty(const mxArray* p);
igraph_bool_t mxIgraphIsWeighted(const mxArray* p);
igraph_bool_t mxIgraphIsDirected(const mxArray* p);
igraph_bool_t mxIgraphIsTriU(const mxArray* p);
igraph_bool_t mxIgraphIsTriL(const mxArray* p);
igraph_bool_t mxIgraphIsSymmetric(const mxArray* p);

// mxPartition
igraph_integer_t mxIgraphVectorLength(const mxArray* p);
int mxIgraphMembershipFromArray(const mxArray* p,
                                igraph_vector_int_t* membership);
mxArray* mxIgraphMembershipToArray(igraph_vector_int_t const* membership);

// mxGraph
igraph_integer_t mxIgraphVCount(const mxArray* p);
igraph_integer_t mxIgraphECount(const mxArray* p,
                                const igraph_bool_t is_directed);
void mxIgraphGetGraph(const mxArray* p, igraph_t* graph,
                      igraph_vector_t* weights, const igraph_bool_t is_directed);
mxArray* mxIgraphCreateAdj(igraph_t const* graph,
                           igraph_vector_t const* weights,
                           mxArray const* adjOptions);

// mxStructures
int mxIgraphVectorFromArray(const mxArray* p, igraph_vector_t* vec,
                            igraph_bool_t const shift_start);
mxArray* mxIgraphVectorToArray(const igraph_vector_t* vec,
                               igraph_bool_t const shift_start);
int mxIgraphVectorIntFromArray(const mxArray* p, igraph_vector_int_t* vec,
                               igraph_bool_t const shift_start);
mxArray* mxIgraphVectorIntToArray(const igraph_vector_int_t* vec,
                                  igraph_bool_t const shift_start);
int mxIgraphVectorBoolFromArray(const mxArray* p, igraph_vector_bool_t* vec,
                                igraph_bool_t const shift_start);
mxArray* mxIgraphVectorBoolToArray(const igraph_vector_bool_t* vec,
                                   igraph_bool_t const shift_start);
int mxIgraphMatrixFromArray(const mxArray* p, igraph_matrix_t* mat,
                            igraph_bool_t const shift_start);
mxArray* mxIgraphMatrixToArray(const igraph_matrix_t* mat,
                               igraph_bool_t const shift_start);
int mxIgraphMatrixIntFromArray(const mxArray* p, igraph_matrix_int_t* mat,
                               igraph_bool_t const shift_start);
mxArray* mxIgraphMatrixIntToArray(const igraph_matrix_int_t* mat,
                                  igraph_bool_t const shift_start);
mxArray* mxIgraphCreateCellFromVectorIntList(const igraph_vector_int_list_t
    *list, igraph_bool_t const shift_start);
mxArray* mxIgraphCreateMatrixFromVectorIntList(const igraph_vector_int_list_t
    *list, igraph_bool_t const shift_start);

// mxHelpers
void mxIgraphPrintGraph(const igraph_t* graph,
                        const igraph_vector_t* weights);

// mxArgumentParsers
igraph_integer_t mxIgraphSelectMethod(const mxArray* p, const char* methods[],
                                      const igraph_integer_t n_methods);
mxIgraphFileFormat_t mxIgraphSelectFileFormat(const mxArray* p);

mxArray* mxIgraphGetArgument(mxArray const* arg_struct,
                             char const fieldname[1]);
mxIgraphDType_t mxIgraphSelectDType(const mxArray* arg_struct);
igraph_neimode_t mxIgraphSelectMode(const mxArray* arg_struct);
igraph_integer_t mxIgraphGetInteger(const mxArray* arg_struct,
                                    char const fieldname[1]);
igraph_real_t mxIgraphGetReal(const mxArray* arg_struct,
                              char const fieldname[1]);
igraph_bool_t mxIgraphGetBool(const mxArray* arg_struct,
                              char const fieldname[1]);
char* mxIgraphGetString(const mxArray* arg_struct, char const fieldname[1]);
void mxIgraphGetVector(const mxArray* arg_struct, char const fieldname[1],
                       igraph_vector_t* vec, igraph_bool_t const shift_start);
void mxIgraphGetVectorInt(const mxArray* arg_struct, char const fieldname[1],
                          igraph_vector_int_t* vec,
                          igraph_bool_t const shift_start);
void mxIgraphGetVectorBool(const mxArray* arg_struct, char const fieldname[1],
                           igraph_vector_bool_t* vec,
                           igraph_bool_t const shift_start);
void mxIgraphGetMatrix(const mxArray* arg_struct, char const fieldname[1],
                       igraph_matrix_t* mat, igraph_bool_t const shift_start);

#endif

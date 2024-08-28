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

#ifndef MEX_IGRAPH_H
#define MEX_IGRAPH_H

#include <mex.h>
#if defined(__GNUC__)
#undef printf
#define printf(...) __attribute__((format(mexPrintf, ...)))
#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include <igraph.h>

/* Helper for passing weights when the graph might be unweighted. */
#define MXIGRAPH_WEIGHTS(weights)                                              \
  igraph_vector_size(weights) > 0 ? weights : NULL

// types
typedef enum {
  MXIGRAPH_DTYPE_LOGICAL = 0,
  MXIGRAPH_DTYPE_DOUBLE,
  MXIGRAPH_DTYPE_N
} mxIgraphDType_t;

typedef enum {
  MXIGRAPH_REPR_GRAPH = 0,
  MXIGRAPH_REPR_FULL,
  MXIGRAPH_REPR_SPARSE,
  MXIGRAPH_REPR_N
} mxIgraphRepr_t;

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

enum { MXIGRAPH_IDX_KEEP = 0, MXIGRAPH_IDX_SHIFT };

// mxHandler
igraph_error_handler_t mxIgraphErrorHandlerMex;
igraph_warning_handler_t mxIgraphWarningHandlerMex;
igraph_warning_handler_t mxIgraphWarningHandlerIgnoreMex;
igraph_progress_handler_t mxIgraphProgressHandlerMex;
igraph_progress_handler_t mxIgraphProgressHandlerIgnoreMex;
igraph_status_handler_t mxIgraphStatusHandlerMex;
igraph_status_handler_t mxIgraphStatusHandlerIgnoreMex;
igraph_interruption_handler_t mxIgraphInterruptionHandlerMex;
igraph_fatal_handler_t mxIgraphFatelHandlerMex;

// mxError
void mxIgraphSetError(igraph_error_t new_code);
igraph_error_t mxIgraphGetError(void);
void mxIgraphSetErrorMsg(char const *msg, ...);
char *mxIgraphGetErrorMsg(void);

#define MXIGRAPH_CHECK_STATUS()                                                \
  do {                                                                         \
    igraph_error_t code = mxIgraphGetError();                                  \
    if (code != IGRAPH_SUCCESS) {                                              \
      char const *reason = mxIgraphGetErrorMsg();                              \
      IGRAPH_ERROR(reason, code);                                              \
    }                                                                          \
  } while (0)

// mxRandom
EXTERNC void mxIgraphSetRNG(void);

// mxPredicate
igraph_bool_t mxIgraphIsSquare(const mxArray *p);
igraph_bool_t mxIgraphIsVector(const mxArray *p);
igraph_bool_t mxIgraphIsEmpty(const mxArray *p);
igraph_bool_t mxIgraphIsWeighted(const mxArray *p);
igraph_bool_t mxIgraphIsGraph(const mxArray *p);
igraph_bool_t mxIgraphIsDirected(const mxArray *p);
igraph_bool_t mxIgraphIsTriU(const mxArray *p);
igraph_bool_t mxIgraphIsTriL(const mxArray *p);
igraph_bool_t mxIgraphIsSymmetric(const mxArray *p);

// mxGraph
igraph_integer_t mxIgraphVCount(mxArray const *p);
igraph_integer_t mxIgraphECount(mxArray const *p,
                                const igraph_bool_t is_directed);
void mxIgraphFromArray(mxArray const *p, igraph_t *graph,
                       igraph_vector_t *weights, mxArray const *graph_options);
mxArray *mxIgraphToArray(igraph_t const *graph,
                         igraph_vector_t const *weights,
                         mxArray const *graphOpts);

// mxStructures
int mxIgraphVectorFromArray(const mxArray *p, igraph_vector_t *vec,
                            igraph_bool_t const shift_start);
mxArray *mxIgraphVectorToArray(const igraph_vector_t *vec,
                               igraph_bool_t const shift_start);

int mxIgraphVectorIntFromArray(const mxArray *p, igraph_vector_int_t *vec,
                               igraph_bool_t const shift_start);
mxArray *mxIgraphVectorIntToArray(const igraph_vector_int_t *vec,
                                  igraph_bool_t const shift_start);

int mxIgraphVectorBoolFromArray(const mxArray *p, igraph_vector_bool_t *vec,
                                igraph_bool_t const shift_start);
mxArray *mxIgraphVectorBoolToArray(const igraph_vector_bool_t *vec,
                                   igraph_bool_t const shift_start);

int mxIgraphMatrixFromArray(const mxArray *p, igraph_matrix_t *mat,
                            igraph_bool_t const shift_start);
mxArray *mxIgraphMatrixToArray(const igraph_matrix_t *mat,
                               igraph_bool_t const shift_start);

int mxIgraphMatrixIntFromArray(const mxArray *p, igraph_matrix_int_t *mat,
                               igraph_bool_t const shift_start);
mxArray *mxIgraphMatrixIntToArray(const igraph_matrix_int_t *mat,
                                  igraph_bool_t const shift_start);

int mxIgraphMatrixBoolFromArray(const mxArray *p, igraph_matrix_bool_t *mat,
                                igraph_bool_t const shift_start);
mxArray *mxIgraphMatrixBoolToArray(const igraph_matrix_bool_t *mat,
                                   igraph_bool_t const shift_start);

int mxIgraphVectorListFromArray(const mxArray *p,
                                igraph_vector_int_list_t *mat,
                                igraph_bool_t const shift_start);
mxArray *mxIgraphVectorListToArray(const igraph_vector_int_list_t *list,
                                   igraph_bool_t const shift_start);

// mxHelpers
void mxIgraphPrintGraph(const igraph_t *graph,
                        const igraph_vector_t *weights);

// mxArgumentParsers
#define MXIGRAPH_CHECK_METHOD(expr, name_ptr)                                  \
  do {                                                                         \
    igraph_integer_t res = (expr);                                             \
    if (res == -1) {                                                           \
      IGRAPH_FATALF("Recieved unexpected method name, %s.",                    \
                    mxArrayToString(name_ptr));                                \
    }                                                                          \
  } while (0)

igraph_integer_t mxIgraphSelectMethod(const mxArray *p, const char *methods[],
                                      const igraph_integer_t n_methods);
mxIgraphFileFormat_t mxIgraphSelectFileFormat(const mxArray *p);

mxArray *mxIgraphGetArgument(mxArray const *arg_struct,
                             char const fieldname[1]);

mxIgraphRepr_t mxIgraphReprFromOptions(const mxArray *arg_struct);
mxIgraphDType_t mxIgraphDTypeFromOptions(const mxArray *arg_struct);
igraph_neimode_t mxIgraphModeFromOptions(const mxArray *arg_struct);

igraph_integer_t mxIgraphIntegerFromOptions(const mxArray *arg_struct,
    char const fieldname[1]);
igraph_real_t mxIgraphRealFromOptions(const mxArray *arg_struct,
                                      char const fieldname[1]);
igraph_bool_t mxIgraphBoolFromOptions(const mxArray *arg_struct,
                                      char const fieldname[1]);
char *mxIgraphStringFromOptions(const mxArray *arg_struct,
                                char const fieldname[1]);

void mxIgraphVectorFromOptions(const mxArray *arg_struct,
                               char const fieldname[1], igraph_vector_t *vec,
                               igraph_bool_t const shift_start);
void mxIgraphVectorIntFromOptions(const mxArray *arg_struct,
                                  char const fieldname[1],
                                  igraph_vector_int_t *vec,
                                  igraph_bool_t const shift_start);
void mxIgraphVectorBoolFromOptions(const mxArray *arg_struct,
                                   char const fieldname[1],
                                   igraph_vector_bool_t *vec,
                                   igraph_bool_t const shift_start);

void mxIgraphMatrixFromOptions(const mxArray *arg_struct,
                               char const fieldname[1], igraph_matrix_t *mat,
                               igraph_bool_t const shift_start);
void mxIgraphMatrixIntFromOptions(const mxArray *arg_struct,
                                  char const fieldname[1],
                                  igraph_matrix_int_t *mat,
                                  igraph_bool_t const shift_start);
void mxIgraphMatrixBoolFromOptions(const mxArray *arg_struct,
                                   char const fieldname[1],
                                   igraph_matrix_bool_t *mat,
                                   igraph_bool_t const shift_start);

#endif

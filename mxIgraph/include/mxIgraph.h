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
# undef printf
# define printf(...) __attribute__((format(mexPrintf, ...)))
#endif

#ifdef __cplusplus
# define EXTERNC extern "C"
#else
# define EXTERNC
#endif

#include <igraph.h>

/* Helper for passing weights when the graph might be unweighted. */
#define MXIGRAPH_WEIGHTS(weights)                                             \
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
void mxIgraphSetDefaultHandlers(void);

// mxError
void mxIgraphSetError(igraph_error_t new_code);
igraph_error_t mxIgraphGetError(void);
void mxIgraphSetErrorMsg(char const* msg, ...);
char* mxIgraphGetErrorMsg(void);

#define MXIGRAPH_CHECK_STATUS()                                               \
  do {                                                                        \
    igraph_error_t code = mxIgraphGetError();                                 \
    if (code != IGRAPH_SUCCESS) {                                             \
      char const* reason = mxIgraphGetErrorMsg();                             \
      IGRAPH_ERROR(reason, code);                                             \
    }                                                                         \
  } while (0)

#define MXIGRAPH_CHECK_STATUS_RETURN(val)                                     \
  do {                                                                        \
    igraph_error_t code = mxIgraphGetError();                                 \
    if (code != IGRAPH_SUCCESS) {                                             \
      char const* reason = mxIgraphGetErrorMsg();                             \
      IGRAPH_ERROR_NO_RETURN(reason, code);                                   \
      return val;                                                             \
    }                                                                         \
  } while (0)

// mxRandom
EXTERNC void mxIgraphSetRNG(void);

// mxPredicate
igraph_bool_t mxIgraphIsSquare(mxArray const* p);
igraph_bool_t mxIgraphIsVector(mxArray const* p);
igraph_bool_t mxIgraphIsEmpty(mxArray const* p);
igraph_bool_t mxIgraphIsWeighted(mxArray const* p);
igraph_bool_t mxIgraphIsGraph(mxArray const* p);
igraph_bool_t mxIgraphIsDirected(mxArray const* p);
igraph_bool_t mxIgraphIsTriU(mxArray const* p);
igraph_bool_t mxIgraphIsTriL(mxArray const* p);
igraph_bool_t mxIgraphIsSymmetric(mxArray const* p);

// mxGraph
igraph_integer_t mxIgraphVCount(mxArray const* p);
igraph_integer_t mxIgraphECount(
  mxArray const* p, igraph_bool_t const is_directed);
igraph_error_t mxIgraphFromArray(mxArray const* p, igraph_t* graph,
  igraph_vector_t* weights, mxArray const* graph_options);
mxArray* mxIgraphToArray(igraph_t const* graph, igraph_vector_t const* weights,
  mxArray const* graphOpts);

// mxStructures
igraph_error_t mxIgraphVectorFromArray(
  mxArray const* p, igraph_vector_t* vec, igraph_bool_t const shift_start);
mxArray* mxIgraphVectorToArray(
  igraph_vector_t const* vec, igraph_bool_t const shift_start);

igraph_error_t mxIgraphVectorIntFromArray(
  mxArray const* p, igraph_vector_int_t* vec, igraph_bool_t const shift_start);
mxArray* mxIgraphVectorIntToArray(
  igraph_vector_int_t const* vec, igraph_bool_t const shift_start);

igraph_error_t mxIgraphVectorBoolFromArray(mxArray const* p,
  igraph_vector_bool_t* vec, igraph_bool_t const shift_start);
mxArray* mxIgraphVectorBoolToArray(
  igraph_vector_bool_t const* vec, igraph_bool_t const shift_start);

igraph_error_t mxIgraphMatrixFromArray(
  mxArray const* p, igraph_matrix_t* mat, igraph_bool_t const shift_start);
mxArray* mxIgraphMatrixToArray(
  igraph_matrix_t const* mat, igraph_bool_t const shift_start);

igraph_error_t mxIgraphMatrixIntFromArray(
  mxArray const* p, igraph_matrix_int_t* mat, igraph_bool_t const shift_start);
mxArray* mxIgraphMatrixIntToArray(
  igraph_matrix_int_t const* mat, igraph_bool_t const shift_start);

igraph_error_t mxIgraphMatrixBoolFromArray(mxArray const* p,
  igraph_matrix_bool_t* mat, igraph_bool_t const shift_start);
mxArray* mxIgraphMatrixBoolToArray(
  igraph_matrix_bool_t const* mat, igraph_bool_t const shift_start);

igraph_error_t mxIgraphVectorListFromArray(mxArray const* p,
  igraph_vector_int_list_t* mat, igraph_bool_t const shift_start);
mxArray* mxIgraphVectorListToArray(
  igraph_vector_int_list_t const* list, igraph_bool_t const shift_start);

// mxHelpers
void mxIgraphPrintGraph(igraph_t const* graph, igraph_vector_t const* weights);

// mxArgumentParsers
#define MXIGRAPH_CHECK_METHOD(expr, name_ptr)                                 \
  do {                                                                        \
    igraph_integer_t res = (expr);                                            \
    if (res == -1) {                                                          \
      IGRAPH_FATALF(                                                          \
        "Recieved unexpected method name, %s.", mxArrayToString(name_ptr));   \
    }                                                                         \
  } while (0)

igraph_integer_t mxIgraphSelectMethod(
  mxArray const* p, char const* methods[], igraph_integer_t const n_methods);
mxIgraphFileFormat_t mxIgraphSelectFileFormat(mxArray const* p);

mxArray* mxIgraphGetArgument(
  mxArray const* arg_struct, char const fieldname[1]);

mxIgraphRepr_t mxIgraphReprFromOptions(mxArray const* arg_struct);
mxIgraphDType_t mxIgraphDTypeFromOptions(mxArray const* arg_struct);
igraph_neimode_t mxIgraphModeFromOptions(mxArray const* arg_struct);

igraph_integer_t mxIgraphIntegerFromOptions(
  mxArray const* arg_struct, char const fieldname[1]);
igraph_real_t mxIgraphRealFromOptions(
  mxArray const* arg_struct, char const fieldname[1]);
igraph_bool_t mxIgraphBoolFromOptions(
  mxArray const* arg_struct, char const fieldname[1]);
char* mxIgraphStringFromOptions(
  mxArray const* arg_struct, char const fieldname[1]);

igraph_error_t mxIgraphVectorFromOptions(mxArray const* arg_struct,
  char const fieldname[1], igraph_vector_t* vec,
  igraph_bool_t const shift_start);
igraph_error_t mxIgraphVectorIntFromOptions(mxArray const* arg_struct,
  char const fieldname[1], igraph_vector_int_t* vec,
  igraph_bool_t const shift_start);
igraph_error_t mxIgraphVectorBoolFromOptions(mxArray const* arg_struct,
  char const fieldname[1], igraph_vector_bool_t* vec,
  igraph_bool_t const shift_start);

igraph_error_t mxIgraphMatrixFromOptions(mxArray const* arg_struct,
  char const fieldname[1], igraph_matrix_t* mat,
  igraph_bool_t const shift_start);
igraph_error_t mxIgraphMatrixIntFromOptions(mxArray const* arg_struct,
  char const fieldname[1], igraph_matrix_int_t* mat,
  igraph_bool_t const shift_start);
igraph_error_t mxIgraphMatrixBoolFromOptions(mxArray const* arg_struct,
  char const fieldname[1], igraph_matrix_bool_t* mat,
  igraph_bool_t const shift_start);

#endif

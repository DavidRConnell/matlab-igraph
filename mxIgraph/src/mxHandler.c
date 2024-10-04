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

#include <mxIgraph.h>
#include <string.h>

/* mxErrId
 * \brief MATLAB error ID for igraph errors.
 * Named error IDs for select igraph errors to be added to the error call.
 *
 * WARNING: IGRAPH_ENOSOL is last error. This could change.
 */
static char* const mxErrId[IGRAPH_ENOSOL + 1] = {
  [IGRAPH_ENOMEM] = "outOfMemory",
  [IGRAPH_PARSEERROR] = "parseError",
  [IGRAPH_EINVAL] = "invalidValue",
  [IGRAPH_EXISTS] = "attributeAlreadyExists",
  [IGRAPH_EINVEVECTOR] = "invalidVertixVector",
  [IGRAPH_EINVVID] = "invalidVertixId",
  [IGRAPH_NONSQUARE] = "notSquare",
  [IGRAPH_EINVMODE] = "invalidMode",
  [IGRAPH_EFILE] = "fileError",
  [IGRAPH_UNIMPLEMENTED] = "notImplemented",
  [IGRAPH_DIVERGED] = "failedToConverge",
  [IGRAPH_EDIVZERO] = "divideByZero",
  [IGRAPH_EOVERFLOW] = "overflow",
  [IGRAPH_CPUTIME] = "exceededMaxTime",
  [IGRAPH_EUNDERFLOW] = "underflow",
  [IGRAPH_ERANGE] = "outOfRange",
};

static size_t mxIgraphBaseName_i(char const* fpath)
{
  size_t pos = strlen(fpath);
  while (fpath[--pos] != '/')
    ;
  return pos + 1;
}

void mxIgraphErrorHandlerMex(
  char const* reason, char const* file, int line, igraph_error_t igraph_errno)
{
  char const* errmsg = igraph_strerror(igraph_errno);
  igraph_bool_t const reason_given = strlen(reason) > 0;
  char const* fname = file + mxIgraphBaseName_i(file);
  char const* id = mxErrId[igraph_errno] ? mxErrId[igraph_errno] : "internal";
  char fullid[512] = "igraph:\0";

  strncat(fullid, id, sizeof(fullid) - 1);

  mxIgraphSetError(IGRAPH_SUCCESS);
  mxIgraphSetErrorMsg("");
  IGRAPH_FINALLY_FREE();

  mexErrMsgIdAndTxt(fullid, "\n%s In %s (line %d)%s%s", errmsg, fname, line,
    reason_given ? "\n    " : "", reason);
}

void mxIgraphFatalHandlerMex(char const* reason, char const* file, int line)
{
  IGRAPH_FINALLY_FREE();
  mexErrMsgIdAndTxt("igraph:internal",
    "\n%s\n\nFatal error In %s (line %d)\n    %s", reason,
    file + mxIgraphBaseName_i(file), line,
    "Process terminated due to a fatal error. This is likely a bug. Please "
    "open an issue on github with the steps needed to reproduce the error.");
}

void mxIgraphWarningHandlerMex(char const* reason, char const* file, int line)
{
  int strmax = 512;
  char msg[strmax];
  char const* fname = file + mxIgraphBaseName_i(file);

  snprintf(msg, strmax - 1, "%s\n\nIn %s (line %d)", reason, fname, line);
  mexWarnMsgTxt(msg);
}

void mxIgraphWarningHandlerIgnoreMex(
  char const* reason, char const* file, int line)
{
  IGRAPH_UNUSED(reason);
  IGRAPH_UNUSED(file);
  IGRAPH_UNUSED(line);
}

igraph_error_t mxIgraphProgressHandlerMex(
  char const* message, igraph_real_t percent, void* data)
{
  IGRAPH_UNUSED(message);
  IGRAPH_UNUSED(data);

  igraph_integer_t n_chunks = 20;
  igraph_integer_t chunks_complete = ceil(n_chunks * percent / 100);
  mexPrintf("\r  Progress: [");
  for (igraph_integer_t i = 0; i < chunks_complete; i++) {
    mexPrintf("#");
  }
  for (igraph_integer_t i = chunks_complete; i < n_chunks; i++) {
    mexPrintf("-");
  }
  mexPrintf("] %g%%", percent);
  fflush(stdout);

  if (percent == 100) {
    mexPrintf("\n");
  }

  return IGRAPH_SUCCESS;
}

igraph_error_t mxIgraphProgressHandlerIgnoreMex(
  char const* message, igraph_real_t percent, void* data)
{
  IGRAPH_UNUSED(message);
  IGRAPH_UNUSED(percent);
  IGRAPH_UNUSED(data);

  return IGRAPH_SUCCESS;
}

igraph_error_t mxIgraphStatusHandlerMex(char const* message, void* data)
{
  IGRAPH_UNUSED(data);

  mexPrintf("%s", message);
  return IGRAPH_SUCCESS;
}

igraph_error_t mxIgraphStatusHandlerIgnoreMex(char const* message, void* data)
{
  IGRAPH_UNUSED(message);
  IGRAPH_UNUSED(data);

  return IGRAPH_SUCCESS;
}

bool utIsInterruptPending(void); // From undocumented libut.

igraph_error_t mxIgraphInterruptionHandlerMex(void* data)
{
  IGRAPH_UNUSED(data);

  if (utIsInterruptPending()) {
    return IGRAPH_INTERRUPTED;
  }

  return IGRAPH_SUCCESS;
}

// NOTE: defaults to silent progress handler. Should explicit turn on progress
// handler if desired. Since progress handler is useful when dealing with slow
// tasks but can clutter stdout when always on.
void mxIgraphSetDefaultHandlers(void)
{
  igraph_set_error_handler(mxIgraphErrorHandlerMex);
  igraph_set_warning_handler(mxIgraphWarningHandlerMex);
  igraph_set_fatal_handler(mxIgraphFatalHandlerMex);
  igraph_set_status_handler(mxIgraphStatusHandlerMex);
  igraph_set_progress_handler(mxIgraphProgressHandlerIgnoreMex);
  igraph_set_interruption_handler(mxIgraphInterruptionHandlerMex);
}

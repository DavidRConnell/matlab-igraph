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

void mxIgraphErrorHandlerMex(const char* reason, const char* file,
                             int line, igraph_error_t igraph_errno)
{
  const char* errmsg = igraph_strerror(igraph_errno);
  /* May be an issue in multi-threaded code when a different thread tries to
  call a freed object before the current thread can send the error. */
  IGRAPH_FINALLY_FREE();
  mexErrMsgIdAndTxt("Igraph:internal", "%s: %s\n\n%s -- %d\n", errmsg, reason,
                    file, line);
}

void mxIgraphWarningHandlerMex(const char* reason, const char* file, int line)
{
  int strmax = 512;
  char msg[strmax];

  snprintf(msg, strmax - 1, "%s\n\n> In %s (line %d)\n", reason, file, line);
  mexWarnMsgTxt(msg);
}

void mxIgraphWarningHandlerIgnoreMex(const char* reason, const char* file,
                                     int line)
{
  IGRAPH_UNUSED(reason);
  IGRAPH_UNUSED(file);
  IGRAPH_UNUSED(line);
}

igraph_error_t mxIgraphProgressHandlerMex(const char* message,
    igraph_real_t percent,
    void* data)
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

igraph_error_t mxIgraphProgressHandlerIgnoreMex(const char* message,
    igraph_real_t percent,
    void* data)
{
  IGRAPH_UNUSED(message);
  IGRAPH_UNUSED(percent);
  IGRAPH_UNUSED(data);

  return IGRAPH_SUCCESS;
}

igraph_error_t mxIgraphStatusHandlerMex(const char* message, void* data)
{
  IGRAPH_UNUSED(data);

  mexPrintf("%s\n", message);
  return IGRAPH_SUCCESS;
}

igraph_error_t mxIgraphStatusHandlerIgnoreMex(const char* message, void* data)
{
  IGRAPH_UNUSED(message);
  IGRAPH_UNUSED(data);

  return IGRAPH_SUCCESS;
}

#include "mxIgraph.h"

static igraph_bool_t setup_ran = false;

void mxIgraphErrorHandlerMex(const char *reason, const char *file,
                             int line, igraph_error_t igraph_errno)
{
  const char *errmsg = igraph_strerror(igraph_errno);
  /* May be an issue in multi-threaded code when a different thread tries to
  call a freed object before the current thread can send the error. */
  IGRAPH_FINALLY_FREE();
  mexErrMsgIdAndTxt("Igraph:internal", "%s: %s\n\n%s -- %d\n", errmsg, reason,
                    file, line);
}

void mxIgraphWarningHandlerMex(const char *reason, const char *file, int line)
{
  int strmax = 512;
  char msg[strmax];

  snprintf(msg, strmax - 1, "%s\n\n> In %s (line %d)\n", reason, file, line);
  mexWarnMsgTxt(msg);
}

igraph_error_t mxIgraphProgressHandlerMex(const char *message,
    igraph_real_t percent,
    void *data)
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
  mexPrintf("] %%%g", percent);
  fflush(stdout);

  return IGRAPH_SUCCESS;
}

igraph_error_t mxIgraphStatusHandlerMex(const char *message, void *data)
{
  IGRAPH_UNUSED(data);

  mexPrintf("%s\n", message);
  return IGRAPH_SUCCESS;
}

void mxIgraphSetupHook()
{
  if (!setup_ran) {
    setup_ran = true;
    igraph_set_error_handler(mxIgraphErrorHandlerMex);
    igraph_set_warning_handler(mxIgraphWarningHandlerMex);
    igraph_set_progress_handler(mxIgraphProgressHandlerMex);
    igraph_set_status_handler(mxIgraphStatusHandlerMex);
  }
}

void mxIgraphErrorNotImplemented(const char *caller, const char *method)
{
  mexErrMsgIdAndTxt("Igraph:notImplemented",
                    "%s method %s has not been implemented.\n\n%s", caller, method,
                    "has not been implemented yet.",
                    "Please report an issue or submit a pull request to github.");
}

void mxIgraphErrorUnknownMethod(const char *caller, const char *method)
{
  mexErrMsgIdAndTxt("Igraph:internal:unknownMethod",
                    "Unknown %s method %s.\n\n%s",
                    caller, method, "Please report an issue to github.");
}

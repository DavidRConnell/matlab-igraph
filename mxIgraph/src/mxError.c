#include "mxIgraph.h"

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

void mxIgraphSetErrorHandler()
{
  igraph_set_error_handler(mxIgraphErrorHandlerMex);
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

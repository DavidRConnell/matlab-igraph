#include "mxIgraph.h"

void mxIgraphErrorHandlerMex(const char *reason, const char *file,
                             int line, igraph_error_t igraph_errno)
{
  const char *errmsg = igraph_strerror(igraph_errno);
  /* May be an issue in multi-threaded code when a different thread tries to
  call a freed object before the current thread can send the error. */
  IGRAPH_FINALLY_FREE();
  mexErrMsgIdAndTxt("Igraph:Internal", "Error at %s:%i : %s - %s.\n", file,
                    line, reason, errmsg);
}

void mxIgraphSetErrorHandler()
{
  igraph_set_error_handler(mxIgraphErrorHandlerMex);
}

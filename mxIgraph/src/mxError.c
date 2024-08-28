#include <mxIgraph.h>

static igraph_error_t mxIgraphError = IGRAPH_SUCCESS;
static char mxIgraphErrorMsg[1024] = "\0";

void mxIgraphSetError(igraph_error_t const new_code)
{
  mxIgraphError = new_code;
}

igraph_error_t mxIgraphGetError(void)
{
  return mxIgraphError;
}

void mxIgraphSetErrorMsg(char const *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(mxIgraphErrorMsg, sizeof(mxIgraphErrorMsg) - 1, fmt, ap);
  va_end(ap);
}

char *mxIgraphGetErrorMsg(void)
{
  return mxIgraphErrorMsg;
}

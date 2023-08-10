#include <string.h>

#include "mxIgraph.h"

igraph_integer_t mxIgraphSelectMethod(const char *name, const char *methods[],
                                      const igraph_integer_t n_methods)
{
  for (igraph_integer_t i = 0; i <= n_methods; i++) {
    if (i == n_methods) {
      mexErrMsgIdAndTxt("Igraph:internal:unknownMethod",
                        "%s is not a known method.", name);
    }

    if (strcmp(name, methods[i]) == 0) {
      return i;
    }
  }

  // Will never get here but quites compiler.
  return -1;
}

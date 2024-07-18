#include <mxIgraph.h>

void mxIgraphErrorNotImplemented(const char* caller, const char* method)
{
  mexErrMsgIdAndTxt("Igraph:notImplemented",
                    "%s method %s has not been implemented.\n\n%s", caller, method,
                    "has not been implemented yet.",
                    "Please report an issue or submit a pull request to github.");
}

void mxIgraphErrorUnknownMethod(const char* caller, const char* method)
{
  mexErrMsgIdAndTxt("Igraph:internal:unknownMethod",
                    "Unknown %s method %s.\n\n%s",
                    caller, method, "Please report an issue to github.");
}

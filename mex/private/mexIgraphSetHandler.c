#include <mxIgraph.h>
#include "utils.h"

igraph_error_t mexIgraphSetHandler(int nlhs, mxArray* UNUSED(plhs[]),
                                   int nrhs, mxArray const* prhs[])
{
  VERIFY_N_INPUTS_EQUAL(2);
  VERIFY_N_OUTPUTS_EQUAL(0);

  typedef enum {
    MXIGRAPH_HANDLER_WARNING = 0,
    MXIGRAPH_HANDLER_PROGRESS,
    MXIGRAPH_HANDLER_STATUS,
    MXIGRAPH_HANDLER_N
  } mxIgraph_handler_t;

  mxIgraph_handler_t type;
  igraph_bool_t verbose = mxGetScalar(prhs[1]);

  const char* handlers[MXIGRAPH_HANDLER_N] = {
    [MXIGRAPH_HANDLER_WARNING] = "warning",
    [MXIGRAPH_HANDLER_PROGRESS] = "progress",
    [MXIGRAPH_HANDLER_STATUS] = "status",
  };

  type = mxIgraphSelectMethod(prhs[0], handlers, MXIGRAPH_HANDLER_N);

  switch (type) {
  case MXIGRAPH_HANDLER_WARNING:
    if (verbose) {
      igraph_set_warning_handler(mxIgraphWarningHandlerMex);
    } else {
      igraph_set_warning_handler(mxIgraphWarningHandlerIgnoreMex);
    }
    break;
  case MXIGRAPH_HANDLER_PROGRESS:
    if (verbose) {
      igraph_set_progress_handler(mxIgraphProgressHandlerMex);
    } else {
      igraph_set_progress_handler(mxIgraphProgressHandlerIgnoreMex);
    }
    break;
  case MXIGRAPH_HANDLER_STATUS:
    if (verbose) {
      igraph_set_status_handler(mxIgraphStatusHandlerMex);
    } else {
      igraph_set_status_handler(mxIgraphStatusHandlerIgnoreMex);
    }
    break;
  default:
    mxIgraphErrorNotImplemented("Handler", mxArrayToString(prhs[0]));
  };

  return IGRAPH_SUCCESS;
}

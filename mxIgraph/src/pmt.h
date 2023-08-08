#ifndef PMT_H
#define PMT_H

#include <string.h>

#define STREQ(a, b) strcmp(a, b) == 0

typedef enum {
  MXIGRAPH_LOGICAL = 0,
  MXIGRAPH_DOUBLE,
  MXIGRAPH_NTYPES
} mxIgraph_type_t;

static char *mxIgraphType_i[MXIGRAPH_NTYPES] = {
  [MXIGRAPH_LOGICAL] = "bool",
  [MXIGRAPH_DOUBLE] = "double"
};

static char *mxIgraphGetter_i[MXIGRAPH_NTYPES] = {
  [MXIGRAPH_LOGICAL] = "mxGetLogicals",
  [MXIGRAPH_DOUBLE] = "mxGetDoubles"
};

static char *mxIgraphConstructer_i[MXIGRAPH_NTYPES] = {
  [MXIGRAPH_LOGICAL] = "mxCreateLogicalMatrix",
  [MXIGRAPH_DOUBLE] = "mxCreateDoubleMatrix"
};

char *mxIgraphType(mxIgraph_type_t type)
{
  return mxIgraphType_i[type];
}

char *mxIgraphGetter(mxIgraph_type_t type)
{
  return mxIgraphGetter_i[type];
}

char *mxIgraphConstructer(mxIgraph_type_t type)
{
  return mxIgraphConstructer_i[type];
}

#define DISPATCHER(ret_type, name)		\
  (ret_type) mxIgraph(name)(const mxArray *p)	\
  {						\
    if (mxIsSparse(p)) {			\
      if (mxIsLogical(p)) {			\
	return mxIgraph(name)SparseLogical(p);	\
      }						\
      return mxIgraph(name)SparseDouble(p);	\
    }						\
    if (mxIsLogical(p)) {			\
      return mxIgraph(name)FullLogical(p);	\
    }						\
    return mxIgraph(name)FullDouble(p);		\
  }

#endif

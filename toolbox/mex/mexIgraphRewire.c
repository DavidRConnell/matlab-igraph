#include "mxIgraph.h"
#include "utils.h"

#define VERIFY_N_OPTIONALS(expected)			\
  do {							\
    if (n_optionals != (expected)) {			\
      mexErrMsgIdAndTxt("Igraph:interenal:n_optionals",	\
			"%s expected %s optionals.",	\
			method, (expected));		\
    }							\
  } while (0)

typedef enum {
  MXIGRAPH_GENERATOR_STAR = 0,
  MXIGRAPH_GENERATOR_WHEEL,
  MXIGRAPH_GENERATOR_RING,
  MXIGRAPH_GENERATOR_SQUARE_LATTICE,
  MXIGRAPH_GENERATOR_TRIANGULAR_LATTICE,
  MXIGRAPH_GENERATOR_HEXAGONAL_LATTICE,
  MXIGRAPH_GENERATOR_KARY_TREE,
  MXIGRAPH_GENERATOR_SYMMETRIC_TREE,
  MXIGRAPH_GENERATOR_REGULAR_TREE,
  MXIGRAPH_GENERATOR_FULL,
  MXIGRAPH_GENERATOR_CITATION,
  MXIGRAPH_GENERATOR_MULTIPARTITE,
  MXIGRAPH_GENERATOR_TURAN,
  MXIGRAPH_GENERATOR_REALIZE_DEGREE_SEQUENCE,
  MXIGRAPH_GENERATOR_PRUFER,
  MXIGRAPH_GENERATOR_ATLAS,
  MXIGRAPH_GENERATOR_DE_BRUIJN,
  MXIGRAPH_GENERATOR_KAUTZ,
  MXIGRAPH_GENERATOR_CIRCULANT,
  MXIGRAPH_GENERATOR_PETERSEN,
  MXIGRAPH_GENERATOR_CHORDAL_RING,
  MXIGRAPH_GENERATOR_N
} mxIgraph_generator_t;

static igraph_error_t mxIgraph_star_i(const mxArray *p_optionals[],
                                      igraph_t *graph)
{
  const char *modes[] = {
    [IGRAPH_STAR_OUT] = "out",
    [IGRAPH_STAR_IN] = "in",
    [IGRAPH_STAR_MUTUAL] = "mutual",
    [IGRAPH_STAR_UNDIRECTED] = "undirected"
  };
  const igraph_integer_t n_modes = 4;
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_star_mode_t mode = mxIgraphSelectMethod(p_optionals[1], modes,
                            n_modes);
  igraph_integer_t center = mxGetScalar(p_optionals[2]);

  if (mode == n_modes) {
    mexErrMsgIdAndTxt("Igraph:unknownMode", "Unknown mode passed to star.");
  }

  return igraph_star(graph, n_nodes, mode, center);
}

static igraph_error_t mxIgraph_wheel_i(const mxArray *p_optionals[],
                                       igraph_t *graph)
{
  const char *modes[] = {
    [IGRAPH_WHEEL_OUT] = "out",
    [IGRAPH_WHEEL_IN] = "in",
    [IGRAPH_WHEEL_MUTUAL] = "mutual",
    [IGRAPH_WHEEL_UNDIRECTED] = "undirected"
  };
  const igraph_integer_t n_modes = 4;
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_wheel_mode_t mode = mxIgraphSelectMethod(p_optionals[1], modes,
                             n_modes);
  igraph_integer_t center = mxGetScalar(p_optionals[2]);

  if (mode == n_modes) {
    mexErrMsgIdAndTxt("Igraph:unknownMode", "Unknown mode passed to wheel.");
  }

  return igraph_wheel(graph, n_nodes, mode, center);
}

static igraph_error_t mxIgraph_ring_i(const mxArray *p_optionals[],
                                      igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_bool_t is_directed = mxGetScalar(p_optionals[1]);
  igraph_bool_t is_mutual = mxGetScalar(p_optionals[2]);
  igraph_bool_t is_circular = mxGetScalar(p_optionals[3]);

  return igraph_ring(graph, n_nodes, is_directed, is_mutual, is_circular);
}

static igraph_error_t mxIgraph_kary_tree_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  const char *types[] = {
    [IGRAPH_TREE_OUT] = "out",
    [IGRAPH_TREE_IN] = "in",
    [IGRAPH_TREE_UNDIRECTED] = "undirected"
  };
  const igraph_integer_t n_types = 3;
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t children = mxGetScalar(p_optionals[1]);
  igraph_tree_mode_t type = mxIgraphSelectMethod(p_optionals[2], types,
                            n_types);

  if (type == n_types) {
    mexErrMsgIdAndTxt("Igraph:unknownMode", "Unknown mode passed to kary tree.");
  }

  return igraph_kary_tree(graph, n_nodes, children, type);
}

static igraph_error_t mxIgraph_regular_tree_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  const char *types[] = {
    [IGRAPH_TREE_OUT] = "out",
    [IGRAPH_TREE_IN] = "in",
    [IGRAPH_TREE_UNDIRECTED] = "undirected"
  };
  const igraph_integer_t n_types = 3;
  igraph_integer_t height = mxGetScalar(p_optionals[0]);
  igraph_integer_t degree = mxGetScalar(p_optionals[1]);
  igraph_tree_mode_t type = mxIgraphSelectMethod(p_optionals[2], types,
                            n_types);

  if (type == n_types) {
    mexErrMsgIdAndTxt("Igraph:unknownMode",
                      "Unknown mode passed to regular tree.");
  }

  return igraph_regular_tree(graph, height, degree, type);
}

static igraph_error_t mxIgraph_full_i(const mxArray *p_optionals[],
                                      igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_bool_t is_directed = mxGetScalar(p_optionals[1]);
  igraph_bool_t loops = mxGetScalar(p_optionals[2]);

  return igraph_full(graph, n_nodes, is_directed, loops);
}

static igraph_error_t mxIgraph_citation_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_bool_t is_directed = mxGetScalar(p_optionals[1]);

  return igraph_full_citation(graph, n_nodes, is_directed);
}

static igraph_error_t mxIgraph_prufer_i(const mxArray *p_optionals[],
                                        igraph_t *graph)
{
  igraph_vector_int_t prufer;
  igraph_error_t errcode;

  mxIgraphGetVectorInt(p_optionals[0], &prufer);

  errcode = igraph_from_prufer(graph, &prufer);
  igraph_vector_int_destroy(&prufer);

  return errcode;
}

static igraph_error_t mxIgraph_atlas_i(const mxArray *p_optionals[],
                                       igraph_t *graph)
{
  igraph_integer_t atlas_id = mxGetScalar(p_optionals[0]);

  return igraph_atlas(graph, atlas_id);
}

static igraph_error_t mxIgraph_de_bruijn_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_letters = mxGetScalar(p_optionals[0]);
  igraph_integer_t str_len = mxGetScalar(p_optionals[1]);

  return igraph_de_bruijn(graph, n_letters, str_len);
}

static igraph_error_t mxIgraph_kautz_i(const mxArray *p_optionals[],
                                       igraph_t *graph)
{
  igraph_integer_t n_letters = mxGetScalar(p_optionals[0]);
  igraph_integer_t str_len = mxGetScalar(p_optionals[1]);

  return igraph_kautz(graph, n_letters, str_len);
}

static igraph_error_t mxIgraph_circulant_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_vector_int_t shifts;
  igraph_bool_t is_directed = mxGetScalar(p_optionals[2]);
  igraph_error_t errcode;

  mxIgraphGetVectorInt(p_optionals[1], &shifts);

  errcode = igraph_circulant(graph, n_nodes, &shifts, is_directed);
  igraph_vector_int_destroy(&shifts);

  return errcode;
}

static igraph_error_t mxIgraph_petersen_i(const mxArray *p_optionals[],
    igraph_t *graph)
{
  igraph_integer_t n_nodes = mxGetScalar(p_optionals[0]);
  igraph_integer_t shift = mxGetScalar(p_optionals[1]);

  return igraph_generalized_petersen(graph, n_nodes, shift);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mxIgraphSetErrorHandler();

  igraph_integer_t n_required = 3;
  igraph_integer_t n_optionals = nrhs - n_required;
  const mxArray **p_optionals = prhs + n_required;

  VERIFY_N_INPUTS_ATLEAST(n_required);
  VERIFY_N_OUTPUTS_EQUAL(1);

  mxIgraph_generator_t method;
  igraph_bool_t make_sparse = mxGetScalar(prhs[1]);
  mxIgraphDType_t dtype = mxIgraphSelectDType(prhs[2]);
  igraph_t graph;
  igraph_error_t errorcode;
  igraph_error_t (*generator_func)(const mxArray **, igraph_t *);

  const char *generators[MXIGRAPH_GENERATOR_N] = {
    [MXIGRAPH_GENERATOR_STAR] = "star",
    [MXIGRAPH_GENERATOR_WHEEL] = "wheel",
    [MXIGRAPH_GENERATOR_RING] = "ring",
    [MXIGRAPH_GENERATOR_SQUARE_LATTICE] = "square_lattice",
    [MXIGRAPH_GENERATOR_TRIANGULAR_LATTICE] = "triangular_lattice",
    [MXIGRAPH_GENERATOR_HEXAGONAL_LATTICE] = "hexagonal_lattice",
    [MXIGRAPH_GENERATOR_KARY_TREE] = "kary_tree",
    [MXIGRAPH_GENERATOR_SYMMETRIC_TREE] = "symmetric_tree",
    [MXIGRAPH_GENERATOR_REGULAR_TREE] = "regular_tree",
    [MXIGRAPH_GENERATOR_FULL] = "full",
    [MXIGRAPH_GENERATOR_CITATION] = "citation",
    [MXIGRAPH_GENERATOR_MULTIPARTITE] = "multipartite",
    [MXIGRAPH_GENERATOR_TURAN] = "turan",
    [MXIGRAPH_GENERATOR_REALIZE_DEGREE_SEQUENCE] = "realize_degree_sequence",
    [MXIGRAPH_GENERATOR_PRUFER] = "prufer",
    [MXIGRAPH_GENERATOR_ATLAS] = "atlas",
    [MXIGRAPH_GENERATOR_DE_BRUIJN] = "de_bruijn",
    [MXIGRAPH_GENERATOR_KAUTZ] = "kautz",
    [MXIGRAPH_GENERATOR_CIRCULANT] = "circulant",
    [MXIGRAPH_GENERATOR_PETERSEN] = "petersen",
    [MXIGRAPH_GENERATOR_CHORDAL_RING] = "chordal_ring"
  };

  method = mxIgraphSelectMethod(prhs[0], generators,
                                MXIGRAPH_GENERATOR_N);

  switch (method) {
  case MXIGRAPH_GENERATOR_STAR:
    VERIFY_N_OPTIONALS(3);
    generator_func = mxIgraph_star_i;
    break;
  case MXIGRAPH_GENERATOR_WHEEL:
    VERIFY_N_OPTIONALS(3);
    generator_func = mxIgraph_wheel_i;
    break;
  case MXIGRAPH_GENERATOR_RING:
    VERIFY_N_OPTIONALS(4);
    generator_func = mxIgraph_ring_i;
    break;
  case MXIGRAPH_GENERATOR_KARY_TREE:
    VERIFY_N_OPTIONALS(3);
    generator_func = mxIgraph_kary_tree_i;
    break;
  case MXIGRAPH_GENERATOR_REGULAR_TREE:
    VERIFY_N_OPTIONALS(3);
    generator_func = mxIgraph_regular_tree_i;
    break;
  case MXIGRAPH_GENERATOR_FULL:
    VERIFY_N_OPTIONALS(3);
    generator_func = mxIgraph_full_i;
    break;
  case MXIGRAPH_GENERATOR_CITATION:
    VERIFY_N_OPTIONALS(2);
    generator_func = mxIgraph_citation_i;
    break;
  case MXIGRAPH_GENERATOR_PRUFER:
    VERIFY_N_OPTIONALS(1);
    generator_func = mxIgraph_prufer_i;
    break;
  case MXIGRAPH_GENERATOR_ATLAS:
    VERIFY_N_OPTIONALS(1);
    generator_func = mxIgraph_atlas_i;
    break;
  case MXIGRAPH_GENERATOR_DE_BRUIJN:
    VERIFY_N_OPTIONALS(2);
    generator_func = mxIgraph_de_bruijn_i;
    break;
  case MXIGRAPH_GENERATOR_KAUTZ:
    VERIFY_N_OPTIONALS(2);
    generator_func = mxIgraph_kautz_i;
    break;
  case MXIGRAPH_GENERATOR_CIRCULANT:
    VERIFY_N_OPTIONALS(3);
    generator_func = mxIgraph_circulant_i;
    break;
  case MXIGRAPH_GENERATOR_PETERSEN:
    VERIFY_N_OPTIONALS(2);
    generator_func = mxIgraph_petersen_i;
    break;
  case MXIGRAPH_GENERATOR_N:
    mxIgraphErrorUnknownMethod(mexFunctionName(), mxArrayToString(prhs[2]));
    exit(1);
  default:
    mxIgraphErrorNotImplemented("Generate", mxArrayToString(prhs[2]));
    exit(1);
  }

  if ((errorcode = generator_func(p_optionals, &graph)) != IGRAPH_SUCCESS) {
    igraph_destroy(&graph);
    switch (errorcode) {
    case IGRAPH_ENOMEM:
      mexErrMsgIdAndTxt("Igraph:outOfMemory",
                        "Failed to generate graph due to insufficient memory.");
      break;
    case IGRAPH_EINVAL:
      mexErrMsgIdAndTxt("Igraph:invalidParameter", "Invalid method parameter.");
      break;
    default:
      mexErrMsgIdAndTxt("Igraph:internal:generate",
                        "Failed to generate a %s graph.", generators[method]);
    }
  }

  plhs[0] = mxIgraphCreateAdj(&graph, NULL, make_sparse, dtype);

  igraph_destroy(&graph);
}

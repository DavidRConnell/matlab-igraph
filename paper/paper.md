---
title: '`matlab-igraph`: bringing igraph to MATLAB'
tags:
  - matlab
  - graph algorithms
authors:
  - name: David R. Connell
  - orcid: 0000-0002-4841-6756
  - affiliation: 1
affiliations:
  - name: Rush University Medical College, United States
    index: 1
	ror: 01j7c0b24
date: 18 June 2025
bibliography: paper.bib
---

# Summary

High-throughput lab techniques have enabled systems biology to view medicine through networks rather than individual genes or gene pairs [@goh07_human_diseas_networ].
Using graph approaches, we can learn generic patterns that apply to many sets of genes, proteins, diseases, etc simultaneously, vastly improving the rate we can find disease genes and enables transferring knowledge between disorders.
We created the `matlab-igraph`, a toolbox that integrates the efficient graph algorithms and tools of the igraph C library [@csardi06_igrap_softw] into MATLAB's environment.
By representing graphs with MATLAB's builtin types, we maintain a simple syntax while leveraging compiled C code to simplify graph analysis without sacrificing efficiency, enabling users to focus on their experiments.

# Statement of need

MATLAB is a powerful tool for numerical analysis that is common in academia alongside Python and R, but unlike Python and R, it lacked support for igraph, a gap filled by `matlab-igraph`.
Without `matlab-igraph`, researchers would need to rewrite algorithms in specifically for MATLAB, which not only takes time but also risks errors.
Development of `matlab-igraph` begin with work on the SpeakEasy2 community detection algorithm, which needed to score partitions, using methods like modularity and normalized mutual information, and compare to current community detection functions such as Leiden and InfoMAP [@gaiteri23_robus_scalab_infor_clust_diver_biolog_networ].
By making graph algorithms available to MATLAB, the `matlab-igraph` toolbox allows easy comparisons against state-of-the-art methods.
This package has two goals, bring igraph functions into MATLAB for end users and allow development of new graph methods using the `mxIgraph` interface library and the `igutils` namespace.

## Use in MATLAB

To take advantage of MATLAB's strong matrix support, we designed the toolbox to support representing graphs as adjacency matrices, allowing co-operation between MATLAB builtins and igraph functions.
For graphs that require more metadata, that final "graph" representation can be requested to use MATLAB's builtin `graph` or `digraph` types, which explicitly track directedness and can store igraph's node and edge attributes.
All functions that accept a graph can read `graph` classes or matrices and functions that return a graph accept the `repr` keyword to choose between a full matrix, sparse matrix, or graph class.
This design choice streamlines workflows, allowing researchers to combine MATLAB’s native functions with igraph’s algorithms without data format conversions or redundant coding.

## mxIgraph

In addition to the igraph functions, we provide a standard for creating new graph functions using MATLAB's mex engine and the igraph C library to compile code for use in MATLAB.
To aid with this, the toolbox includes the bridge C library `mxIgraph` for communicating between MATLAB's C API and igraph and the `igutils` namespace for consistent argument handling throughout the toolbox.
The `mxIgraph` library exposes functions for converting between MATLAB and igraph data types, predicates for working with graphs, and a set of methods for parsing argument structures created by MATLAB's argument blocks in C.
When large graphs are involved, C can reduce the memory demand over writing in MATLAB.
Specifically, "outer--reduce" patterns or when using parallel computation.
When performing matrix algebra, a common pattern is performing an outer-product like function on two vectors and then reducing over an axis.
For an $m \times{} 1$ and a $n \times{} 1$ vector, this leads to an $m \times n$ intermediary matrix that is then reduced back down to a vector.
By using for-loops in C instead of high-level linear algebra packages, the final vector can be created directly, without needing to store a temporary matrix, for large values of $n$ and $m$ this can be the difference between fitting in memory or not.
If the code can be written to run in parallel, C supports multithreading, in contrast to MATLAB's parallel toolbox which is based on multiprocessing, a technique that requires cloning data across each worker, potentially exhausting memory when used with large graphs, while high overhead can cause performance to be worse than serial processing for smaller graphs.
Multithreading allows memory to be shared across thread, enabling efficient parallel processing on all graphs without requiring the MATLAB parallel toolbox.

The `igutils` namespace within the toolbox complements the `mxIgraph` by standardizing parsing of common arguments, ensuring consistency across functions.
For example, `igutils` provides argument parser classes for functions that accept graphs and a second for those that return a graph, which organize common arguments and their acceptable values.
If `mxIgraph` is extended to handle other types in the future, all MATLAB functions using the `igutils` classes and predicates for argument parsing will automatically be updated as well. Together, these components lower the barrier to implemented efficient graph code for MATLAB.

# References

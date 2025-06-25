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

With the development of high-throughput lab techniques, systems biology has been able to show the value of viewing medicine through networks rather than looking at individual genes or gene pairs [@goh07_human_diseas_networ].
Using graph approaches, we can learn generic patterns that apply to many sets of genes, proteins, diseases, etc simultaneously, vastly improving the rate at which we can find disease genes and enables transferring knowledge learned studying one disease to another.
The `matlab-igraph` toolbox brings computationally efficient graph algorithms provided in the igraph C library to the MATLAB environment.
The igraph C library provides a large collection of graph algorithms and other tools for working with graphs [@csardi06_igrap_softw].
By interfacing with igraph, we provide a simple syntax working with graphs while still achieving the performance of low-level compiled code in an analogous way to the simple matrix syntax provided by base MATLAB.

# Statement of need

MATLAB is a powerful tool for numerical analysis that is common in medical research along with Python and R.
The later two language's have igraph packages but until now igraph has been missing from the MATLAB environment.
By making graph algorithms available to MATLAB, the igraph toolbox allows benchmarking new algorithms against state of the art methods.
Development of `matlab-igraph` begin with the `SpeakEasy2` community detection project, which needed to score partitions and compare to current community detection functions such as Leiden and InfoMAP [@gaiteri23_robus_scalab_infor_clust_diver_biolog_networ].
Without `matlab-igraph`, this requires rewriting the algorithms specifically for MATLAB, which not only takes time but also risks making mistakes.
This package has two goals, bring igraph functions into MATLAB for end users and allow development of new graph methods using the `mxIgraph` interface library and the `igutils` namespace.

## Use in MATLAB

To take advantage of MATLAB's strong matrix support, graph's are often represented as adjacency matrices using either dense or sparse matrices.
Our toolbox is designed to support this usage to seamlessly integrate with MATLAB, allowing co-operation between MATLAB builtins and igraph functions.
Any function that accepts a graph can read a matrix and any function that returns a graph accepts the `repr` key word to choose between a full, sparse, or graph type.
For graphs that require more metadata, that final "graph" representation can be requested to use MATLAB's builtin graph types, which explicitly tracks whether the graph is directed or undirected and can store igraph attributes.

## mxIgraph

In addition to the igraph functions, we provide a standard for creating new graph functions using the igraph library.
For some graph algorithms C is required over MATLAB in order to handle larger graphs.
This is particularly important if parallel computing can be used.
MATLAB's parallel toolbox uses multi-processing for computation while C is supports multi-threading.
Multi-processing requires cloning the current process, including starting new instances of MATLAB and giving each instance it's own copy of memory, including adjacency matrices, which in the case of large graphs can quickly exhaust memory.
Multi-threading enables the same parallel computation but can be performed in the parent process so each thread can use the same memory.
Additionally, this will run even on computers without the MATLAB parallel toolbox installed.

The core of this feature set is the `mxIgraph` bridge library, which exposes functions for converting between MATLAB and igraph data types, predicates for working with graphs, and a set of methods for parsing MATLAB function arguments.
The `igutils` namespace within the toolbox handles common argument types, ensuring consistency across functions.
For example, `igutils` provides argument parser classes for functions that accept graphs and a second for those that return a graph, such as the above `repr` argument.
If `mxIgraph` is extended to handle other types in the future, all MATLAB functions using the `igutils` predicates for argument parsing will automatically be updated as well.

# References

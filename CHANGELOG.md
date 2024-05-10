# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Reindex membership.

### Changed

- Bump igraph version to 0.10.12.

## [0.1.13] 2024-04-01

### Added

- Integer matrix converters for mxIgraph.
- Conversion method for graph file formats.

### Changed

- Add a `shift_start` parameter to data converters that allow changing between
  0 and 1-based indexing.
- Prevent save from overwriting files by default.

## [0.1.12] 2024-03-15

### Added

- Mixing parameter option for randgames that use preference matrix.

## [0.1.11] 2024-03-15

### Added

- Logo

### Fixed

- Minor typos in help blocks.
- Mistake in compare's argument block.

## [0.1.10] 2024-03-07

### Fixed

- Mistakes in layout and cluster related to moving to mexIgraphDispatcher.

### Changed

- Build on older Ubuntu for users using older C libs.

## [0.1.9] 2024-02-27

### Fixed

- Seeding random number generators.

### Changed

- `igraph.version` provides version for both toolbox and C library.
- Update igraph version to 0.10.10.
- Move to a single C entry point to better preserve state across function
  calls.

## [0.1.8] 2024-02-06

### Fixed

- Compile with older versions gcc/g++ on Linux to fix GLIBCXX version issue.

## [0.1.7] 2024-02-06

Fake update to try to get File Exchange to change the toolbox file it grabs.

## [0.1.6] 2024-02-06

### Fixed

- Post install script not dependent on what the toolbox gets named locally.
- Post install script gets name of arch that matches mexext function.
- Use RPATH instead of RUNPATH to prevent LD_LIBRARY_PATH from having priority.

### Changed

- Post install function deletes itself after being called.
- Remove OS specific toolboxes to ensure File Exchange gets appropriate toolbox
  (since it only grabs one and I can't specify which).

## [0.1.5] 2024-02-05

### Added

- Package Linux toolbox with runtime dependencies (#2).
- Create system independent toolbox with all supported system's files.

### Fixed

- Ship runtime dependencies for windows (#3).

### Changed

- Pin igraph C library to release 0.10.9.
- Update docs with modified installation instructions.

## [0.1.4] 2024-02-03

### Added

- Instructions for File Exchange users.
- Open in MATLAB online badge.

### Changed

- Move GettingStarted.mlx to from ./doc to root of the toolbox.

## [0.1.3] 2024-02-03

### Fixed

- Version numbers on releases match tags.

### Changed

- Release as MATLAB toolboxes (mltbx) files.
- Use the architecture names MATLAB uses for naming files.

## [0.1.2] 2024-02-02

### Added

- Workflow for automating building and packaging toolbox.
- Automatic release pre-compiled toolboxes.

### Fixed

- Modify CMake so it compiles on Windows and macOS.

### Changed

- Use CMake's "Object" library for mxIgraph instead of a shared library.
- Package now only contain toolbox.

## [0.1.1] 2024-02-01

### Added

- This changelog.

### Changed

- Use CMake instead of GNUMake for portability.
- Move mex source code out of toolbox since they're not needed at runtime.
- Compile `mxIgraph` into a standalone library.

### Fixed

- Add the format attribute to mexPrintf to prevent extraneous compiler warnings

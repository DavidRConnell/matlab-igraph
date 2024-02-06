# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.7] 2024-02-06

Fake update to try to get File Exchange to change the toolbox file it grabs.

## [0.1.6] 2024-02-06

### Fixed

- Post install script not dependent on what the toolbox gets named locally.
- Post install script gets name of arch that matches mexext function.
- Use RPATH instead of RUNPATH to prevent LD_LIBRARY_PATH from having priority.

### Changed

- Post install function deletes itself after being called.
- Remove OS specific toolboxes to ensure File Exchange gets appropriate toolbox (since it only grabs one and I can specify which).

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

# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

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

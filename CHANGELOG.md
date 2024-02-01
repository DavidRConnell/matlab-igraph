# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- This changelog.

### Change

- Use CMake instead of GNUMake for portability.
- Move mex source code out of toolbox since they're not needed at runtime.

### Fix

- Add the format attribute to mexPrintf to prevent extraneous compiler warnings

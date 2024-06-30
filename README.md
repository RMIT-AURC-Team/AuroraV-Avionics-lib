# AuroraV Avionics Library
![HIVE Aurora V footer](./img/banner.png)

## Table of Contents
<!-- mtoc-start -->

* [Overview](#overview)
* [Subprojects](#subprojects)
* [Requirements](#requirements)
  * [Windows](#windows)
  * [Linux](#linux)
* [Building](#building)
* [Usage](#usage)
* [Extending with Additional Subprojects](#extending-with-additional-subprojects)

<!-- mtoc-end -->

## Overview
The AuroraV Avionics Library is a comprehensive suite of tools and libraries designed to aid in avionics development. This repository contains multiple subprojects, each focused on a specific aspect of avionics software.

## Subprojects

1. **Kalman Filter**: Implementation of Kalman filtering techniques for sensor data fusion.
2. **Memory Buffer**: Implementation of a circular memory buffer.
3. **Quaternion**: Quaternion mathematics library for 3D rotations.

## Requirements

### Windows

1. **Compiler**: A ``GCC`` or equivalent compiler is required. ``GCC`` is recommended for building for x86 targets, if building for ARM you can use ``arm-gcc`` or ``Keil MDK``.
2. **GNU tools**: The build system depends on many GNU based tools such as ``ld`` or ``mkdir``, for Windows the easiest way to ensure these are available is to install [Git Bash](https://git-scm.com/downloads) or equivalent. In case some executables are not missing you may have to search for a Windows port to install.
3. **Make**: Ensure that ``make`` is available in your system's ``PATH``.

### Linux

1. **GCC**: GCC should be pre-installed on most Linux distributions. If not, install it using your package manager.
   ```sh
   sudo apt-get install build-essential
   ```
2. **Make**: Install make if it's not already installed.
   ```sh
   sudo apt-get install make
   ```

## Building

1. Open a new terminal.
2. Navigate to the project directory.
3. Run ``make`` to build the project. An optional target for testing subprojects can be invoked as ``make test``.

## Usage

Once built, the library will be compiled to a single archive ``libavionics.a`` inside the created ``/bin`` directory. Includes for all subprojects are linked under ``/inc``. 

To add this library to an existing project, simply add the binary and add ``/inc`` to your project linker's search path.

## Extending with Additional Subprojects

To add a new subproject, follow these steps:

1. **Create the Subproject Directory**: 
   Create a new directory for your subproject at the root level of the repository.

2. **Create the Required Files**: 
   Inside your new directory, create the following files:
   - `README.md`: Describe the subproject and its functionality.
   - `makefile`: Include build instructions.
   - `src/`: Directory containing the source code files (`.c` and `.h`).
   - `test/`: Directory containing the test files. 


3. **Modify the Top-Level Makefile**:
   Add an entry for your new subproject in the top-level `makefile` to include it in the build process.

4. **Example Skeleton**:

   An example project with the following structure can be found in the ``/example`` directory of this repository. This includes skeleton code for the implementations and makefile.

   ```
   example/
   ├── README.md
   ├── makefile
   ├── src/
   │   ├── new_subproject.c
   │   └── new_subproject.h
   └── test/
       └── test_new_subproject.c
   ```

   Since the top level makefile invokes the makefiles of subprojects to compile their source code, tests can easily be implemented beyond the C code examples. An example of this is shown in the ``kalmanfilter`` subproject, using python to execute testing.

![HIVE Aurora V footer](./img/footer.png)
---

# Conway's Game of Life (C++)

Conway's Game of Life implemented in C++ - multiple implementation variations. The main version uses [SFML](https://www.sfml-dev.org/index.php) for rendering the simulation.

## Build

From the project root run
```shell
make
```

**NOTE -** This program currently depends on [OpenMP](https://www.openmp.org/), so the compiler needs to support it. Additionally, it also depends on [SFML](https://www.sfml-dev.org/index.php) being installed on the system to compile.

## Usage
- To run the main program:
    ```shell
    bin/conway
    ```

    Then follow the command line prompts (enter size, probability of cell being alive & generations). Note that currently only a square grid can be created.

    The simulation can be stopped before it's completed by terminating the program, closing the window, or pressing the Escape key.

- There are also a couple of older versions of the program, kept for comparison. No graphical component has been implemented for these yet.

## General Notes

- This implementation has currently only been tested on Linux (Ubuntu 18.04.1).

## Memory Leak Notes

When running the code as parallel code (compiled with -fopenmp & the openmp directives), valgrind reports bytes still reachable & possibly lost. This is not the case when running the serial version of the code and is likely an issue with valgrind / openmp. [See this article](https://medium.com/@auraham/pseudo-memory-leaks-when-using-openmp-11a383cc4cf9) for an investigation.
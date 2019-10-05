# Conway's Game of Life (C++)

Conway's Game of Life implemented in C++. Program uses [SFML](https://www.sfml-dev.org/index.php) for rendering the simulation.

## Build

From the project root run
```shell
make
```

**NOTE -** This program currently depends on [OpenMP](https://www.openmp.org/), so your compiler needs to support it. [See a list of supported compilers](https://www.openmp.org/resources/openmp-compilers-tools/). 
Additionally, you will also need [SFML](https://www.sfml-dev.org/index.php) installed on the system to compile - chances are, [it should be trivial if you're on Linux](https://www.sfml-dev.org/tutorials/2.5/start-linux.php).

## Usage
To run the main program:
```shell
bin/conway
```

Then follow the command line prompts (enter size, probability of cell being alive & generations).

The simulation can be stopped before it's completed by terminating the program, closing the window, or pressing the Escape key.

## Implementation Notes

The implementation is heavily based on one of [Michael Abrash's versions of the simulation](http://www.jagregory.com/abrash-black-book/#chapter-18-its-a-plain-wonderful-life) - see Listing 17.5 L17-5.CPP. However, amongst other things, rather than finding each cell's neighbours on each iteration, I just store them on each cell (since there is not really a space constraint as there is for the linked version).

Each cell therefore contains a list of its neighbours, as well as its state - which is stored as a byte. The byte is split up as follows:

    XXXN NNNS

`X` are unused bits; `N` are bits used for storing the number of the cell's live neighbours; `S` is the bit responsible for storing the cell's state (alive / dead).

<br/>

---

<br/>

## General Notes

- This implementation has currently only been tested on Linux (Ubuntu 18.04.1).
- There is a Visual Studio solution Windows currently [in a separate branch](https://github.com/pasdo501/conway-cpp/tree/windows).

## Memory Leak Notes

When running the code as parallel code (compiled with -fopenmp & the openmp directives), valgrind reports bytes still reachable & possibly lost. This is not the case when running the serial version of the code and is likely an issue with valgrind / openmp. [See this article](https://medium.com/@auraham/pseudo-memory-leaks-when-using-openmp-11a383cc4cf9) for an investigation.
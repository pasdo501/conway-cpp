# Conway's Game of Life (C++)

Conway's Game of Life implemented in C++ - multiple implementation variations. The main version uses [SFML](https://www.sfml-dev.org/index.php) for rendering the simulation.

## Usage
```cmd
Release/Conway-CPP.exe
```

## Build

Built in Microsoft VisualStudio 2019 CE.

**NOTE -** This program currently depends on [OpenMP](https://www.openmp.org/), so the compiler needs to support it. Additionally, it also depends on [SFML](https://www.sfml-dev.org/index.php) being installed on the system to compile. See below.

- To enable OpenMP: Project -> Properties -> C/C++ -> Language -> `OpenMP Support to Yes` (May also need to change C/C++ -> Command Line -> Additional Options `/ZctwoPhase-`)
- [Instructions for compiling SFML can be found here](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php).
- [Instructions for then using SFML with VS can be found here](https://www.sfml-dev.org/tutorials/2.5/start-vc.php).
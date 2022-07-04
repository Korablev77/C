## About

Program solves following task: given text document (consider ASCII encoding), calculate frequences for each
appearing word, and dump sorted table (firstly sort by frequency then in lexicographic order) to output file.
Also program should execute as fast as possible.

To measure perf there's baseline implementation written in python. It's simple and stupid:)
For file ~100mb python takes ~9000ms meanhwile this C++ implementation ~2000ms. Data set is taken from 
[here](http://mattmahoney.net/dc/textdata.html). Still there's some space for optimizations (see comments
in source file).

All code is amalgamated in main.cpp (for the simplicity sake).

Also due to the lack of time repo does not contain any test (but I've tested it, I swear).
Moreover, it would be nice to add some decent benchmarks using e.g. google bench.

## Build

Prerequisite: Unix-like system (supporting mmap system call), compiler supporting C++17 standard and cmake of version >= 3.5.
Tested on gcc version 7.4.0 / Ubuntu~18.04.1 (x86) and clang 12.0.5 / MacOS 11.4 (ARM M1).

```
cmake .
make -j
./FrequencyDict input_filename output_filename
```

To run baseline benchmark python3 is required:
```
python3 baseline.py input_filename output_filename
```

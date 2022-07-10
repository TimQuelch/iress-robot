# Toy robot

Code challenge to implement Toy Robot

Design decisions
- Use of optional types rather than exceptions 
- Easily extendable command list
- Extensive test suite

# Build

Requires CMake, git, make, and compiler (tested with clang and gcc. MSVC /should/ work but I haven't tested it)

``` sh
mkdir build
cd build
cmake ..
make
```

This will pull in dependencies (catch and fmt) and compile all targets

Tests with `make test` or `./tests`

Run with `./main`

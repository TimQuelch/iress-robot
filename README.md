# Toy robot

Code challenge to implement Toy Robot

Design decisions
- Use of optional types rather than exceptions 
- Easily extendable command list
- Extensive test suite

# Build

Requires CMake, git, make, and compiler (tested with clang and gcc. MSVC *should* work but I haven't tested it)

``` sh
mkdir build
cd build
cmake ..
make
```

This will pull in dependencies (catch and fmt) and compile all targets

Run with `./main`

Run all tests with `make test` or `./tests`

# Running

Commands are read through stdin, e.g.

``` sh
$ ./main
PLACE 1, 2, NORTH
REPORT
x = 1, y = 2, direction = NORTH
```

or

``` sh
$ echo -e "PLACE 1,2,NORTH\nMOVE\nLEFT\nREPORT" | ./main
x = 1, y = 3, direction = WEST
```

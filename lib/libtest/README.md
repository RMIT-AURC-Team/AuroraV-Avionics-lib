# libtest

`libtest`  provides a simple interface for defining and running tests, making it easy to implement a unit testing suite.

## Getting Started

### Prerequisites

To build and use `libtest`, you need:

- GCC (GNU Compiler Collection)
- Make

### Building the Library

To build the `libtest` library, from inside the `libtest` run:

```sh
make
```

This will compile the source files and generate the static library `libtest.a` in the `build` directory.

### Usage

To use `libtest` include the `libtest.h` header file and link against the `libtest.a` static library. Here is an example of how to integrate and use `libtest` in a test file:

```c
#include "libtest.h"

// Example test functions
bool test1() {
    return true; // Example test that passes
}

bool test2() {
    return false; // Example test that fails
}

bool test3() {
    return true; // Another example test that passes
}

int main() {
    TestFunction tests[] = {test1, test2, test3};
    int numTests = sizeof(tests) / sizeof(TestFunction);

    test(numTests, tests);

    return 0;
}
```

This will compile the test files and run the tests, printing a summary of the results.

### Cleaning Up

To clean up the compiled files, run:

```sh
make clean
```

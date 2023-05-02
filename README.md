# TCP utilities for C/C++ (CUT)
This repository contains a minimal implementation of a TCP server and client for C/C++.

# Usage
## C
To build the library run `make lib`. Then add:
```C
#include <tcp.h>
```
to your project and compile using the flag `-ltcp`. (Make sure `ld` can find `libtcp.a`.)

For examples of how to use, or for testing/debugging run `make test`.

## C++
To build the library run `make lib LANG=C++`. Then add:
```C++
#include <tcp.hpp>
```
to your project and compile using the flag `-ltcp++`. (Make sure `ld` can find `libtcp++.a`.)

For examples of how to use, or for testing/debugging run `make test LANG=C++`.
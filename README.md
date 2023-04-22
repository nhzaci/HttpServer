# Http Server

Project started to work with Linux system calls to get a working HTTP Server that can support a decent throughput.

## Goals

- Supports HTTP 1.1
- Supports up to 100k requests / second

## Building

### Requirements

- CMake v3.22
- C++23

### Running Build Script

```bash
# from project root

# give exec perms to build.sh
chmod u+x ./build.sh

# run build script
./build.sh
```

### Running Server

```bash
# from project root
./build/bin/RunServer

starting http server...
started http server, listening on port 8080.

# to quit, type "quit"
quit

# close fds and return resources
stopping http server...
successfully stopped.
```

### Running Tests

```bash
./build/bin/TestMain
```
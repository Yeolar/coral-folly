# Coral

Coral is a C++11 foundation and network library.

Coral is mainly based on Facebook folly library, but is restructured and customized with some extensions.

## Dependencies

Coral requires gcc 4.8+ / clang and a version of boost compiled with C++11 support.

Coral is built with CMake 2.8.8+, so you also need to install cmake.

### Debian 8

Coral is full tested on Debian 8.  Please install the dependencies below:

Required:

- libboost-all-dev (required 1.51.0+)
- libdouble-conversion-dev
- libgflags-dev
- libgoogle-glog-dev
- libevent-dev
- libssl-dev

Optional:

- libjemalloc-dev
- libicu-dev
- liblz4-dev
- liblzma-dev
- libonig-dev (prefer https://github.com/k-takata/Onigmo)
- libsnappy-dev
- zlib1g-dev

Test:

- libgtest-dev
- google-mock

Test is also optional, if not installed, test step is just skipped.  Installation of gtest and gmock requires some more steps, for example of gtest (gmock is similar):

```
# cd /usr/src/gtest/
# mkdir build && cd build
# cmake ..
# make
# cp libgtest* /usr/local/lib/
# cd .. && rm -r build
```

### Mac OS X

Coral is also compiled pass on Mac OS X.

On Mac, you can install the requirements with Homwbrew or MacPorts, or manually.

### Other Linux distributions

Coral should be compatible on other Linux.  For dependencies refer Debian.

## Installation

By cmake, it's simple:

```
$ mkdir build && cd build
$ cmake ..
$ make
# make install
```


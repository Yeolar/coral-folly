#
# Copyright (C) 2015, Yeolar
#

# Setup compiler
if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
else()
    set(CMAKE_C_COMPILER gcc-4.8)
    set(CMAKE_CXX_COMPILER g++-4.8)
endif()

# Define project
project(coral)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 55)
set(VERSION_MICRO 0)
set(VERSION_PATCH 0)

cmake_minimum_required(VERSION 2.6)

enable_testing()

# http://www.cmake.org/Wiki/CMake_RPATH_handling#Mac_OS_X_and_the_RPATH
if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
        "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    endif()
endif()

# Required packages
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

find_package(Boost 1.51.0 REQUIRED
    COMPONENTS context filesystem program_options regex system thread)
set(CORAL_HAVE_BOOST ${Boost_FOUND})
set(CORAL_HAVE_BOOST_CONTEXT ${Boost_CONTEXT_FOUND})
set(CORAL_HAVE_BOOST_FILESYSTEM ${Boost_FILESYSTEM_FOUND})
set(CORAL_HAVE_BOOST_PROGRAM_OPTIONS ${Boost_PROGRAM_OPTIONS_FOUND})
set(CORAL_HAVE_BOOST_REGEX ${Boost_REGEX_FOUND})
set(CORAL_HAVE_BOOST_SYSTEM ${Boost_SYSTEM_FOUND})
set(CORAL_HAVE_BOOST_THREAD ${Boost_THREAD_FOUND})
if(APPLE)
    set(CORAL_LINK_BOOST_LIBS
        boost_filesystem-mt boost_regex-mt boost_system-mt boost_thread-mt)
else()
    set(CORAL_LINK_BOOST_LIBS
        boost_filesystem boost_regex boost_system boost_thread)
endif()

find_package(double-conversion REQUIRED)
set(CORAL_HAVE_LIBDOUBLE_CONVERSION ${double-conversion_FOUND})
find_package(GFlags REQUIRED)
set(CORAL_HAVE_LIBGFLAGS ${GFLAGS_FOUND})
find_package(Glog REQUIRED)
set(CORAL_HAVE_LIBGLOG ${GLOG_FOUND})

# Optional packages
find_package(GTest)
set(CORAL_HAVE_LIBGTEST ${GTEST_FOUND})
find_package(JeMalloc)
set(CORAL_HAVE_LIBJEMALLOC ${JEMALLOC_FOUND})
find_package(LibEvent)
set(CORAL_HAVE_LIBEVENT ${LibEvent_FOUND})
find_package(LZ4)
set(CORAL_HAVE_LIBLZ4 ${LZ4_FOUND})
find_package(LibLZMA)
set(CORAL_HAVE_LIBLZMA ${LIBLZMA_FOUND})
find_package(Snappy)
set(CORAL_HAVE_LIBSNAPPY ${SNAPPY_FOUND})
find_package(OpenSSL)
set(CORAL_HAVE_LIBSSL ${OPENSSL_FOUND})
find_package(ZLIB)
set(CORAL_HAVE_LIBZ ${ZLIB_FOUND})

# Option
option(BUILD_SHARED_LIBS "build as shared libs" ON)

# Setup environment
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE OFF)

add_definitions(
    -std=c++11
)

# Check and setup configurations
include(CheckCXXSourceCompiles)
include(CheckFunctionExists)
include(CheckIncludeFileCXX)
include(CheckLibraryExists)
include(CheckTypeSize)

check_include_file_cxx(byteswap.h CORAL_HAVE_BYTESWAP_H)
check_include_file_cxx(dlfcn.h CORAL_HAVE_DLFCN_H)
check_include_file_cxx(emmintrin.h CORAL_HAVE_EMMINTRIN_H)
check_include_file_cxx(fcntl.h CORAL_HAVE_FCNTL_H)
check_include_file_cxx(features.h CORAL_HAVE_FEATURES_H)
check_include_file_cxx(inttypes.h CORAL_HAVE_INTTYPES_H)
check_include_file_cxx(limits.h CORAL_HAVE_LIMITS_H)
check_include_file_cxx(malloc.h CORAL_HAVE_MALLOC_H)
check_include_file_cxx(memory.h CORAL_HAVE_MEMORY_H)
check_include_file_cxx(mutex.h CORAL_HAVE_MUTEX_H)
check_include_file_cxx(stdbool.h CORAL_HAVE_STDBOOL_H)
check_include_file_cxx(stdint.h CORAL_HAVE_STDINT_H)
check_include_file_cxx(stdlib.h CORAL_HAVE_STDLIB_H)
check_include_file_cxx(string.h CORAL_HAVE_STRING_H)
check_include_file_cxx(strings.h CORAL_HAVE_STRINGS_H)
check_include_file_cxx(unistd.h CORAL_HAVE_UNISTD_H)
check_include_file_cxx(bits/c++config.h CORAL_HAVE_BITS_C__CONFIG_H)
check_include_file_cxx(bits/functexcept.h CORAL_HAVE_BITS_FUNCTEXCEPT_H)
check_include_file_cxx(sys/stat.h CORAL_HAVE_SYS_STAT_H)
check_include_file_cxx(sys/time.h CORAL_HAVE_SYS_TIME_H)
check_include_file_cxx(sys/types.h CORAL_HAVE_SYS_TYPES_H)

check_type_size(__int128 INT128_T)
set(CORAL_HAVE_INT128_T ${HAVE_INT128_T})
check_type_size(pthread_spinlock_t PTHREAD_SPINLOCK_T)
set(CORAL_HAVE_PTHREAD_SPINLOCK_T ${HAVE_PTHREAD_SPINLOCK_T})
check_type_size(ptrdiff_t PTRDIFF_T)
set(CORAL_HAVE_PTRDIFF_T ${HAVE_PTRDIFF_T})

check_function_exists(getdelim CORAL_HAVE_GETDELIM)
check_function_exists(gettimeofday CORAL_HAVE_GETTIMEOFDAY)
check_function_exists(malloc_size CORAL_HAVE_MALLOC_SIZE)
check_function_exists(malloc_usable_size CORAL_HAVE_MALLOC_USABLE_SIZE)
check_function_exists(memmove CORAL_HAVE_MEMMOVE)
check_function_exists(memrchr CORAL_HAVE_MEMRCHR)
check_function_exists(memset CORAL_HAVE_MEMSET)
check_function_exists(pipe2 CORAL_HAVE_PIPE2)
check_function_exists(strerror CORAL_HAVE_STRERROR)

check_library_exists(iberty
    cplus_demangle_v3_callback /usr/lib CORAL_HAVE_CPLUS_DEMANGLE_V3_CALLBACK)
check_library_exists(m
    pow /usr/lib CORAL_HAVE_POW)
check_library_exists(pthread
    pthread_atfork /usr/lib CORAL_HAVE_PTHREAD_ATFORK)
check_library_exists(pthread
    pthread_yield /usr/lib CORAL_HAVE_PTHREAD_YIELD)
check_library_exists(rt
    clock_gettime /usr/lib CORAL_HAVE_CLOCK_GETTIME)

if(NOT CORAL_HAVE_PTHREAD_YIELD)
    check_include_file_cxx(sched.h CORAL_HAVE_SCHED_H)
    check_function_exists(sched_yield CORAL_HAVE_SCHED_YIELD)
endif()

set(CMAKE_REQUIRED_FLAGS "-std=c++11")

check_cxx_source_compiles("
    class C {
        virtual void f() final {}
        virtual void g() {}
    };
    class D : public C {
        virtual void g() override {}
    };
    int main() { return 0; }"
    CORAL_FINAL)
set(CORAL_OVERRIDE ${CORAL_FINAL})

check_cxx_source_compiles("
    #include <type_traits>
    #if !_LIBCPP_VERSION
    #error No libc++
    #endif
    int main() { return 0; }"
    CORAL_USE_LIBCPP)

check_cxx_source_compiles("
    #include <cstring>
    static constexpr int val = strlen(\"foo\");
    int main() { return 0; }"
    CORAL_HAVE_CONSTEXPR_STRLEN)

check_cxx_source_compiles("
    #pragma GCC diagnostic error \"-Wattributes\"
    extern \"C\" void (*test_ifunc(void))() { return 0; }
    void func() __attribute__((ifunc(\"test_ifunc\")));
    int main() { return 0; }"
    CORAL_HAVE_IFUNC)

check_cxx_source_compiles("
    #include <type_traits>
    const bool val = std::is_trivially_copyable<bool>::value;
    int main() { return 0; }"
    CORAL_HAVE_STD__IS_TRIVIALLY_COPYABLE)

check_cxx_source_compiles("
    #include <thread>
    #include <chrono>
    void func() { std::this_thread::sleep_for(std::chrono::seconds(1)); }
    int main() { return 0; }"
    CORAL_HAVE_STD__THIS_THREAD__SLEEP_FOR)

check_cxx_source_compiles("
    #include <stdio.h>
    int main() {
        char buf[256];
        return vsnprintf(buf, sizeof(buf), \"%\", 1) < 0 ? 0 : 1;
    }"
    CORAL_HAVE_VSNPRINTF_ERRORS)

check_cxx_source_compiles("
    extern \"C\" void configure_link_extern_weak_test() __attribute__((weak));
    int main() {
        return configure_link_extern_weak_test == nullptr;
    }"
    CORAL_HAVE_WEAK_SYMBOLS)

check_cxx_source_compiles("
    #include <string.h>
    #include <errno.h>
    int main() {
        char buf[1024];
        buf[0] = 0;
        int ret = strerror_r(ENOMEM, buf, sizeof(buf));
        return ret;
    }"
    CORAL_HAVE_XSI_STRERROR_R)

if(GFLAGS_FOUND)
    set(CMAKE_REQUIRED_LIBRARIES "gflags")

    check_cxx_source_compiles("
        #include <gflags/gflags.h>
        void foo() { google::GetArgv(); }
        int main() { return 0; }"
        CORAL_UNUSUAL_GFLAGS_NAMESPACE)

    if(CORAL_UNUSUAL_GFLAGS_NAMESPACE)
        set(CORAL_GFLAGS_NAMESPACE "google")
    else()
        set(CORAL_GFLAGS_NAMESPACE "gflags")
    endif()
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/coral/coral-config.h.in"
    "${PROJECT_SOURCE_DIR}/coral/coral-config.h"
)

include_directories(${PROJECT_SOURCE_DIR})

# Work on all sub directories
add_subdirectory(coral)


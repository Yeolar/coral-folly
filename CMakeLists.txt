#
# Copyright (C) 2015, Yeolar
#

# Setup compiler
if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
else()
    # required GCC >= 4.8
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

# Define project
project(folly)

set(VERSION_MAJOR 55)
set(VERSION_MINOR 0)
set(VERSION_MICRO 0)
set(VERSION_PATCH 0)

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts")

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
set(FOLLY_HAVE_BOOST ${Boost_FOUND})
set(FOLLY_HAVE_BOOST_CONTEXT ${Boost_CONTEXT_FOUND})
set(FOLLY_HAVE_BOOST_FILESYSTEM ${Boost_FILESYSTEM_FOUND})
set(FOLLY_HAVE_BOOST_PROGRAM_OPTIONS ${Boost_PROGRAM_OPTIONS_FOUND})
set(FOLLY_HAVE_BOOST_REGEX ${Boost_REGEX_FOUND})
set(FOLLY_HAVE_BOOST_SYSTEM ${Boost_SYSTEM_FOUND})
set(FOLLY_HAVE_BOOST_THREAD ${Boost_THREAD_FOUND})

find_package(DoubleConversion REQUIRED)
set(FOLLY_HAVE_LIBDOUBLE_CONVERSION ${DOUBLE_CONVERSION_FOUND})
find_package(GFlags REQUIRED)
set(FOLLY_HAVE_LIBGFLAGS ${GFLAGS_FOUND})
find_package(Glog REQUIRED)
set(FOLLY_HAVE_LIBGLOG ${GLOG_FOUND})
find_package(AIO REQUIRED)
set(FOLLY_HAVE_LIBAIO ${AIO_FOUND})
find_package(DWARF REQUIRED)
set(FOLLY_HAVE_LIBDWARF ${DWARF_FOUND})
find_package(Unwind REQUIRED)
set(FOLLY_HAVE_LIBUNWIND ${UNWIND_FOUND})
find_package(LZ4 REQUIRED)
set(FOLLY_HAVE_LIBLZ4 ${LZ4_FOUND})
find_package(LibLZMA REQUIRED)
set(FOLLY_HAVE_LIBLZMA ${LIBLZMA_FOUND})
find_package(Snappy REQUIRED)
set(FOLLY_HAVE_LIBSNAPPY ${SNAPPY_FOUND})
find_package(ZLIB REQUIRED)
set(FOLLY_HAVE_LIBZ ${ZLIB_FOUND})
find_package(LibEvent REQUIRED)
set(FOLLY_HAVE_LIBEVENT ${LibEvent_FOUND})
find_package(OpenSSL REQUIRED)
set(FOLLY_HAVE_LIBSSL ${OPENSSL_FOUND})

# Optional packages
find_package(GTest)
set(FOLLY_HAVE_LIBGTEST ${GTEST_FOUND})
find_package(GMock)
set(FOLLY_HAVE_LIBGMOCK ${GMOCK_FOUND})
find_package(JeMalloc)
set(FOLLY_HAVE_LIBJEMALLOC ${JEMALLOC_FOUND})
find_package(ICU)
set(FOLLY_HAVE_LIBICU ${ICU_FOUND})
find_package(ONIGURUMA)
set(FOLLY_HAVE_LIBONIG ${ONIGURUMA_FOUND})

# Options
include(CMakeOptions.cmake)

# Setup environment
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

# Link libraries
link_libraries(
    ${Boost_LIBRARIES}
    ${DOUBLE_CONVERSION_LIBRARY}
    ${GFLAGS_LIBRARIES}
    ${GLOG_LIBRARIES}
    ${UNWIND_LIBRARY}
    ${AIO_LIBRARIES}
    ${LZ4_LIBRARY}
    ${LIBLZMA_LIBRARIES}
    ${SNAPPY_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${LIBEVENT_LIB}
    ${OPENSSL_LIBRARIES}
    dl
)
#if(FOLLY_HAVE_LIBGLOG AND FOLLY_USE_GLOG)
#    link_libraries(${GLOG_LIBRARIES})
#endif()
if(FOLLY_HAVE_LIBICU)
    if(APPLE)
        link_libraries(icucore)
    else()
        link_libraries(${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES})
    endif()
endif()
if(FOLLY_HAVE_LIBONIG)
    link_libraries(${ONIGURUMA_LIBRARIES})
endif()
if(APPLE)
    find_library(MAC_FRAMEWORKS NAMES CoreFoundation)
    link_libraries(${MAC_FRAMEWORKS})
endif()

# Check and setup configurations
include(CheckCXXSourceCompiles REQUIRED)
include(CheckFunctionExists REQUIRED)
include(CheckIncludeFileCXX REQUIRED)
include(CheckLibraryExists REQUIRED)
include(CheckTypeSize REQUIRED)

check_include_file_cxx(byteswap.h FOLLY_HAVE_BYTESWAP_H)
check_include_file_cxx(dlfcn.h FOLLY_HAVE_DLFCN_H)
check_include_file_cxx(emmintrin.h FOLLY_HAVE_EMMINTRIN_H)
check_include_file_cxx(fcntl.h FOLLY_HAVE_FCNTL_H)
check_include_file_cxx(features.h FOLLY_HAVE_FEATURES_H)
check_include_file_cxx(inttypes.h FOLLY_HAVE_INTTYPES_H)
check_include_file_cxx(limits.h FOLLY_HAVE_LIMITS_H)
check_include_file_cxx(malloc.h FOLLY_HAVE_MALLOC_H)
check_include_file_cxx(memory.h FOLLY_HAVE_MEMORY_H)
check_include_file_cxx(mutex.h FOLLY_HAVE_MUTEX_H)
check_include_file_cxx(stdbool.h FOLLY_HAVE_STDBOOL_H)
check_include_file_cxx(stdint.h FOLLY_HAVE_STDINT_H)
check_include_file_cxx(stdlib.h FOLLY_HAVE_STDLIB_H)
check_include_file_cxx(string.h FOLLY_HAVE_STRING_H)
check_include_file_cxx(strings.h FOLLY_HAVE_STRINGS_H)
check_include_file_cxx(unistd.h FOLLY_HAVE_UNISTD_H)
check_include_file_cxx(bits/c++config.h FOLLY_HAVE_BITS_C__CONFIG_H)
check_include_file_cxx(bits/functexcept.h FOLLY_HAVE_BITS_FUNCTEXCEPT_H)
check_include_file_cxx(sys/stat.h FOLLY_HAVE_SYS_STAT_H)
check_include_file_cxx(sys/time.h FOLLY_HAVE_SYS_TIME_H)
check_include_file_cxx(sys/types.h FOLLY_HAVE_SYS_TYPES_H)

check_type_size(__int128 INT128_T)
set(FOLLY_HAVE_INT128_T ${HAVE_INT128_T})
check_type_size(pthread_spinlock_t PTHREAD_SPINLOCK_T)
set(FOLLY_HAVE_PTHREAD_SPINLOCK_T ${HAVE_PTHREAD_SPINLOCK_T})
check_type_size(ptrdiff_t PTRDIFF_T)
set(FOLLY_HAVE_PTRDIFF_T ${HAVE_PTRDIFF_T})

check_function_exists(getdelim FOLLY_HAVE_GETDELIM)
check_function_exists(gettimeofday FOLLY_HAVE_GETTIMEOFDAY)
check_function_exists(malloc_size FOLLY_HAVE_MALLOC_SIZE)
check_function_exists(malloc_usable_size FOLLY_HAVE_MALLOC_USABLE_SIZE)
check_function_exists(memmove FOLLY_HAVE_MEMMOVE)
check_function_exists(memrchr FOLLY_HAVE_MEMRCHR)
check_function_exists(memset FOLLY_HAVE_MEMSET)
check_function_exists(pipe2 FOLLY_HAVE_PIPE2)
check_function_exists(strerror FOLLY_HAVE_STRERROR)

check_library_exists(iberty
    cplus_demangle_v3_callback /usr/lib FOLLY_HAVE_CPLUS_DEMANGLE_V3_CALLBACK)
check_library_exists(m
    pow /usr/lib FOLLY_HAVE_POW)
check_library_exists(pthread
    pthread_atfork /usr/lib FOLLY_HAVE_PTHREAD_ATFORK)
check_library_exists(pthread
    pthread_yield /usr/lib FOLLY_HAVE_PTHREAD_YIELD)
check_library_exists(rt
    clock_gettime /usr/lib FOLLY_HAVE_CLOCK_GETTIME)

if(FOLLY_HAVE_CPLUS_DEMANGLE_V3_CALLBACK)
    link_libraries(iberty)
endif()

if(NOT FOLLY_HAVE_PTHREAD_YIELD)
    check_include_file_cxx(sched.h FOLLY_HAVE_SCHED_H)
    check_function_exists(sched_yield FOLLY_HAVE_SCHED_YIELD)
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
    FOLLY_FINAL)
set(FOLLY_OVERRIDE ${FOLLY_FINAL})

check_cxx_source_compiles("
    #include <type_traits>
    #if !_LIBCPP_VERSION
    #error No libc++
    #endif
    int main() { return 0; }"
    FOLLY_USE_LIBCPP)

check_cxx_source_compiles("
    #include <cstring>
    static constexpr int val = strlen(\"foo\");
    int main() { return 0; }"
    FOLLY_HAVE_CONSTEXPR_STRLEN)

check_cxx_source_compiles("
    #pragma GCC diagnostic error \"-Wattributes\"
    extern \"C\" void (*test_ifunc(void))() { return 0; }
    void func() __attribute__((ifunc(\"test_ifunc\")));
    int main() { return 0; }"
    FOLLY_HAVE_IFUNC)

check_cxx_source_compiles("
    #include <type_traits>
    const bool val = std::is_trivially_copyable<bool>::value;
    int main() { return 0; }"
    FOLLY_HAVE_STD__IS_TRIVIALLY_COPYABLE)

check_cxx_source_compiles("
    #include <thread>
    #include <chrono>
    void func() { std::this_thread::sleep_for(std::chrono::seconds(1)); }
    int main() { return 0; }"
    FOLLY_HAVE_STD__THIS_THREAD__SLEEP_FOR)

check_cxx_source_compiles("
    #include <stdio.h>
    int main() {
        char buf[256];
        return vsnprintf(buf, sizeof(buf), \"%\", 1) < 0 ? 0 : 1;
    }"
    FOLLY_HAVE_VSNPRINTF_ERRORS)

check_cxx_source_compiles("
    extern \"C\" void configure_link_extern_weak_test() __attribute__((weak));
    int main() {
        return configure_link_extern_weak_test == nullptr;
    }"
    FOLLY_HAVE_WEAK_SYMBOLS)

check_cxx_source_compiles("
    #include <string.h>
    #include <errno.h>
    int main() {
        char buf[1024];
        buf[0] = 0;
        int ret = strerror_r(ENOMEM, buf, sizeof(buf));
        return ret;
    }"
    FOLLY_HAVE_XSI_STRERROR_R)

if(GFLAGS_FOUND)
    set(CMAKE_REQUIRED_LIBRARIES "gflags")

    check_cxx_source_compiles("
        #include <gflags/gflags.h>
        void foo() { google::GetArgv(); }
        int main() { return 0; }"
        FOLLY_UNUSUAL_GFLAGS_NAMESPACE)

    if(FOLLY_UNUSUAL_GFLAGS_NAMESPACE)
        set(FOLLY_GFLAGS_NAMESPACE "google")
    else()
        set(FOLLY_GFLAGS_NAMESPACE "gflags")
    endif()
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/folly/folly-config.h.in"
    "${PROJECT_SOURCE_DIR}/folly/folly-config.h"
)

# Uninstall
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake-scripts/cmake_uninstall.cmake.in"
    "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND}
        -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

include_directories(${PROJECT_SOURCE_DIR})

# Work on all sub directories
add_subdirectory(folly)
add_subdirectory(folly/experimental)
add_subdirectory(folly/experimental/exception_tracer)
add_subdirectory(folly/experimental/fibers)
add_subdirectory(folly/experimental/io)
add_subdirectory(folly/experimental/symbolizer)
add_subdirectory(folly/ext)
add_subdirectory(folly/ext/io)
add_subdirectory(folly/ext/platform)
add_subdirectory(folly/futures)
add_subdirectory(folly/io)
add_subdirectory(folly/io/async)
add_subdirectory(folly/stats)

# Link libraries
add_library(folly_static STATIC
    $<TARGET_OBJECTS:folly_base>
    $<TARGET_OBJECTS:folly_experimental>
    $<TARGET_OBJECTS:folly_experimental_exception_tracer>
    $<TARGET_OBJECTS:folly_experimental_fibers>
    $<TARGET_OBJECTS:folly_experimental_io>
    $<TARGET_OBJECTS:folly_experimental_symbolizer>
    $<TARGET_OBJECTS:folly_ext>
    $<TARGET_OBJECTS:folly_ext_io>
    $<TARGET_OBJECTS:folly_futures>
    $<TARGET_OBJECTS:folly_io>
    $<TARGET_OBJECTS:folly_io_async>
    $<TARGET_OBJECTS:folly_stats>
)
set_target_properties(folly_static PROPERTIES OUTPUT_NAME folly)
add_library(folly_shared SHARED
    $<TARGET_OBJECTS:folly_base>
    $<TARGET_OBJECTS:folly_experimental>
    $<TARGET_OBJECTS:folly_experimental_exception_tracer>
    $<TARGET_OBJECTS:folly_experimental_fibers>
    $<TARGET_OBJECTS:folly_experimental_io>
    $<TARGET_OBJECTS:folly_experimental_symbolizer>
    $<TARGET_OBJECTS:folly_ext>
    $<TARGET_OBJECTS:folly_ext_io>
    $<TARGET_OBJECTS:folly_futures>
    $<TARGET_OBJECTS:folly_io>
    $<TARGET_OBJECTS:folly_io_async>
    $<TARGET_OBJECTS:folly_stats>
)
set_target_properties(folly_shared PROPERTIES OUTPUT_NAME folly)

install(TARGETS folly_static
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE
                GROUP_READ
                WORLD_READ
)
install(TARGETS folly_shared
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)

# Test
if(GTEST_FOUND AND GMOCK_FOUND)
    enable_testing()
    add_subdirectory(folly/test)
    add_subdirectory(folly/experimental/test)
    add_subdirectory(folly/experimental/io/test)
    add_subdirectory(folly/experimental/symbolizer/test)
    add_subdirectory(folly/futures/test)
    add_subdirectory(folly/gen/test)
    add_subdirectory(folly/io/test)
    add_subdirectory(folly/io/async/test)
endif()


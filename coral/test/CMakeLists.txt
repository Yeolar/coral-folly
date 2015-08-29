#
# Copyright (C) 2015, Yeolar
#

set(CORAL_BASE_TEST_SRCS
    ApplyTupleTest.cpp
    AsciiCaseInsensitiveTest.cpp
    BenchmarkTest.cpp
    ConvTest.cpp
    CpuIdTest.cpp
    DemangleTest.cpp
    DynamicConverterTest.cpp
    #DynamicOtherTest.cpp
    DynamicTest.cpp
    ExceptionTest.cpp
    FBStringTest.cpp
    FBVectorTest.cpp
    #FileTest.cpp
    #FileUtilTest.cpp  # no typeof under -std
    ForeachTest.cpp
    #FormatOtherTest.cpp
    FormatTest.cpp
    HashTest.cpp
    JsonOtherTest.cpp
    JsonTest.cpp
    MapUtilTest.cpp
    #MemoryTest.cpp
    OptionalTest.cpp
    PortabilityTest.cpp
    RandomTest.cpp
    RangeTest.cpp
    RWSpinLockTest.cpp
    ScopeGuardTest.cpp
    SpinLockTest.cpp
    SpookyHashV1Test.cpp
    SpookyHashV2Test.cpp
    StringTest.cpp
    SynchronizedTest.cpp
    ThreadLocalTest.cpp
    TraitsTest.cpp
)

foreach(test_src ${CORAL_BASE_TEST_SRCS})
    get_filename_component(test ${test_src} NAME_WE)
    add_executable(${test} ${test_src})
    target_link_libraries(${test} gtest gtest_main gmock coral)
    add_test(coral_${test} ${test} CONFIGURATIONS Debug)
endforeach()

execute_process(
    COMMAND ln -fs "${PROJECT_SOURCE_DIR}/coral/test/json_test_data"
                   "${PROJECT_BINARY_DIR}/coral/test/json_test_data"
)


set(CORAL_BASE_BENCHMARK_SRCS
    AsciiCaseInsensitiveBenchmark.cpp
    FBStringBenchmark.cpp
    FBVectorBenchmark.cpp
    FormatBenchmark.cpp
    RangeFindBenchmark.cpp
    StringBenchmark.cpp
)

foreach(bench_src ${CORAL_BASE_BENCHMARK_SRCS})
    get_filename_component(bench ${bench_src} NAME_WE)
    add_executable(${bench} ${bench_src})
    target_link_libraries(${bench} gtest gtest_main coral)
endforeach()


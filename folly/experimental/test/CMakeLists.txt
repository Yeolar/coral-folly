#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_TEST_LIB_SRCS
    NestedCommandLineAppTestHelper.cpp
    ProgramOptionsTestHelper.cpp
)
add_library(folly_experimental_test STATIC ${FOLLY_EXPERIMENTAL_TEST_LIB_SRCS})
target_link_libraries(folly_experimental_test folly_static)


set(FOLLY_EXPERIMENTAL_TEST_SRCS
    AutoTimerTest.cpp
    BitsTest.cpp
    BitVectorCodingTest.cpp
    EliasFanoCodingTest.cpp
    EventCountTest.cpp
    FunctionSchedulerTest.cpp
    FutureDAGTest.cpp
    JSONSchemaTest.cpp
    LockFreeRingBufferTest.cpp
    NestedCommandLineAppTest.cpp
    ProgramOptionsTest.cpp
    StringKeyedTest.cpp
    TestUtilTest.cpp
    TupleOpsTest.cpp
)

foreach(test_src ${FOLLY_EXPERIMENTAL_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_experimental_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES}
        folly_static folly_base_test folly_experimental_test)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


set(FOLLY_EXPERIMENTAL_BENCHMARK_SRCS
    BitsBenchmark.cpp
    StringKeyedBenchmark.cpp
)

foreach(bench_src ${FOLLY_EXPERIMENTAL_BENCHMARK_SRCS})
    get_filename_component(bench_name ${bench_src} NAME_WE)
    set(bench folly_experimental_${bench_name})
    add_executable(${bench} ${bench_src})
    target_link_libraries(${bench} ${GTEST_BOTH_LIBRARIES} folly_static)
endforeach()


add_executable(NestedCommandLineAppExample NestedCommandLineAppExample.cpp)
target_link_libraries(NestedCommandLineAppExample folly_static)


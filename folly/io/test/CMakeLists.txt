#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_IO_TEST_SRCS
    CompressionTest.cpp
    IOBufCursorTest.cpp
    IOBufQueueTest.cpp
    IOBufTest.cpp
    RecordIOTest.cpp
    ShutdownSocketSetTest.cpp
)

foreach(test_src ${FOLLY_IO_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_io_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} folly_static)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


set(FOLLY_IO_BENCHMARK_SRCS
    NetworkBenchmark.cpp
)

foreach(bench_src ${FOLLY_IO_BENCHMARK_SRCS})
    get_filename_component(bench_name ${bench_src} NAME_WE)
    set(bench folly_io_${bench_name})
    add_executable(${bench} ${bench_src})
    target_link_libraries(${bench} ${GTEST_BOTH_LIBRARIES} folly_static)
endforeach()


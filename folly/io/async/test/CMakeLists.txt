#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_IO_ASYNC_TEST_LIB_SRCS
    SocketPair.cpp
    TimeUtil.cpp
)
add_library(folly_io_async_test STATIC ${FOLLY_IO_ASYNC_TEST_LIB_SRCS})
target_link_libraries(folly_io_async_test folly_static)


set(FOLLY_IO_ASYNC_TEST_SRCS
    AsyncSocketTest.cpp
    #AsyncSocketTest2.cpp
    #AsyncSSLSocketTest.cpp
    #AsyncSSLSocketTest2.cpp
    AsyncSSLSocketWriteTest.cpp
    AsyncTimeoutTest.cpp
    AsyncUDPSocketTest.cpp
    DelayedDestructionBaseTest.cpp
    EventBaseLocalTest.cpp
    EventBaseTest.cpp
    EventHandlerTest.cpp
    HHWheelTimerTest.cpp
    NotificationQueueTest.cpp
    RequestContextTest.cpp
    ScopedEventBaseThreadTest.cpp
)

foreach(test_src ${FOLLY_IO_ASYNC_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_io_async_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES}
        folly_static folly_io_async_test)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


set(FOLLY_IO_ASYNC_BENCHMARK_SRCS
    EventBaseBenchmark.cpp
)

foreach(bench_src ${FOLLY_IO_ASYNC_BENCHMARK_SRCS})
    get_filename_component(bench_name ${bench_src} NAME_WE)
    set(bench folly_io_async_${bench_name})
    add_executable(${bench} ${bench_src})
    target_link_libraries(${bench} ${GTEST_BOTH_LIBRARIES} folly_static)
endforeach()


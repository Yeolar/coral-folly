#
# Copyright (C) 2015, Yeolar
#

set(CORAL_IO_ASYNC_TEST_LIB_SRCS
    SocketPair.cpp
    TimeUtil.cpp
)
add_library(coral_io_async_test STATIC ${CORAL_IO_ASYNC_TEST_LIB_SRCS})
target_link_libraries(coral_io_async_test coral_static)


set(CORAL_IO_ASYNC_TEST_SRCS
    AsyncSocketTest.cpp
    #AsyncSocketTest2.cpp
    #AsyncSSLSocketTest.cpp
    #AsyncSSLSocketTest2.cpp
    AsyncSSLSocketWriteTest.cpp
    AsyncTimeoutTest.cpp
    AsyncUDPSocketTest.cpp
    DelayedDestructionBaseTest.cpp
    EventBaseBenchmark.cpp
    EventBaseTest.cpp
    EventHandlerTest.cpp
    HHWheelTimerTest.cpp
    NotificationQueueTest.cpp
    RequestContextTest.cpp
    ScopedEventBaseThreadTest.cpp
)

foreach(test_src ${CORAL_IO_ASYNC_TEST_SRCS})
    get_filename_component(test ${test_src} NAME_WE)
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        gtest gtest_main gmock coral_static coral_io_async_test)
    add_test(coral_${test} ${test} CONFIGURATIONS Debug)
endforeach()


#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_FUTURES_TEST_SRCS
    BarrierTest.cpp
    CollectTest.cpp
    ContextTest.cpp
    CoreTest.cpp
    EnsureTest.cpp
    ExecutorTest.cpp
    FilterTest.cpp
    FSMTest.cpp
    FutureTest.cpp
    HeaderCompileTest.cpp
    InterruptTest.cpp
    MapTest.cpp
    PollTest.cpp
    PromiseTest.cpp
    ReduceTest.cpp
    RetryingTest.cpp
    SharedPromiseTest.cpp
    ThenTest.cpp
    ThreadedExecutorTest.cpp
    TimekeeperTest.cpp
    TimesTest.cpp
    TryTest.cpp
    UnitTest.cpp
    UnwrapTest.cpp
    ViaTest.cpp
    WaitTest.cpp
    WhenTest.cpp
    WhileDoTest.cpp
    WillEqualTest.cpp
    WindowTest.cpp
)

foreach(test_src ${FOLLY_FUTURES_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_futures_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} folly_static)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


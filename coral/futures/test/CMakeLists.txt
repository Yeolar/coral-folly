#
# Copyright (C) 2015, Yeolar
#

set(CORAL_FUTURES_TEST_SRCS
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

foreach(test_src ${CORAL_FUTURES_TEST_SRCS})
    get_filename_component(test ${test_src} NAME_WE)
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} coral_static)
    add_test(coral_${test} ${test} CONFIGURATIONS Debug)
endforeach()


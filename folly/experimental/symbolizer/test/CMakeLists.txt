#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_SYMBOLIZER_TEST_LIB_SRCS
    Crash.cpp
)
add_library(folly_experimental_symbolizer_test STATIC
    ${FOLLY_EXPERIMENTAL_SYMBOLIZER_TEST_LIB_SRCS})
target_link_libraries(folly_experimental_symbolizer_test folly_static)


set(FOLLY_EXPERIMENTAL_SYMBOLIZER_TEST_SRCS
    DwarfTests.cpp
    ElfTests.cpp
    LineReaderTest.cpp
    SignalHandlerTest.cpp
    StackTraceTest.cpp
    SymbolizerTest.cpp
)

foreach(test_src ${FOLLY_EXPERIMENTAL_SYMBOLIZER_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_experimental_symbolizer_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES}
        folly_static folly_experimental_symbolizer_test)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


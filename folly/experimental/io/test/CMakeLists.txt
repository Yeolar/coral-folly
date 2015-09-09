#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_IO_TEST_SRCS
    AsyncIOTest.cpp
    FsUtilTest.cpp
)

foreach(test_src ${FOLLY_EXPERIMENTAL_IO_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_experimental_io_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} folly_static)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


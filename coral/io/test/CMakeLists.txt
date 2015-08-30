#
# Copyright (C) 2015, Yeolar
#

set(CORAL_IO_TEST_SRCS
    FsUtilTest.cpp
)

foreach(test_src ${CORAL_IO_TEST_SRCS})
    get_filename_component(test ${test_src} NAME_WE)
    add_executable(${test} ${test_src})
    target_link_libraries(${test} gtest gtest_main gmock coral)
    add_test(coral_${test} ${test} CONFIGURATIONS Debug)
endforeach()


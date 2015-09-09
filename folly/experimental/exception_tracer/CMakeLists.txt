#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_EXCEPTION_TRACER_SRCS
    ExceptionTracer.cpp
    ExceptionTracerLib.cpp
    StackTrace.cpp
)
add_library(folly_experimental_exception_tracer OBJECT
    ${FOLLY_EXPERIMENTAL_EXCEPTION_TRACER_SRCS})


install(FILES
    ExceptionAbi.h
    ExceptionTracer.h
    StackTrace.h
    DESTINATION include/folly/experimental/exception_tracer
)


set(FOLLY_EXPERIMENTAL_EXCEPTION_TRACER_TEST_SRCS
    ExceptionTracerTest.cpp
)

foreach(test_src ${FOLLY_EXPERIMENTAL_EXCEPTION_TRACER_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_experimental_exception_tracer_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} folly_static)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


set(FOLLY_EXPERIMENTAL_EXCEPTION_TRACER_BENCHMARK_SRCS
    ExceptionTracerBenchmark.cpp
)

foreach(bench_src ${FOLLY_EXPERIMENTAL_EXCEPTION_TRACER_BENCHMARK_SRCS})
    get_filename_component(bench_name ${bench_src} NAME_WE)
    set(bench folly_experimental_exception_tracer_${bench_name})
    add_executable(${bench} ${bench_src})
    target_link_libraries(${bench} ${GTEST_BOTH_LIBRARIES} folly_static)
endforeach()


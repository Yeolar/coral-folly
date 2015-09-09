#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_BASE_TEST_LIB_SRCS
    DeterministicSchedule.cpp
    FileTestLockHelper.cpp
    SubprocessTestParentDeathHelper.cpp
)
add_library(folly_base_test STATIC ${FOLLY_BASE_TEST_LIB_SRCS})
target_link_libraries(folly_base_test folly_static)


set(FOLLY_BASE_TEST_SRCS
    AHMIntStressTest.cpp
    ApplyTupleTest.cpp
    ArenaSmartPtrTest.cpp
    ArenaTest.cpp
    AsciiCaseInsensitiveTest.cpp
    AtomicBitSetTest.cpp
    AtomicHashArrayTest.cpp
    AtomicHashMapTest.cpp
    AtomicStructTest.cpp
    AtomicUnorderedMapTest.cpp
    BatonTest.cpp
    BenchmarkTest.cpp
    BitIteratorTest.cpp
    BitsTest.cpp
    CacheLocalityTest.cpp
    ChecksumTest.cpp
    ConcurrentSkipListTest.cpp
    ContainerTraitsTest.cpp
    ConvTest.cpp
    CpuIdTest.cpp
    DemangleTest.cpp
    DeterministicScheduleTest.cpp
    DiscriminatedPtrTest.cpp
    DynamicConverterTest.cpp
    DynamicOtherTest.cpp
    DynamicTest.cpp
    EndianTest.cpp
    EvictingCacheMapTest.cpp
    ExceptionTest.cpp
    ExceptionWrapperTest.cpp
    FBStringTest.cpp
    FBVectorTest.cpp
    FileTest.cpp
    #FileUtilTest.cpp
    FingerprintTest.cpp
    ForeachTest.cpp
    FormatOtherTest.cpp
    FormatTest.cpp
    FunctionSchedulerTest.cpp
    FutexTest.cpp
    GroupVarintTest.cpp
    HashTest.cpp
    HasMemberFnTraitsTest.cpp
    HistogramTest.cpp
    IndexedMemPoolTest.cpp
    IPAddressTest.cpp
    JsonOtherTest.cpp
    JsonTest.cpp
    LazyTest.cpp
    LifoSemTests.cpp
    LoggingTest.cpp
    MacAddressTest.cpp
    MapUtilTest.cpp
    MemoryIdlerTest.cpp
    MemoryMappingTest.cpp
    MemoryTest.cpp
    MergeTest.cpp
    MoveWrapperTest.cpp
    MPMCPipelineTest.cpp
    MPMCQueueTest.cpp
    OptionalTest.cpp
    PackedSyncPtrTest.cpp
    PaddedTest.cpp
    PortabilityTest.cpp
    ProducerConsumerQueueTest.cpp
    RandomTest.cpp
    RangeTest.cpp
    RWSpinLockTest.cpp
    SafeAssertTest.cpp
    ScopeGuardTest.cpp
    SharedMutexTest.cpp
    SingletonTest.cpp
    SingletonVaultCTest.cpp
    SmallLocksTest.cpp
    small_vector_test.cpp
    SocketAddressTest.cpp
    sorted_vector_test.cpp
    SpinLockTest.cpp
    SpookyHashV1Test.cpp
    SpookyHashV2Test.cpp
    StringTest.cpp
    SubprocessTest.cpp
    SynchronizedTest.cpp
    ThreadCachedArenaTest.cpp
    ThreadCachedIntTest.cpp
    ThreadLocalTest.cpp
    TimeoutQueueTest.cpp
    TimeseriesHistogramTest.cpp
    TimeseriesTest.cpp
    TraitsTest.cpp
    UriTest.cpp
    VarintTest.cpp
)

foreach(test_src ${FOLLY_BASE_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test folly_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES}
        folly_static folly_base_test)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()

execute_process(
    COMMAND ln -fs "${PROJECT_SOURCE_DIR}/folly/test/json_test_data"
                   "${PROJECT_BINARY_DIR}/folly/test/json_test_data"
)


set(FOLLY_BASE_BENCHMARK_SRCS
    AsciiCaseInsensitiveBenchmark.cpp
    ConcurrentSkipListBenchmark.cpp
    ExceptionWrapperBenchmark.cpp
    FBStringBenchmark.cpp
    FBVectorBenchmark.cpp
    FormatBenchmark.cpp
    HistogramBenchmark.cpp
    IPAddressBenchmark.cpp
    ProducerConsumerQueueBenchmark.cpp
    RangeFindBenchmark.cpp
    StringBenchmark.cpp
    TimeseriesBenchmark.cpp
)

foreach(bench_src ${FOLLY_BASE_BENCHMARK_SRCS})
    get_filename_component(bench_name ${bench_src} NAME_WE)
    set(bench folly_${bench_name})
    add_executable(${bench} ${bench_src})
    target_link_libraries(${bench} ${GTEST_BOTH_LIBRARIES} folly_static)
endforeach()


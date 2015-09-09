#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_SRCS
    FunctionScheduler.cpp
    JSONSchema.cpp
    NestedCommandLineApp.cpp
    ProgramOptions.cpp
    Select64.cpp
    TestUtil.cpp
)
add_library(folly_experimental OBJECT ${FOLLY_EXPERIMENTAL_SRCS})


install(FILES
    AutoTimer.h
    Bits.h
    BitVectorCoding.h
    EliasFanoCoding.h
    EventCount.h
    ExecutionObserver.h
    FunctionScheduler.h
    FutureDAG.h
    Instructions.h
    JSONSchema.h
    LockFreeRingBuffer.h
    NestedCommandLineApp.h
    ProgramOptions.h
    Select64.h
    StringKeyedCommon.h
    StringKeyedMap.h
    StringKeyedSet.h
    StringKeyedUnorderedMap.h
    StringKeyedUnorderedSet.h
    TestUtil.h
    TupleOps.h
    DESTINATION include/folly/experimental
)


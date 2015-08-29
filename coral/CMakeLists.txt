#
# Copyright (C) 2015, Yeolar
#

set(CORAL_BASE_SRCS
    Benchmark.cpp
    Conv.cpp
    Demangle.cpp
    dynamic.cpp
    File.cpp
    FileUtil.cpp
    Format.cpp
    json.cpp
    Logging.cpp
    Malloc.cpp
    Random.cpp
    Range.cpp
    SpookyHashV1.cpp
    SpookyHashV2.cpp
    StringBase.cpp
    String.cpp
    Time.cpp
    TypeDef.cpp
    Unicode.cpp
    build/EscapeTables.cpp
    build/FormatTables.cpp
    detail/Clock.cpp
    detail/FunctionalExcept.cpp
    detail/MallocImpl.cpp
)
add_library(coral
    ${CORAL_BASE_SRCS}
)
#target_link_libraries(coral
#)


install(FILES
    ApplyTuple.h
    Benchmark.h
    Conv.h
    coral-config.h
    CPortability.h
    CpuId.h
    Demangle.h
    DynamicConverter.h
    dynamic.h
    dynamic-inl.h
    Enum.h
    Exception.h
    FBString.h
    FBVector.h
    File.h
    FileUtil.h
    FixedStream.h
    Foreach.h
    FormatArg.h
    Format.h
    Format-inl.h
    FormatTraits.h
    Hash.h
    json.h
    Likely.h
    Logging.h
    Malloc.h
    MapUtil.h
    Memory.h
    MicroSpinLock.h
    NumericRange.h
    Optional.h
    PicoSpinLock.h
    Portability.h
    Preprocessor.h
    Random.h
    Random-inl.h
    Range.h
    RWSpinLock.h
    ScopeGuard.h
    SmallLocks.h
    SpinLock.h
    SpookyHashV1.h
    SpookyHashV2.h
    String.h
    String-inl.h
    Synchronized.h
    ThreadLocal.h
    Time.h
    Traits.h
    TypeDef.h
    Unicode.h
    DESTINATION include/coral
)
install(FILES
    detail/Clock.h
    detail/FunctionalExcept.h
    detail/Malloc.h
    detail/Sleeper.h
    detail/SpinLockImpl.h
    detail/ThreadLocalDetail.h
    detail/UncaughtExceptionCounter.h
    DESTINATION include/coral/detail
)
install(TARGETS
    coral
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)


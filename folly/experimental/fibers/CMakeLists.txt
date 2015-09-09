#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_FIBERS_SRCS
    Baton.cpp
    Fiber.cpp
    FiberManager.cpp
    FiberManagerMap.cpp
    GuardPageAllocator.cpp
    TimeoutController.cpp
)
add_library(folly_experimental_fibers OBJECT ${FOLLY_EXPERIMENTAL_FIBERS_SRCS})


install(FILES
    AddTasks.h
    AddTasks-inl.h
    Baton.h
    Baton-inl.h
    BoostContextCompatibility.h
    EventBaseLoopController.h
    EventBaseLoopController-inl.h
    Fiber.h
    Fiber-inl.h
    FiberManager.h
    FiberManager-inl.h
    FiberManagerMap.h
    ForEach.h
    ForEach-inl.h
    GenericBaton.h
    GuardPageAllocator.h
    LoopController.h
    Promise.h
    Promise-inl.h
    SimpleLoopController.h
    TimedMutex.h
    TimedMutex-inl.h
    TimeoutController.h
    traits.h
    WhenN.h
    WhenN-inl.h
    DESTINATION include/folly/experimental/fibers
)


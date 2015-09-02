#
# Copyright (C) 2015, Yeolar
#

set(CORAL_FIBERS_SRCS
    Baton.cpp
    Fiber.cpp
    FiberManager.cpp
    FiberManagerMap.cpp
    GuardPageAllocator.cpp
    TimeoutController.cpp
)
add_library(coral_fibers OBJECT ${CORAL_FIBERS_SRCS})


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
    DESTINATION include/coral/fibers
)


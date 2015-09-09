#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_FUTURES_SRCS
    Barrier.cpp
    Future.cpp
    InlineExecutor.cpp
    ManualExecutor.cpp
    QueuedImmediateExecutor.cpp
    ThreadedExecutor.cpp
    detail/ThreadWheelTimekeeper.cpp
)
add_library(folly_futures OBJECT ${FOLLY_FUTURES_SRCS})


install(FILES
    Barrier.h
    DrivableExecutor.h
    FutureException.h
    Future.h
    Future-inl.h
    Future-pre.h
    helpers.h
    InlineExecutor.h
    ManualExecutor.h
    OpaqueCallbackShunt.h
    Promise.h
    Promise-inl.h
    QueuedImmediateExecutor.h
    ScheduledExecutor.h
    SharedPromise.h
    SharedPromise-inl.h
    ThreadedExecutor.h
    Timekeeper.h
    Try.h
    Try-inl.h
    Unit.h
    DESTINATION include/folly/futures
)
install(FILES
    detail/Core.h
    detail/FSM.h
    detail/ThreadWheelTimekeeper.h
    detail/Types.h
    DESTINATION include/folly/futures/detail
)


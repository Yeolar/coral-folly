#
# Copyright (C) 2015, Yeolar
#

set(CORAL_IO_ASYNC_SRCS
    AsyncSSLSocket.cpp
    AsyncServerSocket.cpp
    AsyncSignalHandler.cpp
    AsyncSocket.cpp
    AsyncTimeout.cpp
    AsyncUDPSocket.cpp
    EventBase.cpp
    EventBaseManager.cpp
    EventHandler.cpp
    HHWheelTimer.cpp
    ScopedEventBaseThread.cpp
    SSLContext.cpp
)
add_library(coral_io_async OBJECT ${CORAL_IO_ASYNC_SRCS})


install(FILES
    AsyncSSLSocket.h
    AsyncServerSocket.h
    AsyncSignalHandler.h
    AsyncSocket.h
    AsyncSocketBase.h
    AsyncSocketException.h
    AsyncTimeout.h
    AsyncTransport.h
    AsyncUDPServerSocket.h
    AsyncUDPSocket.h
    CMakeLists.txt
    DelayedDestruction.h
    DelayedDestructionBase.h
    EventBase.h
    EventBaseManager.h
    EventFDWrapper.h
    EventHandler.h
    EventUtil.h
    HHWheelTimer.h
    NotificationQueue.h
    ScopedEventBaseThread.h
    SSLContext.h
    TimeoutManager.h
    DESTINATION include/coral/io/async
)

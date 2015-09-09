#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_IO_ASYNC_SRCS
    AsyncServerSocket.cpp
    AsyncSignalHandler.cpp
    AsyncSocket.cpp
    AsyncSSLSocket.cpp
    AsyncTimeout.cpp
    AsyncUDPSocket.cpp
    EventBase.cpp
    EventBaseLocal.cpp
    EventBaseManager.cpp
    EventHandler.cpp
    HHWheelTimer.cpp
    ScopedEventBaseThread.cpp
    SSLContext.cpp
)
add_library(folly_io_async OBJECT ${FOLLY_IO_ASYNC_SRCS})


install(FILES
    AsyncServerSocket.h
    AsyncSignalHandler.h
    AsyncSocketBase.h
    AsyncSocketException.h
    AsyncSocket.h
    AsyncSSLSocket.h
    AsyncTimeout.h
    AsyncTransport.h
    AsyncUDPServerSocket.h
    AsyncUDPSocket.h
    DelayedDestructionBase.h
    DelayedDestruction.h
    EventBase.h
    EventBaseLocal.h
    EventBaseManager.h
    EventFDWrapper.h
    EventHandler.h
    EventUtil.h
    HHWheelTimer.h
    NotificationQueue.h
    Request.h
    ScopedEventBaseThread.h
    SSLContext.h
    TimeoutManager.h
    DESTINATION include/folly/io/async
)
install(FILES
    test/MockAsyncServerSocket.h
    test/MockAsyncSocket.h
    test/MockAsyncSSLSocket.h
    test/MockAsyncTransport.h
    test/TimeUtil.h
    test/UndelayedDestruction.h
    test/Util.h
    DESTINATION include/folly/io/async/test
)


#
# Copyright (C) 2015, Yeolar
#

set(CORAL_IO_SRCS
    FsUtil.cpp
    IOBuf.cpp
    IOBufQueue.cpp
    ShutdownSocketSet.cpp
)
add_library(coral_io OBJECT ${CORAL_IO_SRCS})


install(FILES
    Cursor-defs.h
    Cursor.h
    FsUtil-inl.h
    FsUtil.h
    IOBuf.h
    IOBufQueue.h
    ShutdownSocketSet.h
    TypedIOBuf.h
    DESTINATION include/coral/io
)

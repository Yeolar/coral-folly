#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_IO_SRCS
    Compression.cpp
    IOBuf.cpp
    IOBufQueue.cpp
    RecordIO.cpp
    ShutdownSocketSet.cpp
)
add_library(folly_io OBJECT ${FOLLY_IO_SRCS})


install(FILES
    Compression.h
    Cursor-defs.h
    Cursor.h
    IOBuf.h
    IOBufQueue.h
    RecordIO.h
    RecordIO-inl.h
    ShutdownSocketSet.h
    TypedIOBuf.h
    DESTINATION include/folly/io
)

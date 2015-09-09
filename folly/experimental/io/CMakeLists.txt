#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_IO_SRCS
    AsyncIO.cpp
    FsUtil.cpp
    HugePages.cpp
)
add_library(folly_experimental_io OBJECT ${FOLLY_EXPERIMENTAL_IO_SRCS})


install(FILES
    AsyncIO.h
    FsUtil.h
    HugePages.h
    DESTINATION include/folly/experimental/io
)


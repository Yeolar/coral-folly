#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXT_IO_SRCS
    FsUtil.cpp
)
add_library(folly_ext_io OBJECT ${FOLLY_EXT_IO_SRCS})


install(FILES
    FsUtil.h
    FsUtil-inl.h
    DESTINATION include/folly/ext/io
)


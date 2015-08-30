#
# Copyright (C) 2015, Yeolar
#

set(CORAL_IO_SRCS
    FsUtil.cpp
)
add_library(coral_io ${CORAL_IO_SRCS})


install(FILES
    FsUtil.h
    FsUtil-inl.h
    DESTINATION include/coral/io
)


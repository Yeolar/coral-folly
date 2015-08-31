#
# Copyright (C) 2015, Yeolar
#

set(CORAL_IO_SRCS
    FsUtil.cpp
)
add_library(coral_io OBJECT ${CORAL_IO_SRCS})
set_property(TARGET coral_io PROPERTY POSITION_INDEPENDENT_CODE 1)


install(FILES
    FsUtil.h
    FsUtil-inl.h
    DESTINATION include/coral/io
)


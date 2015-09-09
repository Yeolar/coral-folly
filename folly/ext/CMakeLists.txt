#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXT_SRCS
    Binascii.cpp
    Logging.cpp
    Regex.cpp
    String.cpp
    StringBase.cpp
    Time.cpp
    TypeDef.cpp
    Unicode.cpp
    Utf8StringPiece.cpp
)
add_library(folly_ext OBJECT ${FOLLY_EXT_SRCS})


install(FILES
    Binascii.h
    Binascii-inl.h
    Enum.h
    FixedStream.h
    Logging.h
    MapUtil.h
    NumericRange.h
    Regex.h
    String.h
    String-inl.h
    Time.h
    TypeDef.h
    Unicode.h
    Utf8StringPiece.h
    DESTINATION include/folly/ext
)


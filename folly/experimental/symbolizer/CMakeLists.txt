#
# Copyright (C) 2015, Yeolar
#

set(FOLLY_EXPERIMENTAL_SYMBOLIZER_SRCS
    Dwarf.cpp
    ElfCache.cpp
    Elf.cpp
    LineReader.cpp
    SignalHandler.cpp
    StackTrace.cpp
    Symbolizer.cpp
)
add_library(folly_experimental_symbolizer OBJECT
    ${FOLLY_EXPERIMENTAL_SYMBOLIZER_SRCS})


install(FILES
    Dwarf.h
    ElfCache.h
    Elf.h
    Elf-inl.h
    LineReader.h
    SignalHandler.h
    StackTrace.h
    Symbolizer.h
    DESTINATION include/folly/experimental/symbolizer
)


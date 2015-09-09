# - Find Dwarf
#
#  DWARF_INCLUDE_DIR - where to find dwarf.h.
#  DWARF_LIBRARIES   - List of libraries.
#  DWARF_FOUND       - True if found.

INCLUDE(CheckLibraryExists)

if (DWARF_INCLUDE_DIR AND DWARF_LIBRARY)
	# Already in cache, be silent
	set(DWARF_FIND_QUIETLY TRUE)
endif (DWARF_INCLUDE_DIR AND DWARF_LIBRARY)

find_path(DWARF_INCLUDE_DIR dwarf.h
	/usr/include
	/usr/local/include
	/usr/include/libdwarf
	~/usr/local/include
)

find_library(DWARF_LIBRARY
	NAMES dw dwarf
	PATHS /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64 ~/usr/local/lib ~/usr/local/lib64
)

if (DWARF_INCLUDE_DIR AND DWARF_LIBRARY)
	set(DWARF_FOUND TRUE)
	set(DWARF_LIBRARIES ${DWARF_LIBRARY})
	set(CMAKE_REQUIRED_LIBRARIES ${DWARF_LIBRARIES})
else (DWARF_INCLUDE_DIR AND DWARF_LIBRARY)
	set(DWARF_FOUND FALSE)
	set(DWARF_LIBRARIES)
endif (DWARF_INCLUDE_DIR AND DWARF_LIBRARY)

if (DWARF_FOUND)
	if (NOT DWARF_FIND_QUIETLY)
		message(STATUS "Found dwarf.h header: ${DWARF_INCLUDE_DIR}")
		message(STATUS "Found libdw library: ${DWARF_LIBRARY}")
	endif (NOT DWARF_FIND_QUIETLY)
else (DWARF_FOUND)
	if (DWARF_FIND_REQUIRED)
		message(FATAL_ERROR "Could NOT find DWARF libraries, please install the missing packages")
	endif (DWARF_FIND_REQUIRED)
endif (DWARF_FOUND)

mark_as_advanced(DWARF_INCLUDE_DIR DWARF_LIBRARY)
include_directories(${DWARF_INCLUDE_DIR})


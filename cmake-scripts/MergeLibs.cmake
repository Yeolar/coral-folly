# Merge static libraries into a big static lib. The resulting library 
# should not not have dependencies on other static libraries.

function(GET_DEPENDEND_OS_LIBS target result)
  set(deps ${${target}_LIB_DEPENDS})
  if(deps)
   foreach(lib ${deps})
    # Filter out keywords for used for debug vs optimized builds
    if(NOT lib MATCHES "general" AND
       NOT lib MATCHES "debug" AND
       NOT lib MATCHES "optimized")
      get_target_property(lib_location ${lib} LOCATION)
      if(NOT lib_location)
        set(ret ${ret} ${lib})
      endif()
    endif()
   endforeach()
  endif()
  set(${result} ${ret} PARENT_SCOPE)
endfunction()

function(MERGE_STATIC_LIBS TARGET)
  # To produce a library we need at least one source file.
  # It is created by ADD_CUSTOM_COMMAND below and will helps
  # also help to track dependencies.
  set(SOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_depends.cpp)
  add_library(${TARGET} STATIC ${SOURCE_FILE})

  set(LIBS_TO_MERGE ${ARGV})
  list(REMOVE_AT LIBS_TO_MERGE 0)

  set(OSLIBS)
  foreach(LIB ${LIBS_TO_MERGE})
    if(TARGET ${LIB})
      # This is a target in current project
      # (can be a static or shared lib)
      get_target_property(LIB_TYPE ${LIB} TYPE)
      if(LIB_TYPE STREQUAL "STATIC_LIBRARY")
        list(APPEND STATIC_LIBS ${LIB})
        add_dependencies(${TARGET} ${LIB})
        # Extract dependend OS libraries
        GET_DEPENDEND_OS_LIBS(${LIB} LIB_OSLIBS)
        list(APPEND OSLIBS ${LIB_OSLIBS})
      else()
        # This is a shared library our static lib depends on.
        list(APPEND OSLIBS ${LIB})
      endif()
    else()
      # 3rd party library like libz.so. Make sure that everything
      # that links to our library links to this one as well.
      list(APPEND OSLIBS ${LIB})
    endif()
  endforeach()
  if(OSLIBS)
    list(REMOVE_DUPLICATES OSLIBS)
    target_link_libraries(${TARGET} LINK_PUBLIC ${OSLIBS})
  endif()

  # Make the generated dummy source file depended on all static input
  # libs. If input lib changes,the source file is touched
  # which causes the desired effect (relink).
  add_custom_command(
    OUTPUT  ${SOURCE_FILE}
    COMMAND ${CMAKE_COMMAND}  -E touch ${SOURCE_FILE}
    DEPENDS ${STATIC_LIBS})

  if(MSVC)
    # To merge libs, just pass them to lib.exe command line.
    set(LINKER_EXTRA_FLAGS "")
    foreach(LIB ${STATIC_LIBS})
      set(LINKER_EXTRA_FLAGS "${LINKER_EXTRA_FLAGS} $<TARGET_FILE:${LIB}>")
    endforeach()
    set_target_properties(${TARGET} PROPERTIES STATIC_LIBRARY_FLAGS 
      "${LINKER_EXTRA_FLAGS}")
  else()
    foreach(STATIC_LIB ${STATIC_LIBS})
      list(APPEND STATIC_LIB_FILES $<TARGET_FILE:${STATIC_LIB}>)
    endforeach()
    if(APPLE)
      # Use OSX's libtool to merge archives (ihandles universal 
      # binaries properly)
      add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND rm $<TARGET_FILE:${TARGET}>
        COMMAND /usr/bin/libtool -static -o $<TARGET_FILE:${TARGET}>
        ${STATIC_LIB_FILES}
      )
    else()
      # Generic Unix, Cygwin or MinGW. In post-build step, call
      # script, that extracts objects from archives with "ar x" 
      # and repacks them with "ar r"
      set(TARGET ${TARGET})
      configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/merge_archives_unix.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/merge_archives_${TARGET}.cmake 
        @ONLY
        )
      string(REGEX REPLACE ";" "\\\;" STATIC_LIB_FILES "${STATIC_LIB_FILES}")
      add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND rm $<TARGET_FILE:${TARGET}>
        COMMAND ${CMAKE_COMMAND}
          -D TARGET_FILE=$<TARGET_FILE:${TARGET}>
          -D STATIC_LIB_FILES="${STATIC_LIB_FILES}"
          -P ${CMAKE_CURRENT_BINARY_DIR}/merge_archives_${TARGET}.cmake
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/merge_archives_${TARGET}.cmake"
      )
    endif()
  endif()
endfunction()

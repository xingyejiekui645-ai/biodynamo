# - Find TCMalloc
#
# This module defines the following variables:
#  TCMALLOC_INCLUDE_DIR
#  TCMALLOC_LIBRARY
#  TCMALLOC_LIBRARIES
#  TCMALLOC_LIBRARY_DIRS (not cached)
#  PPROF_EXECUTABLE

find_path(TCMALLOC_INCLUDE_DIR NAMES gperftools/tcmalloc.h google/tcmalloc.h)
foreach(component tcmalloc profiler)
  find_library(TCMALLOC_${component}_LIBRARY NAMES ${component})
  mark_as_advanced(TCMALLOC_${component}_LIBRARY)
endforeach()

if(TCMALLOC_INCLUDE_DIR AND TCMALLOC_tcmalloc_LIBRARY)
  set(TCMALLOC_FOUND TRUE)
  set(TCMALLOC_LIBRARIES ${TCMALLOC_tcmalloc_LIBRARY})
  if(TCMALLOC_profiler_LIBRARY)
    list(APPEND TCMALLOC_LIBRARIES ${TCMALLOC_profiler_LIBRARY})
  endif()
  get_filename_component(TCMALLOC_LIBRARY_DIRS ${TCMALLOC_tcmalloc_LIBRARY} PATH)
else()
  set(TCMALLOC_FOUND FALSE)
endif()

if(TCMALLOC_FOUND)
  if(NOT TCMALLOC_FIND_QUIETLY)
    message(STATUS "Found TCMalloc: ${TCMALLOC_LIBRARIES}")
  endif()
else()
  if(TCMALLOC_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find TCMalloc")
  endif()
endif()

mark_as_advanced(TCMALLOC_INCLUDE_DIR TCMALLOC_LIBRARIES TCMALLOC_LIBRARY_DIRS)

# Find pprof
find_program(PPROF_EXECUTABLE pprof)
mark_as_advanced(PPROF_EXECUTABLE)

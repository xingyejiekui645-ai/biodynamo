function(fix_rootcling_omp_issue)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} --print-file-name=include/omp.h
                  OUTPUT_VARIABLE OMP_HEADER_PATH)
  string(REGEX REPLACE "\n$" "" OMP_HEADER_PATH "${OMP_HEADER_PATH}")

  # Get major GCC version
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
  string(REGEX REPLACE "\n$" "" GCC_VERSION "${GCC_VERSION}")
  string(REGEX MATCH "^[0-9]+" GCC_VERSION_MAJOR "${GCC_VERSION}")

  # Use third_party/omp/omp.h if compiler didn't resolve full path or gcc version > 11
  if ("${OMP_HEADER_PATH}" STREQUAL "include/omp.h" OR GCC_VERSION_MAJOR GREATER 11)
    if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/omp/omp.h")
      execute_process(COMMAND cp -a ${CMAKE_SOURCE_DIR}/third_party/omp ${CMAKE_BINARY_DIR})
    elseif(EXISTS "$ENV{BDMSYS}/third_party/omp/omp.h")
      execute_process(COMMAND cp -a $ENV{BDMSYS}/third_party/omp ${CMAKE_BINARY_DIR})
    elseif(EXISTS "$ENV{BDMSYS}/omp/omp.h")
      execute_process(COMMAND cp -a $ENV{BDMSYS}/omp ${CMAKE_BINARY_DIR})
    else()
      message(FATAL_ERROR "The BioDynaMo environment is not set up correctly. Please execute 'source <path-to-bdm-installation>/bin/thisbdm.sh' and retry this command.")
    endif()
  else()
    execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/omp)
    execute_process(COMMAND cp -f ${OMP_HEADER_PATH} ${CMAKE_BINARY_DIR}/omp)
  endif()
  include_directories("${CMAKE_BINARY_DIR}/omp")
endfunction(fix_rootcling_omp_issue)

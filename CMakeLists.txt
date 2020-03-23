cmake_minimum_required(VERSION 3.4)
MESSAGE("\nConfiguring hypercine...\n")

PROJECT(hypercine)

IF(WIN32)
  IF(NOT BUILD_SHARED_LIBS)
    MESSAGE("\nWarning: This is a windows build, but BUILD_SHARED_LIBS is OFF. Setting BUILD_SHARED_LIBS on automatically.")
    SET(BUILD_SHARED_LIBS ON)
  ENDIF(NOT BUILD_SHARED_LIBS)
ENDIF(WIN32)

MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
SET(DICE_OUTPUT_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "If 'make install' is exectued, the libraries will also be copied to: ${CMAKE_INSTALL_PREFIX}/lib")

# if debug messages are turned on:
IF(HYPERCINE_DEBUG_MSG)
  MESSAGE(STATUS "Debugging messages are ON")
  ADD_DEFINITIONS(-DHYPERCINE_DEBUG_MSG=1)
ELSE()
  MESSAGE(STATUS "Debugging messages are OFF")
ENDIF()

SET (HYPERCINE_SOURCES
  ./src/hypercine.cpp
)

SET (HYPERCINE_HEADERS
 ./src/hypercine.h
)

add_library(hypercine ${HYPERCINE_SOURCES} ${HYPERCINE_HEADERS})
target_compile_definitions(hypercine PRIVATE -D HYPERCINE_LIB_EXPORTS_MODE)

add_subdirectory(tests)
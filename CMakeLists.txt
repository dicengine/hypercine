cmake_minimum_required(VERSION 3.4)
MESSAGE("\nConfiguring hypercine...\n")

PROJECT(hypercine)

MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
SET(DICE_OUTPUT_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "If 'make install' is exectued, the libraries will also be copied to: ${CMAKE_INSTALL_PREFIX}/lib")

SET (HYPERCINE_SOURCES
  ./src/hypercine.cpp
)

SET (HYPERCINE_HEADERS
 ./src/hypercine.h
)

add_library(hypercine ${HYPERCINE_SOURCES} ${HYPERCINE_HEADERS})

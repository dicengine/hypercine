enable_testing()

# Try to find OpenCV, if path was not given
IF (NOT DEFINED OpenCV_DIR)
  # Losely based on:
  # https://github.com/trilinos/Trilinos_tutorial/wiki/CMakeFindPackageTrilinosExample
  # /usr/share/doc/opencv-doc/examples/face/CMakeLists.txt
  # * Find OpenCV
  message(STATUS "OpenCV_DIR not specified, looking in default paths")
  find_package(OpenCV REQUIRED)
ELSE()
  message(STATUS "Looking for OpenCV in dir: ${OpenCV_DIR}")
  find_package( OpenCV NO_DEFAULT_PATH PATHS ${OpenCV_DIR} )
ENDIF()
IF(OpenCV_FOUND)
ELSE()
  message(FATAL_ERROR "OpenCV not found. OpenCV is required")
ENDIF()

# copy the image files to the build dir
FILE ( GLOB img_files "${CMAKE_CURRENT_SOURCE_DIR}/images/*.*")
FOREACH ( img_file ${img_files} )
    FILE(COPY ${img_file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images/ )
ENDFOREACH ( )


# glob all the cpp files
FILE ( GLOB test_cpp_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
FOREACH ( test_file ${test_cpp_files})
  # get the name of the test from the cpp filename
  GET_FILENAME_COMPONENT ( name ${test_file} NAME_WE)
  add_executable(${name} ${test_file})
  target_link_libraries(${name} hypercine ${OpenCV_LIBRARIES})
  target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
  add_test(NAME ${name}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${name})
  set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION "TEST PASSED")
ENDFOREACH ( )

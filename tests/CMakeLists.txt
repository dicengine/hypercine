enable_testing()

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
  #install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  target_link_libraries(${name} hypercine ${OpenCV_LIBRARIES})
  target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
  add_test(NAME ${name}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${name})
  set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION "TEST PASSED")
ENDFOREACH ( )

# glob all the cpp files
FILE ( GLOB tools_cpp_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
FOREACH ( tools_file ${tools_cpp_files})
  # get the name of the tool from the cpp filename
  GET_FILENAME_COMPONENT ( name ${tools_file} NAME_WE)
  add_executable(${name} ${tools_file})
  target_link_libraries(${name} hypercine ${OpenCV_LIBRARIES})
  install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
ENDFOREACH ( )

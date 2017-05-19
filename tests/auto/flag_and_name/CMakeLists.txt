
project( test.flag_and_name )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.flag_and_name ${SRC} )

add_test( NAME test.flag_and_name
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.flag_and_name
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )


project( test.arg_as_command )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.arg_as_command ${SRC} )

add_test( NAME test.arg_as_command
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.arg_as_command
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )


project( test.command )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.command ${SRC} )

add_test( NAME test.command
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.command
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

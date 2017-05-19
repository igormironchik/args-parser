
project( test.arg )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.arg ${SRC} )

add_test( NAME test.arg
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.arg
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

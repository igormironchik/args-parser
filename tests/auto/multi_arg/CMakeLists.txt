
project( test.multi_arg )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.multi_arg ${SRC} )

add_test( NAME test.multi_arg
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.multi_arg
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

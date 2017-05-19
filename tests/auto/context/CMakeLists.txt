
project( test.context )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.context ${SRC} )

add_test( NAME test.context
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.context
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

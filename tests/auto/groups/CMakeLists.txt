
project( test.groups )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.groups ${SRC} )

add_test( NAME test.groups
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.groups
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

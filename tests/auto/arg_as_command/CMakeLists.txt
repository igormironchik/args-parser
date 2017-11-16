
project( test.arg_as_command )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty/doctest )

add_executable( test.arg_as_command ${SRC} )

add_test( NAME test.arg_as_command
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.arg_as_command
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

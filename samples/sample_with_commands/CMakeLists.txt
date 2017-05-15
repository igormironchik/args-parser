
project( example.with_commands )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

add_executable( example.with_commands ${SRC} )

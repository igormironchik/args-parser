
project( test.build )

set( SRC main.cpp setup.cpp setup.hpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.build ${SRC} )

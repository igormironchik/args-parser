
project( example.qt_help )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

find_package( Qt5Core )

add_executable( example.qt_help ${SRC} )

target_link_libraries( example.qt_help Qt5::Core )

target_compile_definitions( example.qt_help PRIVATE
	ARGS_QSTRING_BUILD )

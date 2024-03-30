# SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project( example.qt_help )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

find_package( Qt6Core )

add_executable( example.qt_help ${SRC} )

target_link_libraries( example.qt_help Qt6::Core )

target_compile_definitions( example.qt_help PRIVATE
	ARGS_QSTRING_BUILD )

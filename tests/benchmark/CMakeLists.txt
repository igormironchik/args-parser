# SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required( VERSION 3.1 )

project( benchmark )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

find_package( Boost COMPONENTS program_options REQUIRED )

add_executable( benchmark main.cpp )

target_link_libraries( benchmark ${Boost_LIBRARIES} )

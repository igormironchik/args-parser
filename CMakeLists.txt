# SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required( VERSION 3.19 )

set( ARGS_VERSION "6.3.4" )

option( BUILD_EXAMPLES "Build examples? Default ON." ON )
option( BUILD_TESTS "Build tests? Default ON." ON )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE )
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

project( args-parser )

if( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )

	if( BUILD_EXAMPLES )
		add_subdirectory( samples )
	endif()

	if( BUILD_TESTS )
		enable_testing()

		add_subdirectory( tests )
	endif()

	file( GLOB_RECURSE SRC args-parser/* )

	add_library( args-parser INTERFACE ${SRC} )
	add_library( args-parser::args-parser ALIAS args-parser )

	target_include_directories( args-parser INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

	install( DIRECTORY args-parser
		DESTINATION include
	)

	install( TARGETS args-parser
		EXPORT args-parser-targets
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		INCLUDES DESTINATION include
	)

	install( EXPORT args-parser-targets
		DESTINATION lib/cmake/args-parser
		NAMESPACE args-parser::
	)

	include( CMakePackageConfigHelpers )

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/args-parser-config-version.cmake"
		VERSION ${ARGS_VERSION}
		COMPATIBILITY AnyNewerVersion )

	configure_package_config_file( args-parser-config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/args-parser-config.cmake"
		INSTALL_DESTINATION lib/cmake/args-parser )

	install( FILES ${PROJECT_BINARY_DIR}/args-parser-config.cmake
			${PROJECT_BINARY_DIR}/args-parser-config-version.cmake
		DESTINATION lib/cmake/args-parser )

else()

	set( args-parser_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

endif( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )


cmake_minimum_required( VERSION 2.8.11 )

option( BUILD_EXAMPLES "Build examples? Default ON." ON )
option( BUILD_TESTS "Build tests? Default ON." ON )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE )
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

file( COPY runtests.rb
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

project( Args )

if( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )

	if( BUILD_EXAMPLES )
		add_subdirectory( samples )
	endif()

	if( BUILD_TESTS )
		enable_testing()

		add_subdirectory( tests )
	endif()

else()

	add_library( Args INTERFACE )

	set( Args_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

endif( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )


cmake_minimum_required( VERSION 2.8.11 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

file( COPY runtests.rb
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

project( Args )

add_library( Args INTERFACE )

target_include_directories( Args INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

set( Args_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

add_subdirectory( samples )

enable_testing()

add_subdirectory( tests )

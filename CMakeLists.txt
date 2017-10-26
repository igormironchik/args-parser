
cmake_minimum_required( VERSION 2.8.11 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

if( ENABLE_COVERAGE )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g " )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage" )
    set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

file( COPY runtests.rb
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

project( Args )

add_subdirectory( samples )

enable_testing()

add_subdirectory( tests )

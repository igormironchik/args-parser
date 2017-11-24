
cmake_minimum_required( VERSION 2.8.11 )

set( Args_MAJOR_VERSION 5 )
set( Args_MINOR_VERSION 0 )
set( Args_PATCH_VERSION 1 )
set( Args_VERSION
	${Args_MAJOR_VERSION}.${Args_MINOR_VERSION}.${Args_PATCH_VERSION} )

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

	add_library( Args INTERFACE )

	install( DIRECTORY Args
		DESTINATION include
	)

	install( FILES
			Args/all.hpp
			Args/api.hpp
			Args/arg_as_command.hpp
			Args/arg.hpp
			Args/arg_iface.hpp
			Args/cmd_line.hpp
			Args/command.hpp
			Args/context.hpp
			Args/enums.hpp
			Args/exceptions.hpp
			Args/group_iface.hpp
			Args/groups.hpp
			Args/help.hpp
			Args/help_printer.hpp
			Args/help_printer_iface.hpp
			Args/multi_arg.hpp
			Args/types.hpp
			Args/utils.hpp
			Args/value_utils.hpp
		DESTINATION
			include/Args
	)

	install( TARGETS Args
		EXPORT ArgsTargets
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		INCLUDES DESTINATION include
	)

	install( EXPORT ArgsTargets
		DESTINATION lib/cmake/Args
	)

	configure_file( ArgsConfig.cmake.in
		"ArgsConfig.cmake" @ONLY )

	install( FILES ${PROJECT_BINARY_DIR}/ArgsConfig.cmake
		DESTINATION lib/cmake/Args
	)

	configure_file( ArgsConfigVersion.cmake.in
		"ArgsConfigVersion.cmake" @ONLY )

	install( FILES ${PROJECT_BINARY_DIR}/ArgsConfigVersion.cmake
		DESTINATION lib/cmake/Args
	)

else()

	set( Args_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

endif( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )

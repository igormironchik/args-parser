
cmake_minimum_required( VERSION 2.8.11 )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( Args )

add_subdirectory( samples )
add_subdirectory( tests )

enable_testing()

add_test( NAME test.arg
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/arg/test.arg
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/arg )

add_test( NAME test.arg_as_command
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/arg_as_command/test.arg_as_command
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/arg_as_command )

add_test( NAME test.command
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/command/test.command
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/command )

add_test( NAME test.context
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/context/test.context
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/context )

add_test( NAME test.flag_and_name
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/flag_and_name/test.flag_and_name
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/flag_and_name )

add_test( NAME test.groups
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/groups/test.groups
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/groups )

add_test( NAME test.multi_arg
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/multi_arg/test.multi_arg
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/multi_arg )

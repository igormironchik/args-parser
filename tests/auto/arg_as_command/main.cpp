
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013-2017 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// UnitTest include.
#include <UnitTest/unit_test.hpp>

// Args include.
#include <Args/arg.hpp>
#include <Args/groups.hpp>
#include <Args/cmd_line.hpp>
#include <Args/exceptions.hpp>
#include <Args/command.hpp>
#include <Args/arg_as_command.hpp>


using namespace Args;


TEST( ArgAsCommandCase, TestAllIsOk )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100", "pos", "400" };

	CmdLine cmd( argc, argv );

	Arg t( 't', true, true );
	ArgAsCommand p( "pos", true, ValueOptions::OneValue );

	cmd.addArg( t );
	cmd.addArg( p );

	cmd.parse();

	CHECK_CONDITION( t.isDefined() == true )
	CHECK_CONDITION( t.value() == "100" )

	CHECK_CONDITION( p.isDefined() == true )
	CHECK_CONDITION( p.values().size() == 1 )
	CHECK_CONDITION( p.value() == "400" )
}

TEST( ArgAsCommandCase, TestManyValues )
{
	const int argc = 6;
	const char * argv[ argc ] = { "program.exe",
		"pos", "100", "200", "300", "400" };

	CmdLine cmd( argc, argv );

	ArgAsCommand p( "pos", true, ValueOptions::ManyValues );

	cmd.addArg( p );

	cmd.parse();

	CHECK_CONDITION( p.isDefined() == true )
	CHECK_CONDITION( p.values().size() == 4 )

	auto it = p.values().cbegin();

	CHECK_CONDITION( *it == "100" )

	++it;

	CHECK_CONDITION( *it == "200" )

	++it;

	CHECK_CONDITION( *it == "300" )

	++it;

	CHECK_CONDITION( *it == "400" )
}

int main()
{
	RUN_ALL_TESTS()

	return 0;
}


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
#include <Args/cmd_line.hpp>
#include <Args/exceptions.hpp>
#include <Args/command.hpp>


using namespace Args;


TEST( CommandCase, TestAllIsOk )
{
	const int argc = 7;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100",
		"--port", "4545",
		"--host", "any" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	cmd.addArg( &timeout );
	cmd.addArg( &port );
	cmd.addArg( &host );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == "100" )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == "4545" )

	CHECK_CONDITION( host.isDefined() == true )
	CHECK_CONDITION( host.value() == "any" )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

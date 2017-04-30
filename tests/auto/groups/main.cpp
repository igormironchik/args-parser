
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2017 Igor Mironchik

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


using namespace Args;


TEST( GroupCase, TestOnlyOneAllIsOk )
{
	const int argc = 3;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	OnlyOneGroup g( "only_one" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == "100" )

	CHECK_CONDITION( port.isDefined() == false )

	CHECK_CONDITION( host.isDefined() == false )
}


TEST( GroupCase, TestOnlyOneFailed )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100",
		"-p", "4545" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	OnlyOneGroup g( "only_one" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestOnlyOneWithRequiredFailed )
{
	const int argc = 3;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true, true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	OnlyOneGroup g( "only_one" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestAllOfIsOk )
{
	const int argc = 7;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100", "-p", "4545", "-h", "localhost" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	AllOfGroup g( "all_of" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == "100" )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == "4545" )

	CHECK_CONDITION( host.isDefined() == true )
	CHECK_CONDITION( host.value() == "localhost" )
}


TEST( GroupCase, TestAllOfFailed )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100",
		"-p", "4545" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	AllOfGroup g( "all_of" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestAllOfWithRequiredFailed )
{
	const int argc = 3;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true, true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	OnlyOneGroup g( "all_of" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( GroupCase, TestAtLeasOneIsOk )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100", "-p", "4545" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	AtLeastOneGroup g( "at_least_one" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == "100" )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == "4545" )

	CHECK_CONDITION( host.isDefined() == false )

	CHECK_CONDITION( g.isDefined() == true )
}


TEST( GroupCase, TestAtLeasOneFailed )
{
	const int argc = 1;
	const char * argv[ argc ] = { "program.exe" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	AtLeastOneGroup g( "at_least_one" );
	g.setRequired( true );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestAtLeasOneWithRequiredFailed )
{
	const int argc = 3;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100" };

	CmdLine cmd( argc, argv );

	Arg timeout( 't', std::string( "timeout" ), true, true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	AtLeastOneGroup g( "at_least_one" );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, GroupsIsOk )
{
	const int argc = 9;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100", "-p", "4545", "-h", "localhost", "-s", "db" };

	CmdLine cmd( argc, argv );

	Arg store( 's', "store", true );
	Arg file( 'f', "file", true );

	OnlyOneGroup onlyOne( "only_one" );
	onlyOne.addArg( store );
	onlyOne.addArg( file );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	AllOfGroup g( "all_of" );
	g.setRequired( true );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );
	g.addArg( onlyOne );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == "100" )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == "4545" )

	CHECK_CONDITION( host.isDefined() == true )
	CHECK_CONDITION( host.value() == "localhost" )

	CHECK_CONDITION( store.isDefined() == true )
	CHECK_CONDITION( store.value() == "db" )

	CHECK_CONDITION( file.isDefined() == false )

	CHECK_CONDITION( g.isDefined() == true )
}


TEST( GroupCase, GroupsFailed )
{
	const int argc = 11;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100", "-p", "4545", "-h", "localhost", "-s", "db",
		"-f", "out.txt" };

	CmdLine cmd( argc, argv );

	Arg store( 's', "store", true );
	Arg file( 'f', "file", true );

	OnlyOneGroup onlyOne( "only_one" );
	onlyOne.addArg( store );
	onlyOne.addArg( file );

	Arg timeout( 't', std::string( "timeout" ), true );
	Arg port( 'p', std::string( "port" ), true );
	Arg host( 'h', std::string( "host" ), true );

	AllOfGroup g( "all_of" );
	g.setRequired( true );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );
	g.addArg( onlyOne );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

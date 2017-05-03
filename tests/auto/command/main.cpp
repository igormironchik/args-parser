
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
#include <Args/all.hpp>


using namespace Args;


TEST( CommandCase, TestAllIsOk )
{
	const int argc = 7;
	const char * argv[ argc ] = { "program.exe",
		"add", "100", "4545", "500",
		"-f", "local" };

	CmdLine cmd( argc, argv );

	Arg f( 'f', true, true );

	Command c( "add", ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	cmd.parse();

	CHECK_CONDITION( f.isDefined() == true )
	CHECK_CONDITION( f.value() == "local" )

	CHECK_CONDITION( c.isDefined() == true )
	CHECK_CONDITION( c.value() == "100" )
	CHECK_CONDITION( c.values().size() == 3 )

	auto it = c.values().begin();

	CHECK_CONDITION( *it == "100" )

	++it;

	CHECK_CONDITION( *it == "4545" )

	++it;

	CHECK_CONDITION( *it == "500" )
}

TEST( CommandCase, WrongName1 )
{
	CHECK_THROW( Command c( "-a" ), BaseException )
}

TEST( CommandCase, WrongName2 )
{
	CHECK_THROW( Command c( "--name" ), BaseException )
}

TEST( CommandCase, TestNotDefinedRequiredArgInCommand )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"add", "100", "4545", "500" };

	CmdLine cmd( argc, argv );

	Arg f( 'f', true, true );

	Command c( "add", ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( CommandCase, TestManyArgs )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"add", "100", "4545", "500" };

	CmdLine cmd( argc, argv );

	Arg f( 'f', true, true );

	Command c( "add", ValueOptions::OneValue );
	c.addArg( f );

	cmd.addArg( c );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( CommandCase, TestRedefinition )
{
	const int argc = 3;
	const char * argv[ argc ] = { "program.exe",
		"add", "del" };

	CmdLine cmd( argc, argv );

	Command c( "add" );
	Command d( "del" );

	cmd.addArg( c );
	cmd.addArg( d );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( CommandCase, TestNotDefinedCommand )
{
	const int argc = 1;
	const char * argv[ argc ] = { "program.exe" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command c( "add" );
	Command d( "del" );

	cmd.addArg( c );
	cmd.addArg( d );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( CommandCase, TestAllIsOk2 )
{
	const int argc = 7;
	const char * argv[ argc ] = { "program.exe",
		"add", "100", "4545", "500",
		"-f", "local" };

	CmdLine cmd( argc, argv );

	Arg f( 'f', true );

	Command c( "add", ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	cmd.parse();

	CHECK_CONDITION( f.isDefined() == true )
	CHECK_CONDITION( f.value() == "local" )

	CHECK_CONDITION( c.isDefined() == true )
	CHECK_CONDITION( c.value() == "100" )
	CHECK_CONDITION( c.values().size() == 3 )

	auto it = c.values().begin();

	CHECK_CONDITION( *it == "100" )

	++it;

	CHECK_CONDITION( *it == "4545" )

	++it;

	CHECK_CONDITION( *it == "500" )
}

TEST( CommandCase, TestAllIsOk3 )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"add", "100", "4545", "500" };

	CmdLine cmd( argc, argv );

	Arg f( 'f', true );

	Command c( "add", ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	cmd.parse();

	CHECK_CONDITION( f.isDefined() == false )

	CHECK_CONDITION( c.isDefined() == true )
	CHECK_CONDITION( c.value() == "100" )
	CHECK_CONDITION( c.values().size() == 3 )

	auto it = c.values().begin();

	CHECK_CONDITION( *it == "100" )

	++it;

	CHECK_CONDITION( *it == "4545" )

	++it;

	CHECK_CONDITION( *it == "500" )
}

TEST( CommandCase, TestAllIsOk4 )
{
	const int argc = 1;
	const char * argv[ argc ] = { "program.exe" };

	CmdLine cmd( argc, argv );

	Command c( "add" );
	Command d( "del" );

	cmd.addArg( c );
	cmd.addArg( d );

	CHECK_CONDITION( c.isDefined() == false )
	CHECK_CONDITION( d.isDefined() == false )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

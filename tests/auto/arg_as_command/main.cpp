
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


TEST( ArgAsCommandCase, TestWithArg )
{
	const int argc = 5;
	const char * argv[ argc ] = { "program.exe",
		"-t", "100", "pos", "400" };

	CmdLine cmd( argc, argv );

	Arg t( Char( 't' ), true, true );
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

TEST( ArgAsCommandCase, TestNoValue )
{
	const int argc = 2;
	const char * argv[ argc ] = { "program.exe",
		"pos" };

	CmdLine cmd( argc, argv );

	ArgAsCommand p( "pos", true );

	cmd.addArg( p );

	cmd.parse();

	CHECK_CONDITION( p.isDefined() == true )
	CHECK_CONDITION( p.values().size() == 0 )
	CHECK_CONDITION( p.value().empty() )
}

TEST( ArgAsCommandCase, TestUnderCommand )
{
	const int argc = 4;
	const char * argv[ argc ] = { "program.exe",
		"add", "file", "out.txt" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( "add" );
	ArgAsCommand file( "file", true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	cmd.parse();

	CHECK_CONDITION( add.isDefined() == true )

	CHECK_CONDITION( file.isDefined() == true )
	CHECK_CONDITION( file.values().size() == 1 )
	CHECK_CONDITION( file.value() == "out.txt" )
}

TEST( ArgAsCommandCase, TestWrongName )
{
	CHECK_THROW( ArgAsCommand( "-pos" ), BaseException )
	CHECK_THROW( ArgAsCommand( "--pos" ), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandNotDefinedRequired )
{
	const int argc = 2;
	const char * argv[ argc ] = { "program.exe",
		"add" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( "add" );
	ArgAsCommand file( "file", true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandNotDefinedValue )
{
	const int argc = 3;
	const char * argv[ argc ] = { "program.exe",
		"add", "file" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( "add" );
	ArgAsCommand file( "file", true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandNotDefinedValue2 )
{
	const int argc = 4;
	const char * argv[ argc ] = { "program.exe",
		"add", "file", "-t" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( "add" );
	ArgAsCommand file( "file", true, ValueOptions::ManyValues );
	Arg t( Char( 't' ) );

	add.addArg( file );

	cmd.addArg( add );
	cmd.addArg( t );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandWithManyValuesAndArg )
{
	const int argc = 6;
	const char * argv[ argc ] = { "program.exe",
		"add", "file", "1.txt", "2.txt", "-t" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( "add" );
	ArgAsCommand file( "file", true, ValueOptions::ManyValues );
	Arg t( Char( 't' ) );

	add.addArg( file );

	cmd.addArg( add );
	cmd.addArg( t );

	cmd.parse();

	CHECK_CONDITION( add.isDefined() == true )

	CHECK_CONDITION( file.isDefined() == true )

	CHECK_CONDITION( file.values().size() == 2 )

	auto it = file.values().cbegin();

	CHECK_CONDITION( *it == "1.txt" )

	++it;

	CHECK_CONDITION( *it == "2.txt" )

	CHECK_CONDITION( t.isDefined() == true )
}

TEST( ArgAsCommandCase, TestAlreadyDefined )
{
	const int argc = 4;
	const char * argv[ argc ] = { "program.exe",
		"file", "1.txt", "file" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	ArgAsCommand file( "file", true, ValueOptions::ManyValues );

	cmd.addArg( file );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestNameRedefinition )
{
	const int argc = 3;
	const char * argv[ argc ] = { "program.exe",
		"add", "add" };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( "add" );
	ArgAsCommand file( "add", true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	CHECK_THROW( cmd.parse(), BaseException )
}

int main()
{
	RUN_ALL_TESTS()

	return 0;
}


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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest.h>

// Args include.
#include <Args/all.hpp>


using namespace Args;

#ifdef ARGS_WSTRING_BUILD
	using CHAR = String::value_type;
#else
	using CHAR = char;
#endif


TEST_CASE( "TestWithArg" )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ), SL( "pos" ), SL( "400" ) };

	CmdLine cmd( argc, argv );

	Arg t( Char( SL( 't' ) ), true, true );
	ArgAsCommand p( SL( "pos" ), true, ValueOptions::OneValue );

	cmd.addArg( t );
	cmd.addArg( p );

	cmd.parse();

	REQUIRE( t.isDefined() == true );
	REQUIRE( t.value() == SL( "100" ) );

	REQUIRE( p.isDefined() == true );
	REQUIRE( p.values().size() == 1 );
	REQUIRE( p.value() == SL( "400" ) );
}

TEST_CASE( "TestManyValues" )
{
	const int argc = 6;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "pos" ), SL( "100" ), SL( "200" ), SL( "300" ), SL( "400" ) };

	CmdLine cmd( argc, argv );

	ArgAsCommand p( SL( "pos" ), true, ValueOptions::ManyValues );

	cmd.addArg( p );

	cmd.parse();

	REQUIRE( p.isDefined() == true );
	REQUIRE( p.values().size() == 4 );

	auto it = p.values().cbegin();

	REQUIRE( *it == SL( "100" ) );

	++it;

	REQUIRE( *it == SL( "200" ) );

	++it;

	REQUIRE( *it == SL( "300" ) );

	++it;

	REQUIRE( *it == SL( "400" ) );
}

TEST_CASE( "TestNoValue" )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "pos" ) };

	CmdLine cmd( argc, argv );

	ArgAsCommand p( SL( "pos" ), true );

	cmd.addArg( p );

	cmd.parse();

	REQUIRE( p.isDefined() == true );
	REQUIRE( p.values().size() == 0 );
	REQUIRE( p.value().empty() );
}

TEST_CASE( "TestUnderCommand" )
{
	const int argc = 4;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ), SL( "out.txt" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	cmd.parse();

	REQUIRE( add.isDefined() == true );

	REQUIRE( file.isDefined() == true );
	REQUIRE( file.values().size() == 1 );
	REQUIRE( file.value() == SL( "out.txt" ) );
}

TEST_CASE( "TestWrongName" )
{
	REQUIRE_THROWS_AS( ArgAsCommand( SL( "-pos" ) ), BaseException );
	REQUIRE_THROWS_AS( ArgAsCommand( SL( "--pos" ) ), BaseException );
}

TEST_CASE( "TestUnderCommandNotDefinedRequired" )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestUnderCommandNotDefinedValue" )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestUnderCommandNotDefinedValue2" )
{
	const int argc = 4;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ), SL( "-t" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );
	Arg t( Char( SL( 't' ) ) );

	add.addArg( file );

	cmd.addArg( add );
	cmd.addArg( t );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestUnderCommandWithManyValuesAndArg" )
{
	const int argc = 6;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ), SL( "1.txt" ), SL( "2.txt" ), SL( "-t" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );
	Arg t( Char( SL( 't' ) ) );

	add.addArg( file );

	cmd.addArg( add );
	cmd.addArg( t );

	cmd.parse();

	REQUIRE( add.isDefined() == true );

	REQUIRE( file.isDefined() == true );

	REQUIRE( file.values().size() == 2 );

	auto it = file.values().cbegin();

	REQUIRE( *it == SL( "1.txt" ) );

	++it;

	REQUIRE( *it == SL( "2.txt" ) );

	REQUIRE( t.isDefined() == true );
}

TEST_CASE( "TestAlreadyDefined" )
{
	const int argc = 4;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "file" ), SL( "1.txt" ), SL( "file" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );

	cmd.addArg( file );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestNameRedefinition" )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "add" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "add" ), true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestAllIsOk1" )
{
	const int argc = 1;

	const CHAR * argv[ argc ] = { SL( "program.exe" ) };

	CmdLine cmd( argc, argv );

	ArgAsCommand file( SL( "add" ), false, ValueOptions::ManyValues );

	cmd.addArg( file );

	cmd.parse();

	REQUIRE( file.defaultValue().empty() );
	REQUIRE( file.defaultValues().empty() );
	REQUIRE( file.value().empty() );
	REQUIRE( file.values().empty() );
}

TEST_CASE( "TestAllIsOk2" )
{
	const int argc = 1;

	const CHAR * argv[ argc ] = { SL( "program.exe" ) };

	CmdLine cmd( argc, argv );

	ArgAsCommand file( SL( "add" ), false, ValueOptions::ManyValues );
	file.setDefaultValue( SL( "default" ) );

	cmd.addArg( file );

	cmd.parse();

	REQUIRE( file.defaultValue() == SL( "default" ) );
	REQUIRE( file.defaultValues().size() == 1 );
	REQUIRE( file.defaultValues().front() == SL( "default" ) );
	REQUIRE( file.value() == SL( "default" ) );
	REQUIRE( file.values().size() == 1 );
	REQUIRE( file.values().front() == SL( "default" ) );
}

TEST_CASE( "TestMisspelling" )
{
	ArgAsCommand add( SL( "add" ) );

	StringList correct;

	REQUIRE( add.isMisspelledName( SL( "dad" ), correct ) );
	REQUIRE( correct.size() == 1 );
	REQUIRE( correct.front() == SL( "add" ) );
}

TEST_CASE( "ArgAsCommandStuff" )
{
	ArgAsCommand file( SL( "add" ), false, ValueOptions::ManyValues );
	file.setDefaultValue( SL( "default" ) );

	REQUIRE( file.defaultValue() == SL( "default" ) );
	REQUIRE( file.defaultValues().size() == 1 );
	REQUIRE( file.defaultValues().front() == SL( "default" ) );
	REQUIRE( !file.isRequired() );
	REQUIRE( file.isWithValue() );
	REQUIRE( !file.isDefined() );
}

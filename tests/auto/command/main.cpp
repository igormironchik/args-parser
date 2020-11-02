
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013-2020 Igor Mironchik

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


TEST_CASE( "TestAllIsOk1" )
{
	const int argc = 7;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "100" ), SL( "4545" ), SL( "500" ),
		SL( "-f" ), SL( "local" ) };

	CmdLine cmd( argc, argv );

	Arg f( Char( SL( 'f' ) ), true, true );

	Command c( SL( "add" ), ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	cmd.parse();

	REQUIRE( f.isDefined() == true );
	REQUIRE( f.value() == SL( "local" ) );

	REQUIRE( c.isDefined() == true );
	REQUIRE( c.value() == SL( "100" ) );
	REQUIRE( c.values().size() == 3 );

	auto it = c.values().begin();

	REQUIRE( *it == SL( "100" ) );

	++it;

	REQUIRE( *it == SL( "4545" ) );

	++it;

	REQUIRE( *it == SL( "500" ) );
}

TEST_CASE( "WrongName1" )
{
	REQUIRE_THROWS_AS( Command c( SL( "-a" ) ), BaseException );
}

TEST_CASE( "WrongName2" )
{
	REQUIRE_THROWS_AS( Command c( SL( "--name" ) ), BaseException );
}

TEST_CASE( "TestNotDefinedRequiredArgInCommand" )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "100" ), SL( "4545" ), SL( "500" ) };

	CmdLine cmd( argc, argv );

	Arg f( Char( SL( 'f' ) ), true, true );

	Command c( SL( "add" ), ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestRedifinition" )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ), SL( "1.txt" ), SL( "del" ) };

	CmdLine cmd( argc, argv );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue, true )
			.addCommand( SL( "file" ), ValueOptions::OneValue, false )
			.end()
		.end()
		.addCommand( SL( "del" ), ValueOptions::NoValue, true )
			.addCommand( SL( "file" ), ValueOptions::OneValue, false )
			.end()
		.end()
	.end();

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestOkWithSubCommand" )
{
	const int argc = 4;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ), SL( "1.txt" ) };

	CmdLine cmd( argc, argv );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue, true )
			.addCommand( SL( "file" ), ValueOptions::OneValue, false )
			.end()
		.end()
		.addCommand( SL( "del" ), ValueOptions::NoValue, true )
			.addCommand( SL( "file" ), ValueOptions::OneValue, false )
			.end()
		.end()
	.end();

	cmd.parse();

	REQUIRE( cmd.isDefined( SL( "add" ) ) );
	REQUIRE( cmd.isDefined( SL( "file" ) ) );

	const auto v = cmd.values( SL( "file" ) );

	REQUIRE( v.size() == 1 );
	REQUIRE( v.front() == SL( "1.txt" ) );
}

TEST_CASE( "TestManyArgs" )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "100" ), SL( "4545" ), SL( "500" ) };

	CmdLine cmd( argc, argv );

	Arg f( Char( SL( 'f' ) ), true, true );

	Command c( SL( "add" ), ValueOptions::OneValue );
	c.addArg( f );

	cmd.addArg( c );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestRedefinition" )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "del" ) };

	CmdLine cmd( argc, argv );

	Command c( SL( "add" ) );
	Command d( SL( "del" ) );

	cmd.addArg( c );
	cmd.addArg( d );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestNotDefinedCommand" )
{
	const int argc = 1;
	const CHAR * argv[ argc ] = { SL( "program.exe" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command c( SL( "add" ) );
	Command d( SL( "del" ) );

	cmd.addArg( c );
	cmd.addArg( d );

	REQUIRE_THROWS_AS( cmd.parse(), BaseException );
}

TEST_CASE( "TestAllIsOk2" )
{
	const int argc = 7;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "100" ), SL( "4545" ), SL( "500" ),
		SL( "-f" ), SL( "local" ) };

	CmdLine cmd( argc, argv );

	Arg f( Char( SL( 'f' ) ), true );

	Command c( SL( "add" ), ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	cmd.parse();

	REQUIRE( f.isDefined() == true );
	REQUIRE( f.value() == SL( "local" ) );

	REQUIRE( c.isDefined() == true );
	REQUIRE( c.value() == SL( "100" ) );
	REQUIRE( c.values().size() == 3 );

	auto it = c.values().begin();

	REQUIRE( *it == SL( "100" ) );

	++it;

	REQUIRE( *it == SL( "4545" ) );

	++it;

	REQUIRE( *it == SL( "500" ) );
}

TEST_CASE( "TestAllIsOk3" )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "100" ), SL( "4545" ), SL( "500" ) };

	CmdLine cmd( argc, argv );

	Arg f( Char( SL( 'f' ) ), true );

	Command c( SL( "add" ), ValueOptions::ManyValues );
	c.addArg( f );

	cmd.addArg( c );

	cmd.parse();

	REQUIRE( f.isDefined() == false );

	REQUIRE( c.isDefined() == true );
	REQUIRE( c.value() == SL( "100" ) );
	REQUIRE( c.values().size() == 3 );

	auto it = c.values().begin();

	REQUIRE( *it == SL( "100" ) );

	++it;

	REQUIRE( *it == SL( "4545" ) );

	++it;

	REQUIRE( *it == SL( "500" ) );
}

TEST_CASE( "TestAllIsOk4" )
{
	const int argc = 1;
	const CHAR * argv[ argc ] = { SL( "program.exe" ) };

	CmdLine cmd( argc, argv );

	Command c( SL( "add" ) );
	Command d( SL( "del" ) );

	cmd.addArg( c );
	cmd.addArg( d );

	REQUIRE( c.isDefined() == false );
	REQUIRE( d.isDefined() == false );
}

TEST_CASE( "TestAllIsOk5" )
{
	const int argc = 1;
	const CHAR * argv[ argc ] = { SL( "program.exe" ) };

	CmdLine cmd( argc, argv );

	Command a( SL( "add" ), ValueOptions::ManyValues );

	cmd.addArg( a );

	cmd.parse();

	REQUIRE( a.isDefined() == false );
	REQUIRE( a.value().empty() );
	REQUIRE( a.values().empty() );
	REQUIRE( a.defaultValue().empty() );
	REQUIRE( a.defaultValues().empty() );
}

TEST_CASE( "TestAllIsOk6" )
{
	const int argc = 1;
	const CHAR * argv[ argc ] = { SL( "program.exe" ) };

	CmdLine cmd( argc, argv );

	Command a( SL( "add" ), ValueOptions::ManyValues );
	a.setDefaultValue( SL( "default" ) );

	cmd.addArg( a );

	cmd.parse();

	REQUIRE( a.isDefined() == false );
	REQUIRE( a.value() == SL( "default" ) );
	REQUIRE( a.values().size() == 1 );
	REQUIRE( a.values().front() == SL( "default" ) );
	REQUIRE( a.defaultValue() == SL( "default" ) );
	REQUIRE( a.defaultValues().size() == 1 );
	REQUIRE( a.defaultValues().front() == SL( "default" ) );
}

TEST_CASE( "TestMisspelling" )
{
	CmdLine cmd;

	Command add( SL( "add" ) );

	Command file( SL( "file" ) );
	add.addCommand( file );

	cmd.addArg( add );

	StringList correct;

	REQUIRE( add.isMisspelledName( SL( "dad" ), correct ) );
	REQUIRE( correct.size() == 1 );
	REQUIRE( correct.front() == SL( "add" ) );

	correct.clear();

	REQUIRE( add.isMisspelledName( SL( "fiel" ), correct ) );
	REQUIRE( correct.size() == 1 );
	REQUIRE( correct.front() == SL( "file" ) );

	correct.clear();

	REQUIRE( !add.isMisspelledCommand( SL( "fiel" ), correct ) );

	REQUIRE( add.isMisspelledCommand( SL( "dad" ), correct ) );
	REQUIRE( correct.size() == 1 );
	REQUIRE( correct.front() == SL( "add" ) );

	correct.clear();

	REQUIRE( cmd.isMisspelledName( SL( "dad" ), correct ) );
	REQUIRE( correct.size() == 1 );
	REQUIRE( correct.front() == SL( "add" ) );
}

TEST_CASE( "TestStuff" )
{
	Command add( SL( "add" ) );

	ArgIface & a = add;

	REQUIRE( a.description().empty() );
	REQUIRE( a.longDescription().empty() );

	add.setDescription( SL( "desc" ) );

	REQUIRE( a.description() == SL( "desc" ) );
	REQUIRE( a.longDescription() == SL( "desc" ) );

	add.setLongDescription( SL( "long" ) );

	REQUIRE( a.description() == SL( "desc" ) );
	REQUIRE( a.longDescription() == SL( "long" ) );

	REQUIRE( a.valueSpecifier().empty() );

	add.setValueSpecifier( SL( "val" ) );

	REQUIRE( a.valueSpecifier() == SL( "val" ) );

	REQUIRE( !a.isDefined() );

	GroupIface & g = add;

	REQUIRE( !g.isRequired() );

	g.setRequired();

	REQUIRE( !g.isRequired() );

	Command c( SL( "c" ), ValueOptions::ManyValues );

	ArgIface & a2 = c;

	REQUIRE( a2.isWithValue() );
}

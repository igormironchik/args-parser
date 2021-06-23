
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
#include <args-parser/all.hpp>


using namespace Args;

#ifdef ARGS_WSTRING_BUILD
	using CHAR = String::value_type;
#else
	using CHAR = char;
#endif


TEST_CASE( "TestNoValueForArg" )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-a" ) };

	CmdLine cmd;

	cmd.addArgWithFlagOnly( SL( 'a' ), true );

	try {
		cmd.parse( argc, argv );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Argument \"-a\" requires value that wasn't presented." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestRequiredArgInAllOfGroup" )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-a" ) };

	CmdLine cmd;

	cmd.addAllOfGroup( SL( "group" ) )
			.addArgWithFlagOnly( SL( 'a' ), true, true )
		.end();

	try {
		cmd.parse( argc, argv );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Required argument \"-a\" is not allowed to be in "
				"AllOf group \"group\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestAlreadyInCmdLine1" )
{
	CmdLine cmd;

	try {
		auto a1 = CmdLine::ArgPtr( new Arg( Char( SL( 'a' ) ), true, true ),
			details::Deleter< ArgIface >( true ) );
		auto a2 = CmdLine::ArgPtr( a1.get(),
			details::Deleter< ArgIface >( false ) );

		cmd.addArg( std::move( a1 ) );
		cmd.addArg( std::move( a2 ) );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Argument \"-a\" already in the command line parser." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestAlreadyInCmdLine2" )
{
	CmdLine cmd;

	try {
		Arg a1( Char( SL( 'a' ) ), true, true );

		cmd.addArg( &a1 );
		cmd.addArg( &a1 );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Argument \"-a\" already in the command line parser." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestAddingNull" )
{
	CmdLine cmd;

	try {
		cmd.addArg( nullptr );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Attempt to add nullptr to the "
				"command line as argument." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestEmptyNameOfCommand" )
{
	try {
		CmdLine cmd;

		Command a( SL( "" ) );

		cmd.addArg( a );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Command can't be with empty name." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestRedefinitionOfCommand" )
{
	try {
		CmdLine cmd;

		Command a( SL( "cmd" ) );
		Command b( SL( "cmd" ) );

		cmd.addArg( a );
		cmd.addArg( b );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Redefinition of command with name \"cmd\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestDissallowedNameOfCommand" )
{
	try {
		CmdLine cmd;

		Command a( SL( "cmd with space" ) );

		cmd.addArg( a );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Disallowed name \"cmd with space\" for the command." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestArgAlreadyDefined" )
{
	try {
		const int argc = 3;
		const CHAR * argv[ argc ] = { SL( "program.exe" ),
			SL( "-a" ), SL( "-a" ) };

		CmdLine cmd;

		cmd.addArgWithFlagOnly( SL( 'a' ) );

		cmd.parse( argc, argv );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Argument \"-a\" already defined." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestArgRedefinition" )
{
	try {
		CmdLine cmd;

		cmd.addArgWithFlagOnly( SL( 'a' ) )
			.addArgWithFlagOnly( SL( 'a' ) );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Redefinition of argument witg flag \"-a\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestDissallowedFlag" )
{
	try {
		CmdLine cmd;

		cmd.addArgWithFlagOnly( SL( '~' ) );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Disallowed flag \"-~\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestArgNameRedefinition" )
{
	try {
		CmdLine cmd;

		cmd.addArgWithNameOnly( SL( "name" ) )
			.addArgWithNameOnly( SL( "name" ) );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Redefinition of argument with name \"--name\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestArgDissallowedName" )
{
	try {
		CmdLine cmd;

		cmd.addArgWithNameOnly( SL( "~~" ) );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Disallowed name \"--~~\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestArgEmptyNameAndFlag" )
{
	try {
		CmdLine cmd;

		cmd.addArgWithNameOnly( SL( "" ) );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Arguments with empty flag and name "
				"are disallowed." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestUnknownFlag" )
{
	try {
		const int argc = 2;
		const CHAR * argv[ argc ] = { SL( "program.exe" ),
			SL( "-h" ) };

		CmdLine cmd;

		cmd.parse( argc, argv );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Unknown argument \"-h\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "TestWrongCombo" )
{
	try {
		const int argc = 2;
		const CHAR * argv[ argc ] = { SL( "program.exe" ),
			SL( "-ab" ) };

		CmdLine cmd;

		cmd.addArgWithFlagOnly( SL( 'a' ), true )
			.addArgWithFlagOnly( SL( 'b' ), true );

		cmd.parse( argc, argv );
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Only last argument in "
				"flags combo can be with value. Flags combo is \"-ab\"." ) );

		return;
	}

	REQUIRE( false );
}

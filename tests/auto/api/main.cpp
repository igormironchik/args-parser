
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


TEST_CASE( "TestAllIsOk" )
{
	const int argc = 7;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ),
		SL( "-f" ), SL( "test.txt" ),
		SL( "-d" ), SL( "-r" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue,
				SL( "Add file." ) )
			.addAllOfGroup( SL( "file group" ) )
				.addSubCommand( SL( "file" ), false, ValueOptions::NoValue )
				.addArgWithFlagAndName( SL( 'f' ), SL( "file" ), true, false,
					SL( "Name of the file." ), SL( "" ), SL( "" ),
					SL( "fn" ) )
			.end()
			.addArgWithFlagOnly( SL( 'd' ), false, false, SL( "Do job." ) )
		.end()
		.addCommand( SL( "delete" ), ValueOptions::NoValue,
				SL( "Delete file." ) )
			.addArgWithFlagOnly( SL( 'd' ), false, false,
				SL( "Do NOT job." ) )
		.end()
		.addArgWithFlagAndName( SL( 'r' ), SL( "recurcieve" ), false, false,
			SL( "Do operation recurcively?" ) )
		.addHelp( true, argv[ 0 ],
			SL( "This application just show power of the Args help." ) )
		// Dummy, just for compillation testing.
		.addAllOfGroup( SL( "1" ) )
		.end()
		.addOnlyOneGroup( SL( "2" ) )
		.end()
		.addAtLeastOneGroup( SL( "3" ) )
			.addAllOfGroup( SL( "4" ) )
			.end()
			.addOnlyOneGroup( SL( "5" ) )
			.end()
			.addAtLeastOneGroup( SL( "6" ) )
			.end()
			.addMultiArg( SL( '7' ), SL( "77" ) )
			.addMultiArgWithFlagOnly( SL( '8' ) )
			.addMultiArgWithNameOnly( SL( "9" ) )
		.end()
		.addMultiArg( SL( 'q' ), SL( "qq" ) )
		.addMultiArgWithFlagOnly( SL( 'w' ) )
		.addMultiArgWithNameOnly( SL( "qwe" ) );

	cmd.parse();

	REQUIRE( cmd.isDefined( SL( "add" ) ) );
	REQUIRE( cmd.isDefined( SL( "file" ) ) );
	REQUIRE( cmd.isDefined( SL( "-f" ) ) );
	REQUIRE( cmd.value( SL( "-f" ) ) == SL( "test.txt" ) );
	REQUIRE( cmd.values( SL( "-f" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "-f" ) ).front() == SL( "test.txt" ) );
	REQUIRE( cmd.isDefined( SL( "-d" ) ) );
	REQUIRE( cmd.isDefined( SL( "-r" ) ) );

	REQUIRE( !cmd.isDefined( SL( "delete" ) ) );
	REQUIRE( !cmd.isDefined( SL( "-h" ) ) );
}

TEST_CASE( "TestAllIsOkReparse" )
{
	const int argc = 7;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ),
		SL( "-f" ), SL( "test.txt" ),
		SL( "-d" ), SL( "-r" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue,
				SL( "Add file." ) )
			.addAllOfGroup( SL( "file group" ) )
				.addSubCommand( SL( "file" ), false, ValueOptions::NoValue )
				.addArgWithFlagAndName( SL( 'f' ), SL( "file" ), true, false,
					SL( "Name of the file." ), SL( "" ), SL( "" ),
					SL( "fn" ) )
			.end()
			.addArgWithFlagOnly( SL( 'd' ), false, false, SL( "Do job." ) )
		.end()
		.addCommand( SL( "delete" ), ValueOptions::NoValue,
				SL( "Delete file." ) )
			.addArgWithFlagOnly( SL( 'd' ), false, false,
				SL( "Do NOT job." ) )
		.end()
		.addArgWithFlagAndName( SL( 'r' ), SL( "recurcieve" ), false, false,
			SL( "Do operation recurcively?" ) )
		.addHelp( true, argv[ 0 ],
			SL( "This application just show power of the Args help." ) )
		// Dummy, just for compillation testing.
		.addMultiArgWithDefaulValues( SL( '1' ), SL( "11" ) )
		.addMultiArgWithFlagOnlyAndDefaultValues( SL( '2' ) )
		.addMultiArgWithNameOnlyAndDefaultValues( SL( "22" ) )
		.addArgAsCommandWithDefaulValues( SL( "33" ) )
		.addSubCommandWithDefaultValues( SL( "44" ) )
		.addAtLeastOneGroup( SL( "5" ) )
			.addMultiArgWithDefaulValues( SL( '6' ), SL( "66" ) )
			.addMultiArgWithFlagOnlyAndDefaultValues( SL( '7' ) )
			.addMultiArgWithNameOnlyAndDefaultValues( SL( "77" ) )
			.addArgAsCommandWithDefaulValues( SL( "88" ) )
			.addSubCommandWithDefaultValues( SL( "99" ) )
		.end();

	cmd.parse();

	REQUIRE( cmd.isDefined( SL( "add" ) ) );
	REQUIRE( cmd.isDefined( SL( "file" ) ) );
	REQUIRE( cmd.isDefined( SL( "-f" ) ) );
	REQUIRE( cmd.value( SL( "-f" ) ) == SL( "test.txt" ) );
	REQUIRE( cmd.values( SL( "-f" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "-f" ) ).front() == SL( "test.txt" ) );
	REQUIRE( cmd.isDefined( SL( "-d" ) ) );
	REQUIRE( cmd.isDefined( SL( "-r" ) ) );

	REQUIRE( !cmd.isDefined( SL( "delete" ) ) );
	REQUIRE( !cmd.isDefined( SL( "-h" ) ) );

	const int nargc = 2;
	const CHAR * nargv[ nargc ] = { SL( "program.exe" ),
		SL( "delete" ) };

	cmd.parse( nargc, nargv );

	REQUIRE( !cmd.isDefined( SL( "add" ) ) );
	REQUIRE( cmd.isDefined( SL( "delete" ) ) );
}

TEST_CASE( "TestAllIsOkWithEmptyCtor" )
{
	const int argc = 7;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ),
		SL( "-f" ), SL( "test.txt" ),
		SL( "-d" ), SL( "-r" ) };

	CmdLine cmd( CmdLine::CommandIsRequired );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue,
				SL( "Add file." ) )
			.addAllOfGroup( SL( "file group" ) )
				.addSubCommand( SL( "file" ), false, ValueOptions::NoValue )
				.addArgWithFlagAndName( SL( 'f' ), SL( "file" ), true, false,
					SL( "Name of the file." ), SL( "" ), SL( "" ),
					SL( "fn" ) )
			.end()
			.addArgWithFlagOnly( SL( 'd' ), false, false, SL( "Do job." ) )
		.end()
		.addCommand( SL( "delete" ), ValueOptions::NoValue,
				SL( "Delete file." ) )
			.addArgWithFlagOnly( SL( 'd' ), false, false,
				SL( "Do NOT job." ) )
		.end()
		.addArgWithFlagAndName( SL( 'r' ), SL( "recurcieve" ), false, false,
			SL( "Do operation recurcively?" ) )
		.addHelp( true, argv[ 0 ],
			SL( "This application just show power of the Args help." ) )
		// Dummy, just for compillation testing.
		.addAllOfGroup( SL( "1" ) )
		.end()
		.addOnlyOneGroup( SL( "2" ) )
		.end()
		.addAtLeastOneGroup( SL( "3" ) )
			.addAllOfGroup( SL( "4" ) )
			.end()
			.addOnlyOneGroup( SL( "5" ) )
			.end()
			.addAtLeastOneGroup( SL( "6" ) )
			.end()
			.addMultiArg( SL( '7' ), SL( "77" ) )
			.addMultiArgWithFlagOnly( SL( '8' ) )
			.addMultiArgWithNameOnly( SL( "9" ) )
		.end()
		.addMultiArg( SL( 'q' ), SL( "qq" ) )
		.addMultiArgWithFlagOnly( SL( 'w' ) )
		.addMultiArgWithNameOnly( SL( "qwe" ) );

	cmd.parse( argc, argv );

	REQUIRE( cmd.isDefined( SL( "add" ) ) );
	REQUIRE( cmd.isDefined( SL( "file" ) ) );
	REQUIRE( cmd.isDefined( SL( "-f" ) ) );
	REQUIRE( cmd.value( SL( "-f" ) ) == SL( "test.txt" ) );
	REQUIRE( cmd.values( SL( "-f" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "-f" ) ).front() == SL( "test.txt" ) );
	REQUIRE( cmd.isDefined( SL( "-d" ) ) );
	REQUIRE( cmd.isDefined( SL( "-r" ) ) );

	REQUIRE( !cmd.isDefined( SL( "delete" ) ) );
	REQUIRE( !cmd.isDefined( SL( "-h" ) ) );
}

TEST_CASE( "TestGetterSetterOfArg" )
{
	CmdLine cmd;

	cmd.addArgWithFlagAndName( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgWithFlagOnly( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgWithNameOnly( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) );

	auto * a1 = static_cast< Arg* > ( cmd.findArgument( SL( "-1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const CmdLine & tmpCmd = cmd;

	const auto * a2 = static_cast< const Arg* > (
		tmpCmd.findArgument( SL( "-2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val" ) );

	auto * a3 = static_cast< Arg* > ( cmd.findArgument( SL( "--33" ) ) );

	REQUIRE( a3->description() == SL( "desc" ) );
	REQUIRE( a3->longDescription() == SL( "long desc" ) );
	REQUIRE( a3->value() == SL( "val" ) );
	REQUIRE( a3->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a3->defaultValue() == SL( "val" ) );

	REQUIRE( cmd.value( SL( "-1" ) ) == SL( "val" ) );
	REQUIRE( cmd.values( SL( "-1" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "-1" ) ).front() == SL( "val" ) );
}

TEST_CASE( "TestGetterSetterOfMultiArg" )
{
	CmdLine cmd;

	cmd.addMultiArg( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addMultiArgWithFlagOnly( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addMultiArgWithNameOnly( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) );

	auto * a1 = static_cast< Arg* > ( cmd.findArgument( SL( "-1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const CmdLine & tmpCmd = cmd;

	const auto * a2 = static_cast< const Arg* > (
		tmpCmd.findArgument( SL( "-2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val" ) );

	const auto * a3 = static_cast< const Arg* > (
		tmpCmd.findArgument( SL( "--33" ) ) );

	REQUIRE( a3->description() == SL( "desc" ) );
	REQUIRE( a3->longDescription() == SL( "long desc" ) );
	REQUIRE( a3->value() == SL( "val" ) );
	REQUIRE( a3->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a3->defaultValue() == SL( "val" ) );

	REQUIRE( cmd.value( SL( "-1" ) ) == SL( "val" ) );
	REQUIRE( cmd.values( SL( "-1" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "-1" ) ).front() == SL( "val" ) );
}

TEST_CASE( "TestGetterSetterOfMultiArgWithValues" )
{
	CmdLine cmd;

	cmd.addMultiArgWithDefaulValues( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), { SL( "val1" ), SL( "val2" ) },
			SL( "vs" ) )
		.addMultiArgWithFlagOnlyAndDefaultValues( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), { SL( "val1" ), SL( "val2" ) },
			SL( "vs" ) )
		.addMultiArgWithNameOnlyAndDefaultValues( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), { SL( "val1" ), SL( "val2" ) },
			SL( "vs" ) );

	auto * a1 = static_cast< MultiArg* > ( cmd.findArgument( SL( "-1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val1" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val1" ) );
	REQUIRE( a1->defaultValues().size() == 2 );
	REQUIRE( a1->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a1->defaultValues().back() == SL( "val2" ) );

	const auto * a2 = static_cast< const MultiArg* > ( cmd.findArgument( SL( "-2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val1" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().size() == 2 );
	REQUIRE( a2->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().back() == SL( "val2" ) );

	auto * a3 = static_cast< MultiArg* > ( cmd.findArgument( SL( "--33" ) ) );

	REQUIRE( a3->description() == SL( "desc" ) );
	REQUIRE( a3->longDescription() == SL( "long desc" ) );
	REQUIRE( a3->value() == SL( "val1" ) );
	REQUIRE( a3->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a3->defaultValue() == SL( "val1" ) );
	REQUIRE( a3->defaultValues().size() == 2 );
	REQUIRE( a3->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a3->defaultValues().back() == SL( "val2" ) );

	REQUIRE( cmd.value( SL( "-1" ) ) == SL( "val1" ) );
	REQUIRE( cmd.values( SL( "-1" ) ).size() == 2 );
	REQUIRE( cmd.values( SL( "-1" ) ).front() == SL( "val1" ) );
	REQUIRE( cmd.values( SL( "-1" ) ).back() == SL( "val2" ) );
}

TEST_CASE( "TestGetterSetterOfArgAsCommand" )
{
	CmdLine cmd;

	cmd.addArgAsCommand( SL( "arg1" ), false, ValueOptions::OneValue,
		SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgAsCommandWithDefaulValues( SL( "arg2" ), false,
			ValueOptions::ManyValues, SL( "desc" ), SL( "long desc" ),
			{ SL( "val1" ), SL( "val2" ) }, SL( "vs" ) );

	auto * a1 = static_cast< ArgAsCommand* > (
		cmd.findArgument( SL( "arg1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val1" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().size() == 2 );
	REQUIRE( a2->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().back() == SL( "val2" ) );

	REQUIRE( cmd.value( SL( "arg1" ) ) == SL( "val" ) );
	REQUIRE( cmd.values( SL( "arg1" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "arg1" ) ).front() == SL( "val" ) );

	REQUIRE( cmd.value( SL( "arg2" ) ) == SL( "val1" ) );
	REQUIRE( cmd.values( SL( "arg2" ) ).size() == 2 );
	REQUIRE( cmd.values( SL( "arg2" ) ).front() == SL( "val1" ) );
	REQUIRE( cmd.values( SL( "arg2" ) ).back() == SL( "val2" ) );
}

TEST_CASE( "TestGetterSetterOfSubCommand" )
{
	CmdLine cmd;

	cmd.addSubCommand( SL( "arg1" ), false, ValueOptions::OneValue,
		SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addSubCommandWithDefaultValues( SL( "arg2" ), false,
			ValueOptions::ManyValues, SL( "desc" ), SL( "long desc" ),
			{ SL( "val1" ), SL( "val2" ) }, SL( "vs" ) );

	auto * a1 = static_cast< ArgAsCommand* > (
		cmd.findArgument( SL( "arg1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val1" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().size() == 2 );
	REQUIRE( a2->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().back() == SL( "val2" ) );

	REQUIRE( cmd.value( SL( "arg1" ) ) == SL( "val" ) );
	REQUIRE( cmd.values( SL( "arg1" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "arg1" ) ).front() == SL( "val" ) );
}

TEST_CASE( "TestGetterSetterOfArgInGroup" )
{
	CmdLine cmd;

	cmd.addAllOfGroup( SL( "group" ) )
		.addArgWithFlagAndName( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgWithFlagOnly( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgWithNameOnly( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.end();

	auto * a1 = static_cast< Arg* > ( cmd.findArgument( SL( "-1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const auto * a2 = static_cast< const Arg* > ( cmd.findArgument( SL( "-2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val" ) );

	auto * a3 = static_cast< Arg* > ( cmd.findArgument( SL( "--33" ) ) );

	REQUIRE( a3->description() == SL( "desc" ) );
	REQUIRE( a3->longDescription() == SL( "long desc" ) );
	REQUIRE( a3->value() == SL( "val" ) );
	REQUIRE( a3->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a3->defaultValue() == SL( "val" ) );
}

TEST_CASE( "TestGetterSetterOfMultiArgInGroup" )
{
	CmdLine cmd;

	cmd.addAllOfGroup( SL( "group" ) )
		.addMultiArg( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addMultiArgWithFlagOnly( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addMultiArgWithNameOnly( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.end();

	auto * a1 = static_cast< Arg* > ( cmd.findArgument( SL( "-1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const auto * a2 = static_cast< const Arg* > ( cmd.findArgument( SL( "-2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val" ) );

	auto * a3 = static_cast< Arg* > ( cmd.findArgument( SL( "--33" ) ) );

	REQUIRE( a3->description() == SL( "desc" ) );
	REQUIRE( a3->longDescription() == SL( "long desc" ) );
	REQUIRE( a3->value() == SL( "val" ) );
	REQUIRE( a3->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a3->defaultValue() == SL( "val" ) );
}

TEST_CASE( "TestGetterSetterOfMultiArgWithValuesInGroup" )
{
	CmdLine cmd;

	cmd.addAllOfGroup( SL( "group" ) )
		.addMultiArgWithDefaulValues( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), { SL( "val1" ), SL( "val2" ) },
			SL( "vs" ) )
		.addMultiArgWithFlagOnlyAndDefaultValues( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), { SL( "val1" ), SL( "val2" ) },
			SL( "vs" ) )
		.addMultiArgWithNameOnlyAndDefaultValues( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), { SL( "val1" ), SL( "val2" ) },
			SL( "vs" ) )
		.end();

	auto * a1 = static_cast< MultiArg* > ( cmd.findArgument( SL( "-1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val1" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val1" ) );
	REQUIRE( a1->defaultValues().size() == 2 );
	REQUIRE( a1->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a1->defaultValues().back() == SL( "val2" ) );

	const auto * a2 = static_cast< const MultiArg* > ( cmd.findArgument( SL( "-2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val1" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().size() == 2 );
	REQUIRE( a2->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().back() == SL( "val2" ) );

	auto * a3 = static_cast< MultiArg* > ( cmd.findArgument( SL( "--33" ) ) );

	REQUIRE( a3->description() == SL( "desc" ) );
	REQUIRE( a3->longDescription() == SL( "long desc" ) );
	REQUIRE( a3->value() == SL( "val1" ) );
	REQUIRE( a3->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a3->defaultValue() == SL( "val1" ) );
	REQUIRE( a3->defaultValues().size() == 2 );
	REQUIRE( a3->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a3->defaultValues().back() == SL( "val2" ) );
}

TEST_CASE( "TestGetterSetterOfArgAsCommandInGroup" )
{
	CmdLine cmd;

	cmd.addAllOfGroup( SL( "group" ) )
		.addArgAsCommand( SL( "arg1" ), false, ValueOptions::OneValue,
		SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgAsCommandWithDefaulValues( SL( "arg2" ), false,
			ValueOptions::ManyValues, SL( "desc" ), SL( "long desc" ),
			{ SL( "val1" ), SL( "val2" ) }, SL( "vs" ) )
		.end();

	auto * a1 = static_cast< ArgAsCommand* > (
		cmd.findArgument( SL( "arg1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val1" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().size() == 2 );
	REQUIRE( a2->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().back() == SL( "val2" ) );
}

TEST_CASE( "TestGetterSetterOfSubCommandInGroup" )
{
	CmdLine cmd;

	cmd.addAllOfGroup( SL( "group" ) )
		.addSubCommand( SL( "arg1" ), false, ValueOptions::OneValue,
		SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addSubCommandWithDefaultValues( SL( "arg2" ), false,
			ValueOptions::ManyValues, SL( "desc" ), SL( "long desc" ),
			{ SL( "val1" ), SL( "val2" ) }, SL( "vs" ) )
		.end();

	auto * a1 = static_cast< ArgAsCommand* > (
		cmd.findArgument( SL( "arg1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val1" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().size() == 2 );
	REQUIRE( a2->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().back() == SL( "val2" ) );
}

TEST_CASE( "TestGetterSetterOfCommand" )
{
	CmdLine cmd;

	cmd.addCommand( SL( "cmd1" ), ValueOptions::OneValue,
			SL( "desc" ), SL( "long desc" ), SL( "val" ),
			SL( "vs" ) )
		.end()
		.addCommandWithDefaultValues( SL( "cmd2" ), ValueOptions::ManyValues,
			SL( "desc" ), SL( "long desc" ), { SL( "val1" ), SL( "val2" ) },
			SL( "vs" ) )
		.end();

	auto * a1 = static_cast< Command* > (
		cmd.findArgument( SL( "cmd1" ) ) );

	REQUIRE( a1->description() == SL( "desc" ) );
	REQUIRE( a1->longDescription() == SL( "long desc" ) );
	REQUIRE( a1->value() == SL( "val" ) );
	REQUIRE( a1->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a1->defaultValue() == SL( "val" ) );

	const CmdLine & tmpCmd = cmd;

	const auto * a2 = static_cast< const Command* > (
		tmpCmd.findArgument( SL( "cmd2" ) ) );

	REQUIRE( a2->description() == SL( "desc" ) );
	REQUIRE( a2->longDescription() == SL( "long desc" ) );
	REQUIRE( a2->value() == SL( "val1" ) );
	REQUIRE( a2->valueSpecifier() == SL( "vs" ) );
	REQUIRE( a2->defaultValue() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().size() == 2 );
	REQUIRE( a2->defaultValues().front() == SL( "val1" ) );
	REQUIRE( a2->defaultValues().back() == SL( "val2" ) );

	REQUIRE( cmd.value( SL( "cmd1" ) ) == SL( "val" ) );
	REQUIRE( cmd.values( SL( "cmd1" ) ).size() == 1 );
	REQUIRE( cmd.values( SL( "cmd1" ) ).front() == SL( "val" ) );

	REQUIRE( cmd.value( SL( "cmd2" ) ) == SL( "val1" ) );
	REQUIRE( cmd.values( SL( "cmd2" ) ).size() == 2 );
	REQUIRE( cmd.values( SL( "cmd2" ) ).front() == SL( "val1" ) );
	REQUIRE( cmd.values( SL( "cmd2" ) ).back() == SL( "val2" ) );
}

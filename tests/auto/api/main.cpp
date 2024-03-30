
/*
	SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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


TEST_CASE( "TestAllIsOk" )
{
	const int argc = 6;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ),
		SL( "-f" ), SL( "test.txt" ),
		SL( "-d" ), SL( "-r" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue, false,
				SL( "Add file." ) )
			.addAllOfGroup( SL( "file group" ) )
				.addArgWithFlagAndName( SL( 'f' ), SL( "file" ), true, false,
					SL( "Name of the file." ), SL( "" ), SL( "" ),
					SL( "fn" ) )
			.end()
			.addArgWithFlagOnly( SL( 'd' ), false, false, SL( "Do job." ) )
		.end()
		.addCommand( SL( "delete" ), ValueOptions::NoValue, false,
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
	const int argc = 6;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ),
		SL( "-f" ), SL( "test.txt" ),
		SL( "-d" ), SL( "-r" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue, false,
				SL( "Add file." ) )
			.addAllOfGroup( SL( "file group" ) )
				.addArgWithFlagAndName( SL( 'f' ), SL( "file" ), true, false,
					SL( "Name of the file." ), SL( "" ), SL( "" ),
					SL( "fn" ) )
			.end()
			.addArgWithFlagOnly( SL( 'd' ), false, false, SL( "Do job." ) )
		.end()
		.addCommand( SL( "delete" ), ValueOptions::NoValue, false,
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
		.addAtLeastOneGroup( SL( "5" ) )
			.addMultiArgWithDefaulValues( SL( '6' ), SL( "66" ) )
			.addMultiArgWithFlagOnlyAndDefaultValues( SL( '7' ) )
			.addMultiArgWithNameOnlyAndDefaultValues( SL( "77" ) )
		.end();

	cmd.parse();

	REQUIRE( cmd.isDefined( SL( "add" ) ) );
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
	const int argc = 6;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ),
		SL( "-f" ), SL( "test.txt" ),
		SL( "-d" ), SL( "-r" ) };

	CmdLine cmd( CmdLine::CommandIsRequired );

	cmd.addCommand( SL( "add" ), ValueOptions::NoValue, false,
				SL( "Add file." ) )
			.addAllOfGroup( SL( "file group" ) )
				.addArgWithFlagAndName( SL( 'f' ), SL( "file" ), true, false,
					SL( "Name of the file." ), SL( "" ), SL( "" ),
					SL( "fn" ) )
			.end()
			.addArgWithFlagOnly( SL( 'd' ), false, false, SL( "Do job." ) )
		.end()
		.addCommand( SL( "delete" ), ValueOptions::NoValue, false,
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

TEST_CASE( "TestGetterSetterOfCommand" )
{
	CmdLine cmd;

	cmd.addCommand( SL( "cmd1" ), ValueOptions::OneValue, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ),
			SL( "vs" ) )
		.end()
		.addCommandWithDefaultValues( SL( "cmd2" ), ValueOptions::ManyValues, false,
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

TEST_CASE( "JustCompileIt" )
{
	CmdLine cmd;

	cmd.addCommandWithDefaultValues( SL( "cmd1" ), ValueOptions::NoValue, false,
		SL( "1" ), SL( "2" ), { SL( "1" ), SL( "2" ) }, SL( "3" ) )
			.addOnlyOneGroup( SL( "group1" ), false ).end()
			.addAllOfGroup( SL( "group2" ), false ).end()
			.addAtLeastOneGroup( SL( "group3" ), false ).end()
			.addArgWithFlagAndName( SL( 'a' ), SL( "flag1" ), true, false, SL( "1" ),
				SL( "2" ), SL( "3" ), SL( "vs" ) )
			.addArgWithFlagOnly( SL( 'b' ), false, false, SL( "1" ), SL( "2" ),
				SL( "3" ), SL( "vs" ) )
			.addArgWithNameOnly( SL( "flag2" ), false, false, SL( "1" ), SL( "2" ),
				SL( "3" ), SL( "vs" ) )
			.addMultiArg( SL( 'c' ), SL( "flag3" ), false, false, SL( "1" ), SL( "2" ),
				SL( "3" ), SL( "vs" ) )
			.addMultiArgWithFlagOnly( SL( 'd' ), false, false, SL( "1" ), SL( "2" ),
				SL( "3" ), SL( "vs" ) )
			.addMultiArgWithNameOnly( SL( "flag4" ), false, false, SL( "1" ), SL( "2" ),
				SL( "3" ), SL( "vs" ) )
			.addMultiArgWithDefaulValues( SL( 'e' ), SL( "flag5" ), false, false, SL( "1" ), SL( "2" ),
				{ SL( "3" ), SL( "4" ) }, SL( "vs" ) )
			.addMultiArgWithFlagOnlyAndDefaultValues( SL( 'f' ), false, false, SL( "1" ), SL( "2" ),
				{ SL( "3" ), SL( "4" ) }, SL( "vs" ) )
			.addMultiArgWithNameOnlyAndDefaultValues( SL( "flag6" ), false, false, SL( "1" ), SL( "2" ),
				{ SL( "3" ), SL( "4" ) }, SL( "vs" ) )
			.addCommand( SL( "cmd2" ), ValueOptions::NoValue, false, SL( "1" ), SL( "2" ),
				SL( "3" ), SL( "vs" ) ).end()
			.addCommandWithDefaultValues( SL( "cmd3" ), ValueOptions::NoValue, false, SL( "1" ), SL( "2" ),
				{ SL( "3" ), SL( "4" ) }, SL( "vs" ) ).end()
		.end();
}

TEST_CASE( "CommandInGroup1" )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "compress" ), SL( "./dir" ) };

	CmdLine cmd( argc, argv );

	cmd.addCommand( "extract", ValueOptions::OneValue,
			false, "Extract file", "", "", "fn" )
		.end()
		.addOnlyOneGroup( "use command or option, not both" )
			.addCommand( "compress", ValueOptions::OneValue,
				false, "Compress file", "", "", "fn" )
			.end()
			.addArgWithFlagOnly( 'c', true, false, "Compress file",
				"Compress file or folder", "", "fn" )
		.end()
		.addHelp( true, argv[ 0 ], "unzip application" );

	cmd.parse();

	REQUIRE( cmd.isDefined( "compress" ) == true );
	REQUIRE( cmd.value( "compress" ) == SL( "./dir" ) );
	REQUIRE( cmd.isDefined( "extract" ) == false );
	REQUIRE( cmd.isDefined( "-c" ) == false );
}

TEST_CASE( "CommandInGroup2" )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-c" ), SL( "./dir" ) };

	CmdLine cmd( argc, argv );

	cmd.addCommand( "extract", ValueOptions::OneValue,
			false, "Extract file", "", "", "fn" )
		.end()
		.addOnlyOneGroup( "use command or option, not both" )
			.addCommand( "compress", ValueOptions::OneValue,
				false, "Compress file", "", "", "fn" )
			.end()
			.addArgWithFlagOnly( 'c', true, false, "Compress file",
				"Compress file or folder", "", "fn" )
		.end()
		.addHelp( true, argv[ 0 ], "unzip application" );

	cmd.parse();

	REQUIRE( cmd.isDefined( "-c" ) == true );
	REQUIRE( cmd.value( "-c" ) == SL( "./dir" ) );
	REQUIRE( cmd.isDefined( "extract" ) == false );
	REQUIRE( cmd.isDefined( "compress" ) == false );
}

TEST_CASE( "CommandInGroup3" )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "extract" ), SL( "1.zip" ) };

	CmdLine cmd( argc, argv );

	cmd.addCommand( "extract", ValueOptions::OneValue,
			false, "Extract file", "Extract file", "", "fn" )
		.end()
		.addOnlyOneGroup( "use command or option, not both" )
			.addCommand( "compress", ValueOptions::OneValue,
				false, "Compress file", "Compress file", "./", "fn" )
			.end()
			.addArgWithFlagOnly( 'c', true, false, "Compress file",
				"Compress file or folder", "", "fn" )
		.end()
		.addHelp( true, argv[ 0 ], "unzip application" );

	cmd.parse();

	REQUIRE( cmd.isDefined( "extract" ) == true );
	REQUIRE( cmd.value( "extract" ) == SL( "1.zip" ) );
	REQUIRE( cmd.isDefined( "-c" ) == false );
	REQUIRE( cmd.isDefined( "compress" ) == false );

	REQUIRE( cmd.value( "compress" ) == SL( "./" ) );
}

TEST_CASE( "CommandInGroup4" )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-c" ), SL( "./dir" ), SL( "compress" ), SL( "./dir" ) };

	CmdLine cmd( argc, argv );

	cmd.addCommand( "extract", ValueOptions::OneValue,
			false, "Extract file", "", "", "fn" )
		.end()
		.addOnlyOneGroup( "use command or option, not both" )
			.addCommand( "compress", ValueOptions::OneValue,
				false, "Compress file", "", "", "fn" )
			.end()
			.addArgWithFlagOnly( 'c', true, false, "Compress file",
				"Compress file or folder", "", "fn" )
		.end()
		.addHelp( true, argv[ 0 ], "unzip application" );

	try {
		cmd.parse();
	}
	catch( const BaseException & x )
	{
		REQUIRE( x.desc() ==
			SL( "Only one argument can be defined in OnlyOne group \"use command or option, "
				"not both\". Whereas defined \"compress\" and \"-c\"." ) );

		return;
	}

	REQUIRE( false );
}

TEST_CASE( "CommandInGroup5" )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "extract" ), SL( "1.zip" ) };

	CmdLine cmd( argc, argv );

	cmd.addCommand( "extract", ValueOptions::OneValue,
			false, "Extract file", "Extract file", "", "fn" )
		.end()
		.addOnlyOneGroup( "use command or option, not both" )
			.addCommandWithDefaultValues( "compress", ValueOptions::OneValue,
				false, "Compress file", "Compress file", { "./", "~/Work" } , "fn" )
			.end()
			.addArgWithFlagOnly( 'c', true, false, "Compress file",
				"Compress file or folder", "", "fn" )
		.end()
		.addHelp( true, argv[ 0 ], "unzip application" );

	cmd.parse();

	REQUIRE( cmd.isDefined( "extract" ) == true );
	REQUIRE( cmd.value( "extract" ) == SL( "1.zip" ) );
	REQUIRE( cmd.isDefined( "-c" ) == false );
	REQUIRE( cmd.isDefined( "compress" ) == false );

	REQUIRE( cmd.value( "compress" ) == SL( "./" ) );
	const auto values = cmd.values( "compress" );
	REQUIRE( values.size() == 2 );
	REQUIRE( values.at( 0 ) == SL( "./" ) );
	REQUIRE( values.at( 1 ) == SL( "~/Work" ) );
}

TEST_CASE( "CommandInGroup6" )
{
	const int argc = 4;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "extract" ), SL( "file" ), SL( "1.zip" ) };

	CmdLine cmd( argc, argv );

	cmd.addOnlyOneGroup( "command or option" )
		.addCommand( "extract", ValueOptions::NoValue,
			true, "Extract file", "Extract file", "", "" )
				.addOnlyOneGroup( "file or folder" )
					.addCommand( "file", ValueOptions::ManyValues, false, "Extract file",
						"Extract file", "", "fn" )
					.end()
					.addCommand( "folder", ValueOptions::ManyValues, false, "Extract folder",
						"Extract folder", "", "dir" )
					.end()
				.end()
		.end()
		.addArgWithFlagOnly( 'e', true, false, "Extract file", "Extract file", "", "fn" )
	.end()
	.addHelp( true, argv[ 0 ], "commands in groups" );

	cmd.parse();

	REQUIRE( cmd.isDefined( "extract" ) == true );
	REQUIRE( cmd.isDefined( "file" ) == true );

	REQUIRE( cmd.value( "file" ) == SL( "1.zip" ) );
	const auto values = cmd.values( "file" );
	REQUIRE( values.size() == 1 );
	REQUIRE( values.at( 0 ) == SL( "1.zip" ) );

	REQUIRE( cmd.isDefined( "folder" ) == false );
	REQUIRE( cmd.isDefined( "-e" ) == false );
}

TEST_CASE( "CommandInGroup7" )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "extract" ) };

	CmdLine cmd( argc, argv );

	cmd.addOnlyOneGroup( "command or option" )
		.addCommand( "extract", ValueOptions::NoValue,
			true, "Extract file", "Extract file", "", "" )
				.addOnlyOneGroup( "file or folder" )
					.addCommand( "file", ValueOptions::ManyValues, false, "Extract file",
						"Extract file", "", "fn" )
					.end()
					.addCommand( "folder", ValueOptions::ManyValues, false, "Extract folder",
						"Extract folder", "", "dir" )
					.end()
				.end()
		.end()
		.addArgWithFlagOnly( 'e', true, false, "Extract file", "Extract file", "", "fn" )
	.end()
	.addHelp( true, argv[ 0 ], "commands in groups" );

	try {
		cmd.parse();
	}
	catch( const BaseException & )
	{
		return;
	}

	REQUIRE( false );
}

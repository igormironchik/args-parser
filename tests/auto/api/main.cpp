
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

#ifdef ARGS_WSTRING_BUILD
	using CHAR = String::value_type;
#else
	using CHAR = char;
#endif


TEST( ArgAPI, TestAllIsOk )
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

	CHECK_CONDITION( cmd.isDefined( SL( "add" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "file" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-f" ) ) )
	CHECK_CONDITION( cmd.value( SL( "-f" ) ) == SL( "test.txt" ) )
	CHECK_CONDITION( cmd.values( SL( "-f" ) ).size() == 1 )
	CHECK_CONDITION( cmd.values( SL( "-f" ) ).front() == SL( "test.txt" ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-d" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-r" ) ) )

	CHECK_CONDITION( !cmd.isDefined( SL( "delete" ) ) )
	CHECK_CONDITION( !cmd.isDefined( SL( "-h" ) ) )
}

TEST( ArgAPI, TestAllIsOkReparse )
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

	CHECK_CONDITION( cmd.isDefined( SL( "add" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "file" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-f" ) ) )
	CHECK_CONDITION( cmd.value( SL( "-f" ) ) == SL( "test.txt" ) )
	CHECK_CONDITION( cmd.values( SL( "-f" ) ).size() == 1 )
	CHECK_CONDITION( cmd.values( SL( "-f" ) ).front() == SL( "test.txt" ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-d" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-r" ) ) )

	CHECK_CONDITION( !cmd.isDefined( SL( "delete" ) ) )
	CHECK_CONDITION( !cmd.isDefined( SL( "-h" ) ) )

	const int nargc = 2;
	const CHAR * nargv[ nargc ] = { SL( "program.exe" ),
		SL( "delete" ) };

	cmd.parse( nargc, nargv );

	CHECK_CONDITION( !cmd.isDefined( SL( "add" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "delete" ) ) )
}

TEST( ArgAPI, TestAllIsOkWithEmptyCtor )
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

	CHECK_CONDITION( cmd.isDefined( SL( "add" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "file" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-f" ) ) )
	CHECK_CONDITION( cmd.value( SL( "-f" ) ) == SL( "test.txt" ) )
	CHECK_CONDITION( cmd.values( SL( "-f" ) ).size() == 1 )
	CHECK_CONDITION( cmd.values( SL( "-f" ) ).front() == SL( "test.txt" ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-d" ) ) )
	CHECK_CONDITION( cmd.isDefined( SL( "-r" ) ) )

	CHECK_CONDITION( !cmd.isDefined( SL( "delete" ) ) )
	CHECK_CONDITION( !cmd.isDefined( SL( "-h" ) ) )
}

TEST( ArgAPI, TestGetterSetterOfArg )
{
	CmdLine cmd;

	cmd.addArgWithFlagAndName( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgWithFlagOnly( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgWithNameOnly( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) );

	auto * a1 = static_cast< Arg* > ( cmd.findArgument( SL( "-1" ) ) );

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const Arg* > ( cmd.findArgument( SL( "-2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val" ) )

	auto * a3 = static_cast< Arg* > ( cmd.findArgument( SL( "--33" ) ) );

	CHECK_CONDITION( a3->description() == SL( "desc" ) )
	CHECK_CONDITION( a3->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a3->value() == SL( "val" ) )
	CHECK_CONDITION( a3->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a3->defaultValue() == SL( "val" ) )
}

TEST( ArgAPI, TestGetterSetterOfMultiArg )
{
	CmdLine cmd;

	cmd.addMultiArg( SL( '1' ), SL( "11" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addMultiArgWithFlagOnly( SL( '2' ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addMultiArgWithNameOnly( SL( "33" ), true, false,
			SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) );

	auto * a1 = static_cast< Arg* > ( cmd.findArgument( SL( "-1" ) ) );

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const Arg* > ( cmd.findArgument( SL( "-2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val" ) )

	auto * a3 = static_cast< Arg* > ( cmd.findArgument( SL( "--33" ) ) );

	CHECK_CONDITION( a3->description() == SL( "desc" ) )
	CHECK_CONDITION( a3->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a3->value() == SL( "val" ) )
	CHECK_CONDITION( a3->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a3->defaultValue() == SL( "val" ) )
}

TEST( ArgAPI, TestGetterSetterOfMultiArgWithValues )
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

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val1" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a1->defaultValues().size() == 2 )
	CHECK_CONDITION( a1->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a1->defaultValues().back() == SL( "val2" ) )

	const auto * a2 = static_cast< const MultiArg* > ( cmd.findArgument( SL( "-2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val1" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().size() == 2 )
	CHECK_CONDITION( a2->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().back() == SL( "val2" ) )

	auto * a3 = static_cast< MultiArg* > ( cmd.findArgument( SL( "--33" ) ) );

	CHECK_CONDITION( a3->description() == SL( "desc" ) )
	CHECK_CONDITION( a3->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a3->value() == SL( "val1" ) )
	CHECK_CONDITION( a3->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a3->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a3->defaultValues().size() == 2 )
	CHECK_CONDITION( a3->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a3->defaultValues().back() == SL( "val2" ) )
}

TEST( ArgAPI, TestGetterSetterOfArgAsCommand )
{
	CmdLine cmd;

	cmd.addArgAsCommand( SL( "arg1" ), false, ValueOptions::OneValue,
		SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addArgAsCommandWithDefaulValues( SL( "arg2" ), false,
			ValueOptions::ManyValues, SL( "desc" ), SL( "long desc" ),
			{ SL( "val1" ), SL( "val2" ) }, SL( "vs" ) );

	auto * a1 = static_cast< ArgAsCommand* > (
		cmd.findArgument( SL( "arg1" ) ) );

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val1" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().size() == 2 )
	CHECK_CONDITION( a2->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().back() == SL( "val2" ) )
}

TEST( ArgAPI, TestGetterSetterOfSubCommand )
{
	CmdLine cmd;

	cmd.addSubCommand( SL( "arg1" ), false, ValueOptions::OneValue,
		SL( "desc" ), SL( "long desc" ), SL( "val" ), SL( "vs" ) )
		.addSubCommandWithDefaultValues( SL( "arg2" ), false,
			ValueOptions::ManyValues, SL( "desc" ), SL( "long desc" ),
			{ SL( "val1" ), SL( "val2" ) }, SL( "vs" ) );

	auto * a1 = static_cast< ArgAsCommand* > (
		cmd.findArgument( SL( "arg1" ) ) );

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val1" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().size() == 2 )
	CHECK_CONDITION( a2->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().back() == SL( "val2" ) )
}

TEST( ArgAPI, TestGetterSetterOfArgInGroup )
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

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const Arg* > ( cmd.findArgument( SL( "-2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val" ) )

	auto * a3 = static_cast< Arg* > ( cmd.findArgument( SL( "--33" ) ) );

	CHECK_CONDITION( a3->description() == SL( "desc" ) )
	CHECK_CONDITION( a3->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a3->value() == SL( "val" ) )
	CHECK_CONDITION( a3->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a3->defaultValue() == SL( "val" ) )
}

TEST( ArgAPI, TestGetterSetterOfMultiArgInGroup )
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

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const Arg* > ( cmd.findArgument( SL( "-2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val" ) )

	auto * a3 = static_cast< Arg* > ( cmd.findArgument( SL( "--33" ) ) );

	CHECK_CONDITION( a3->description() == SL( "desc" ) )
	CHECK_CONDITION( a3->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a3->value() == SL( "val" ) )
	CHECK_CONDITION( a3->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a3->defaultValue() == SL( "val" ) )
}

TEST( ArgAPI, TestGetterSetterOfMultiArgWithValuesInGroup )
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

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val1" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a1->defaultValues().size() == 2 )
	CHECK_CONDITION( a1->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a1->defaultValues().back() == SL( "val2" ) )

	const auto * a2 = static_cast< const MultiArg* > ( cmd.findArgument( SL( "-2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val1" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().size() == 2 )
	CHECK_CONDITION( a2->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().back() == SL( "val2" ) )

	auto * a3 = static_cast< MultiArg* > ( cmd.findArgument( SL( "--33" ) ) );

	CHECK_CONDITION( a3->description() == SL( "desc" ) )
	CHECK_CONDITION( a3->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a3->value() == SL( "val1" ) )
	CHECK_CONDITION( a3->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a3->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a3->defaultValues().size() == 2 )
	CHECK_CONDITION( a3->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a3->defaultValues().back() == SL( "val2" ) )
}

TEST( ArgAPI, TestGetterSetterOfArgAsCommandInGroup )
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

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val1" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().size() == 2 )
	CHECK_CONDITION( a2->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().back() == SL( "val2" ) )
}

TEST( ArgAPI, TestGetterSetterOfSubCommandInGroup )
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

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const ArgAsCommand* > (
		cmd.findArgument( SL( "arg2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val1" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().size() == 2 )
	CHECK_CONDITION( a2->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().back() == SL( "val2" ) )
}

TEST( ArgAPI, TestGetterSetterOfCommand )
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

	CHECK_CONDITION( a1->description() == SL( "desc" ) )
	CHECK_CONDITION( a1->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a1->value() == SL( "val" ) )
	CHECK_CONDITION( a1->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a1->defaultValue() == SL( "val" ) )

	const auto * a2 = static_cast< const Command* > (
		cmd.findArgument( SL( "cmd2" ) ) );

	CHECK_CONDITION( a2->description() == SL( "desc" ) )
	CHECK_CONDITION( a2->longDescription() == SL( "long desc" ) )
	CHECK_CONDITION( a2->value() == SL( "val1" ) )
	CHECK_CONDITION( a2->valueSpecifier() == SL( "vs" ) )
	CHECK_CONDITION( a2->defaultValue() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().size() == 2 )
	CHECK_CONDITION( a2->defaultValues().front() == SL( "val1" ) )
	CHECK_CONDITION( a2->defaultValues().back() == SL( "val2" ) )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

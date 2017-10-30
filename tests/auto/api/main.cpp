
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


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

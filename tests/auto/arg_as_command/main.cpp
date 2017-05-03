
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


TEST( ArgAsCommandCase, TestWithArg )
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

	CHECK_CONDITION( t.isDefined() == true )
	CHECK_CONDITION( t.value() == SL( "100" ) )

	CHECK_CONDITION( p.isDefined() == true )
	CHECK_CONDITION( p.values().size() == 1 )
	CHECK_CONDITION( p.value() == SL( "400" ) )
}

TEST( ArgAsCommandCase, TestManyValues )
{
	const int argc = 6;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "pos" ), SL( "100" ), SL( "200" ), SL( "300" ), SL( "400" ) };

	CmdLine cmd( argc, argv );

	ArgAsCommand p( SL( "pos" ), true, ValueOptions::ManyValues );

	cmd.addArg( p );

	cmd.parse();

	CHECK_CONDITION( p.isDefined() == true )
	CHECK_CONDITION( p.values().size() == 4 )

	auto it = p.values().cbegin();

	CHECK_CONDITION( *it == SL( "100" ) )

	++it;

	CHECK_CONDITION( *it == SL( "200" ) )

	++it;

	CHECK_CONDITION( *it == SL( "300" ) )

	++it;

	CHECK_CONDITION( *it == SL( "400" ) )
}

TEST( ArgAsCommandCase, TestNoValue )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "pos" ) };

	CmdLine cmd( argc, argv );

	ArgAsCommand p( SL( "pos" ), true );

	cmd.addArg( p );

	cmd.parse();

	CHECK_CONDITION( p.isDefined() == true )
	CHECK_CONDITION( p.values().size() == 0 )
	CHECK_CONDITION( p.value().empty() )
}

TEST( ArgAsCommandCase, TestUnderCommand )
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

	CHECK_CONDITION( add.isDefined() == true )

	CHECK_CONDITION( file.isDefined() == true )
	CHECK_CONDITION( file.values().size() == 1 )
	CHECK_CONDITION( file.value() == SL( "out.txt" ) )
}

TEST( ArgAsCommandCase, TestWrongName )
{
	CHECK_THROW( ArgAsCommand( SL( "-pos" ) ), BaseException )
	CHECK_THROW( ArgAsCommand( SL( "--pos" ) ), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandNotDefinedRequired )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandNotDefinedValue )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "file" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandNotDefinedValue2 )
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

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestUnderCommandWithManyValuesAndArg )
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

	CHECK_CONDITION( add.isDefined() == true )

	CHECK_CONDITION( file.isDefined() == true )

	CHECK_CONDITION( file.values().size() == 2 )

	auto it = file.values().cbegin();

	CHECK_CONDITION( *it == SL( "1.txt" ) )

	++it;

	CHECK_CONDITION( *it == SL( "2.txt" ) )

	CHECK_CONDITION( t.isDefined() == true )
}

TEST( ArgAsCommandCase, TestAlreadyDefined )
{
	const int argc = 4;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "file" ), SL( "1.txt" ), SL( "file" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	ArgAsCommand file( SL( "file" ), true, ValueOptions::ManyValues );

	cmd.addArg( file );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( ArgAsCommandCase, TestNameRedefinition )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "add" ), SL( "add" ) };

	CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

	Command add( SL( "add" ) );
	ArgAsCommand file( SL( "add" ), true, ValueOptions::ManyValues );

	add.addArg( file );

	cmd.addArg( add );

	CHECK_THROW( cmd.parse(), BaseException )
}

int main()
{
	RUN_ALL_TESTS()

	return 0;
}

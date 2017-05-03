
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


TEST( GroupCase, TestOnlyOneAllIsOk )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	OnlyOneGroup g( SL( "only_one" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == SL( "100" ) )

	CHECK_CONDITION( port.isDefined() == false )

	CHECK_CONDITION( host.isDefined() == false )
}


TEST( GroupCase, TestOnlyOneFailed )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ),
		SL( "-p" ), SL( "4545" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	OnlyOneGroup g( SL( "only_one" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestOnlyOneWithRequiredFailed )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true, true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	OnlyOneGroup g( SL( "only_one" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestAllOfIsOk )
{
	const int argc = 7;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ), SL( "-p" ), SL( "4545" ),
		SL( "-h" ), SL( "localhost" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	AllOfGroup g( SL( "all_of" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == SL( "100" ) )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == SL( "4545" ) )

	CHECK_CONDITION( host.isDefined() == true )
	CHECK_CONDITION( host.value() == SL( "localhost" ) )
}


TEST( GroupCase, TestAllOfFailed )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ),
		SL( "-p" ), SL( "4545" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	AllOfGroup g( SL( "all_of" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestAllOfWithRequiredFailed )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true, true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	OnlyOneGroup g( SL( "all_of" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}

TEST( GroupCase, TestAtLeasOneIsOk )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ), SL( "-p" ), SL( "4545" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	AtLeastOneGroup g( SL( "at_least_one" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == SL( "100" ) )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == SL( "4545" ) )

	CHECK_CONDITION( host.isDefined() == false )

	CHECK_CONDITION( g.isDefined() == true )
}


TEST( GroupCase, TestAtLeasOneFailed )
{
	const int argc = 1;
	const CHAR * argv[ argc ] = { SL( "program.exe" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	AtLeastOneGroup g( SL( "at_least_one" ) );
	g.setRequired( true );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, TestAtLeasOneWithRequiredFailed )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true, true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	AtLeastOneGroup g( SL( "at_least_one" ) );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


TEST( GroupCase, GroupsIsOk )
{
	const int argc = 9;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ), SL( "-p" ), SL( "4545" ),
		SL( "-h" ), SL( "localhost" ),
		SL( "-s" ), SL( "db" ) };

	CmdLine cmd( argc, argv );

	Arg store( Char( SL( 's' ) ), SL( "store" ), true );
	Arg file( Char( SL( 'f' ) ), SL( "file" ), true );

	OnlyOneGroup onlyOne( SL( "only_one" ) );
	onlyOne.addArg( store );
	onlyOne.addArg( file );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	AllOfGroup g( SL( "all_of" ) );
	g.setRequired( true );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );
	g.addArg( onlyOne );

	cmd.addArg( g );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == SL( "100" ) )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == SL( "4545" ) )

	CHECK_CONDITION( host.isDefined() == true )
	CHECK_CONDITION( host.value() == SL( "localhost" ) )

	CHECK_CONDITION( store.isDefined() == true )
	CHECK_CONDITION( store.value() == SL( "db" ) )

	CHECK_CONDITION( file.isDefined() == false )

	CHECK_CONDITION( g.isDefined() == true )
}


TEST( GroupCase, GroupsFailed )
{
	const int argc = 11;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ), SL( "-p" ), SL( "4545" ),
		SL( "-h" ), SL( "localhost" ),
		SL( "-s" ), SL( "db" ), SL( "-f" ), SL( "out.txt" ) };

	CmdLine cmd( argc, argv );

	Arg store( Char( SL( 's' ) ), SL( "store" ), true );
	Arg file( Char( SL( 'f' ) ), SL( "file" ), true );

	OnlyOneGroup onlyOne( SL( "only_one" ) );
	onlyOne.addArg( store );
	onlyOne.addArg( file );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	AllOfGroup g( SL( "all_of" ) );
	g.setRequired( true );
	g.addArg( timeout );
	g.addArg( port );
	g.addArg( host );
	g.addArg( onlyOne );

	cmd.addArg( g );

	CHECK_THROW( cmd.parse(), BaseException )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

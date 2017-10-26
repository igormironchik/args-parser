
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


TEST( ArgTestCase, TestAllIsOk )
{
	const int argc = 6;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t=100" ),
		SL( "--port" ), SL( "4545" ),
		SL( "--host" ), SL( "any" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true );
	Arg d( SL( "default" ), false, true );
	d.setDefaultValue( SL( "default" ) );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	cmd.addArg( &timeout );
	cmd.addArg( &port );
	cmd.addArg( &host );

	cmd.parse();

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value() == SL( "100" ) )

	CHECK_CONDITION( port.isDefined() == true )
	CHECK_CONDITION( port.value() == SL( "4545" ) )

	CHECK_CONDITION( host.isDefined() == true )
	CHECK_CONDITION( host.value() == SL( "any" ) )

	CHECK_CONDITION( d.isDefined() == false )
	CHECK_CONDITION( d.value() == SL( "default" ) )
}

TEST( ArgTestCase, TestUndefinedArg )
{
	const int argc = 7;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ),
		SL( "--port" ), SL( "4545" ),
		SL( "--host" ), SL( "any" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg host( String( SL( "host" ) ), true );

	cmd.addArg( &timeout );
	cmd.addArg( &host );

	try {
		cmd.parse();
	}
	catch( const BaseException & x )
	{
		CHECK_CONDITION(
			x.desc() == String( SL( "Unknown argument \"--port\"." ) ) )

		return;
	}

	CHECK_CONDITION( false )
}

TEST( ArgTestCase, TestUndefinedRequiredArg )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-t" ), SL( "100" ),
		SL( "--host" ), SL( "any" ) };

	CmdLine cmd( argc, argv );

	Arg timeout( SL( 't' ), String( SL( "timeout" ) ), true );
	Arg port( SL( 'p' ), String( SL( "port" ) ), true, true );
	Arg host( SL( 'h' ), String( SL( "host" ) ), true );

	cmd.addArg( &timeout );
	cmd.addArg( &port );
	cmd.addArg( &host );

	try {
		cmd.parse();
	}
	catch( const BaseException & x )
	{
		CHECK_CONDITION(
			x.desc() == String( SL( "Undefined required argument \"--port\"." ) ) )

		return;
	}

	CHECK_CONDITION( false )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

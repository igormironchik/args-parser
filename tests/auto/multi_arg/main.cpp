
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


TEST( MultiArgTestCase, TestAllIsOk )
{
	const int argc = 10;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-m" ), SL( "100" ),
		SL( "-m" ), SL( "200" ),
		SL( "-m" ), SL( "300" ),
		SL( "-m" ), SL( "400" ), SL( "500" ) };

	CmdLine cmd( argc, argv );

	MultiArg multi( SL( 'm' ), String( SL( "multi" ) ), true );

	cmd.addArg( &multi );

	cmd.parse();

	CHECK_CONDITION( multi.isDefined() == true )

	const StringList values = multi.values();

	CHECK_CONDITION( multi.count() == 5 )
	CHECK_CONDITION( values.size() == 5 )

	CHECK_CONDITION( multi.value() == SL( "100" ) )

	auto it = values.begin();

	CHECK_CONDITION( *it == SL( "100" ) )
	++it;

	CHECK_CONDITION( *it == SL( "200" ) )
	++it;

	CHECK_CONDITION( *it == SL( "300" ) )
	++it;

	CHECK_CONDITION( *it == SL( "400" ) )
	++it;

	CHECK_CONDITION( *it == SL( "500" ) )
}

TEST( MultiArgTestCase, TestAllIsOk2 )
{
	const int argc = 11;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-m" ), SL( "100" ),
		SL( "-m" ), SL( "200" ),
		SL( "-m" ), SL( "300" ),
		SL( "-m" ), SL( "400" ), SL( "500" ),
		SL( "-t" ) };

	CmdLine cmd( argc, argv );

	MultiArg multi( SL( 'm' ), String( SL( "multi" ) ), true );
	Arg timeout( SL( 't' ), String( SL( "timeout" ) ) );

	cmd.addArg( &multi );
	cmd.addArg( &timeout );

	cmd.parse();

	CHECK_CONDITION( multi.isDefined() == true )

	const StringList values = multi.values();

	CHECK_CONDITION( multi.count() == 5 )
	CHECK_CONDITION( values.size() == 5 )

	CHECK_CONDITION( multi.value() == SL( "100" ) )

	auto it = values.begin();

	CHECK_CONDITION( *it == SL( "100" ) )
	++it;

	CHECK_CONDITION( *it == SL( "200" ) )
	++it;

	CHECK_CONDITION( *it == SL( "300" ) )
	++it;

	CHECK_CONDITION( *it == SL( "400" ) )
	++it;

	CHECK_CONDITION( *it == SL( "500" ) )

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value().empty() )
}

TEST( MultiArgTestCase, TestAllIsOk3 )
{
	const int argc = 5;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-m" ),	SL( "-m" ),
		SL( "-m" ), SL( "-m" ) };

	CmdLine cmd( argc, argv );

	MultiArg multi( SL( 'm' ), String( SL( "multi" ) ), false, true );

	cmd.addArg( &multi );

	cmd.parse();

	CHECK_CONDITION( multi.isDefined() == true )
	CHECK_CONDITION( multi.count() == 4 )
}

TEST( MultiArgTestCase, NotDefinedValue )
{
	const int argc = 8;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-m" ), SL( "100" ),
		SL( "-m" ), SL( "200" ),
		SL( "-m" ), SL( "300" ),
		SL( "-m" ) };

	CmdLine cmd( argc, argv );

	MultiArg multi( SL( 'm' ), String( SL( "multi" ) ), true );

	cmd.addArg( &multi );

	try {
		cmd.parse();
	}
	catch( const BaseException & x )
	{
		CHECK_CONDITION( x.desc() == String( SL( "Argument \"--multi"
			"\" requires value that wasn't presented." ) ) )

		return;
	}

	CHECK_CONDITION( false )
}

TEST( MultiArgTestCase, NotDefinedValue2 )
{
	const int argc = 9;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-m" ), SL( "100" ),
		SL( "-m" ), SL( "200" ),
		SL( "-m" ), SL( "300" ),
		SL( "-m" ),
		SL( "-t" ) };

	CmdLine cmd( argc, argv );

	MultiArg multi( SL( 'm' ), String( SL( "multi" ) ), true );
	Arg timeout( SL( 't' ), String( SL( "timeout" ) ) );

	cmd.addArg( &multi );
	cmd.addArg( &timeout );

	try {
		cmd.parse();
	}
	catch( const BaseException & x )
	{
		CHECK_CONDITION( x.desc() == String( SL( "Argument \"--multi"
			"\" requires value that wasn't presented." ) ) )

		return;
	}

	CHECK_CONDITION( false )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

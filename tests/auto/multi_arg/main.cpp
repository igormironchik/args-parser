
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


TEST( MultiArgTestCase, TestAllIsOk )
{
	const int argc = 10;
	const char * argv[ argc ] = { "program.exe",
		"-m", "100",
		"-m", "200",
		"-m", "300",
		"-m", "400", "500" };

	CmdLine cmd( argc, argv );

	MultiArg multi( 'm', std::string( "multi" ), true );

	cmd.addArg( &multi );

	cmd.parse();

	CHECK_CONDITION( multi.isDefined() == true )

	const std::list< std::string > values = multi.values();

	CHECK_CONDITION( multi.count() == 5 )
	CHECK_CONDITION( values.size() == 5 )

	CHECK_CONDITION( multi.value() == "100" )

	auto it = values.begin();

	CHECK_CONDITION( *it == "100" )
	++it;

	CHECK_CONDITION( *it == "200" )
	++it;

	CHECK_CONDITION( *it == "300" )
	++it;

	CHECK_CONDITION( *it == "400" )
	++it;

	CHECK_CONDITION( *it == "500" )
}

TEST( MultiArgTestCase, TestAllIsOk2 )
{
	const int argc = 11;
	const char * argv[ argc ] = { "program.exe",
		"-m", "100",
		"-m", "200",
		"-m", "300",
		"-m", "400", "500",
		"-t" };

	CmdLine cmd( argc, argv );

	MultiArg multi( 'm', std::string( "multi" ), true );
	Arg timeout( 't', std::string( "timeout" ) );

	cmd.addArg( &multi );
	cmd.addArg( &timeout );

	cmd.parse();

	CHECK_CONDITION( multi.isDefined() == true )

	const std::list< std::string > values = multi.values();

	CHECK_CONDITION( multi.count() == 5 )
	CHECK_CONDITION( values.size() == 5 )

	CHECK_CONDITION( multi.value() == "100" )

	auto it = values.begin();

	CHECK_CONDITION( *it == "100" )
	++it;

	CHECK_CONDITION( *it == "200" )
	++it;

	CHECK_CONDITION( *it == "300" )
	++it;

	CHECK_CONDITION( *it == "400" )
	++it;

	CHECK_CONDITION( *it == "500" )

	CHECK_CONDITION( timeout.isDefined() == true )
	CHECK_CONDITION( timeout.value().empty() )
}

TEST( MultiArgTestCase, NotDefinedValue )
{
	const int argc = 8;
	const char * argv[ argc ] = { "program.exe",
		"-m", "100",
		"-m", "200",
		"-m", "300",
		"-m" };

	CmdLine cmd( argc, argv );

	MultiArg multi( 'm', std::string( "multi" ), true );

	cmd.addArg( &multi );

	try {
		cmd.parse();
	}
	catch( const BaseException & x )
	{
		CHECK_CONDITION( x.what() == std::string( "Argument \"--multi"
			"\" require value that wasn't presented." ) )

		return;
	}

	CHECK_CONDITION( false )
}

TEST( MultiArgTestCase, NotDefinedValue2 )
{
	const int argc = 9;
	const char * argv[ argc ] = { "program.exe",
		"-m", "100",
		"-m", "200",
		"-m", "300",
		"-m",
		"-t" };

	CmdLine cmd( argc, argv );

	MultiArg multi( 'm', std::string( "multi" ), true );
	Arg timeout( 't', std::string( "timeout" ) );

	cmd.addArg( &multi );
	cmd.addArg( &timeout );

	try {
		cmd.parse();
	}
	catch( const BaseException & x )
	{
		CHECK_CONDITION( x.what() == std::string( "Argument \"--multi"
			"\" require value that wasn't presented." ) )

		return;
	}

	CHECK_CONDITION( false )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

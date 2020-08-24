
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


TEST_CASE( "DisallowedFlags" )
{
	REQUIRE( details::isCorrectFlag( SL( "-" ) ) == false );
	REQUIRE( details::isCorrectFlag( SL( " " ) ) == false );
	REQUIRE( details::isCorrectFlag( SL( "=" ) ) == false );
	REQUIRE( details::isCorrectFlag( SL( "" ) ) == false );
}

TEST_CASE( "AllowedFlags" )
{
	REQUIRE( details::isCorrectFlag( SL( "a" ) ) == true );
	REQUIRE( details::isCorrectFlag( SL( "1" ) ) == true );
}

TEST_CASE( "DisallowedNames" )
{
	REQUIRE( details::isCorrectName( SL( "time out" ) ) == false );
	REQUIRE( details::isCorrectName( SL( "" ) ) == false );
}

TEST_CASE( "AllowedNames" )
{
	REQUIRE( details::isCorrectName( SL( "timeout" ) ) == true );
	REQUIRE( details::isCorrectName( SL( "time-out" ) ) == true );
	REQUIRE( details::isCorrectName( SL( "time_out" ) ) == true );
}

TEST_CASE( "IsFlagOrArgument" )
{
	REQUIRE( details::isFlag( SL( "-a" ) ) == true );
	REQUIRE( details::isFlag( SL( "--timeout" ) ) == false );
	REQUIRE( details::isArgument( SL( "-a" ) ) == false );
	REQUIRE( details::isArgument( SL( "--timeout" ) ) == true );
}


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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest.h>

// Args include.
#include <Args/all.hpp>

// C++ include.
#include <utility>


using namespace Args;


TEST_CASE( "TestContext" )
{
	ContextInternal internal;

	internal.push_back( SL( "-a" ) );
	internal.push_back( SL( "--timeout" ) );
	internal.push_back( SL( "100" ) );
	internal.push_back( SL( "--port" ) );
	internal.push_back( SL( "4545" ) );
	internal.push_back( SL( "--host" ) );
	internal.push_back( SL( "localhost" ) );

	Context ctx( std::move( internal ) );

	REQUIRE( *ctx.next() == SL( "-a" ) );

	ctx.prepend( SL( "value" ) );

	REQUIRE( *ctx.next() == SL( "value" ) );

	ctx.putBack();

	REQUIRE( *ctx.next() == SL( "value" ) );
	REQUIRE( *ctx.next() == SL( "--timeout" ) );
	REQUIRE( *ctx.next() == SL( "100" ) );
	REQUIRE( *ctx.next() == SL( "--port" ) );
	REQUIRE( *ctx.next() == SL( "4545" ) );
	REQUIRE( *ctx.next() == SL( "--host" ) );

	REQUIRE( ctx.atEnd() == false );

	REQUIRE( *ctx.next() == SL( "localhost" ) );

	REQUIRE( ctx.atEnd() == true );
}

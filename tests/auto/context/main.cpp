
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013 Igor Mironchik

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
#include <Args/context.hpp>

// C++ include.
#include <utility>


using namespace Args;


TEST( ContextTestCase, TestContext )
{
	ContextInternal internal;

	internal.push_back( "-a" );
	internal.push_back( "--timeout" );
	internal.push_back( "100" );
	internal.push_back( "--port" );
	internal.push_back( "4545" );
	internal.push_back( "--host" );
	internal.push_back( "localhost" );

	Context ctx( std::move( internal ) );

	CHECK_CONDITION( *ctx.next() == "-a" )

	ctx.prepend( "value" );

	CHECK_CONDITION( *ctx.next() == "value" )

	ctx.putBack();

	CHECK_CONDITION( *ctx.next() == "value" )
	CHECK_CONDITION( *ctx.next() == "--timeout" )
	CHECK_CONDITION( *ctx.next() == "100" )
	CHECK_CONDITION( *ctx.next() == "--port" )
	CHECK_CONDITION( *ctx.next() == "4545" )
	CHECK_CONDITION( *ctx.next() == "--host" )

	CHECK_CONDITION( ctx.atEnd() == false )

	CHECK_CONDITION( *ctx.next() == "localhost" )

	CHECK_CONDITION( ctx.atEnd() == true )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}

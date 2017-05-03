
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


TEST( Flags, DissallowedFlags )
{
	CHECK_CONDITION( isCorrectFlag( "-" ) == false )
	CHECK_CONDITION( isCorrectFlag( " " ) == false )
	CHECK_CONDITION( isCorrectFlag( "=" ) == false )
	CHECK_CONDITION( isCorrectFlag( "" ) == false )
}

TEST( Flags, AllowedFlags )
{
	CHECK_CONDITION( isCorrectFlag( "a" ) == true )
	CHECK_CONDITION( isCorrectFlag( "1" ) == true )
}

TEST( Names, DissallowedNames )
{
	CHECK_CONDITION( isCorrectName( "time out" ) == false )
	CHECK_CONDITION( isCorrectName( "" ) == false )
}

TEST( Names, AllowedNames )
{
	CHECK_CONDITION( isCorrectName( "timeout" ) == true )
	CHECK_CONDITION( isCorrectName( "time-out" ) == true )
	CHECK_CONDITION( isCorrectName( "time_out" ) == true )
}

TEST( FlagAndArgument, IsFlagOrArgument )
{
	CHECK_CONDITION( isFlag( "-a" ) == true )
	CHECK_CONDITION( isFlag( "--timeout" ) == false )
	CHECK_CONDITION( isArgument( "-a" ) == false )
	CHECK_CONDITION( isArgument( "--timeout" ) == true )
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}


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

// Args include.
#include <Args/utils.hpp>

// C++ include.
#include <regex>


namespace Args {

//
// isArgument
//

bool
isArgument( const std::string & word )
{
	const size_t it = word.find( "--" );

	if( it == 0 )
		return true;
	else
		return false;
} // isArgument


//
// isFlag
//

bool
isFlag( const std::string & word )
{
	if( !isArgument( word ) )
	{
		const size_t it = word.find( '-' );

		if( it == 0 )
			return true;
	}

	return false;
} // isFlag


//
// isCorrectFlag
//

bool
isCorrectFlag( const std::string & flag )
{
	if( flag.empty() || flag.length() > 1 )
		return false;

	static const std::string availableSymbols( "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" );

	if( availableSymbols.find( flag ) == std::string::npos )
		return false;

	return true;
} // isCorrectFlag


//
// isCorrectName
//

bool
isCorrectName( const std::string & name )
{
	if( name.empty() )
		return false;

	std::regex r( "\\s" );

	if( std::regex_search( name, r ) )
		return false;

	static const std::string availableSymbols( "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ-_" );

	for( const char & c : name )
	{
		if( availableSymbols.find( c ) == std::string::npos )
			return false;
	}

	return true;
} // isCorrectName

} /* namespace Args */

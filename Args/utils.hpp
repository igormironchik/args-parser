
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

#ifndef ARGS__UTILS_HPP__INCLUDED
#define ARGS__UTILS_HPP__INCLUDED

#ifndef ARGS_QSTRING_BUILD

// C++ include.
#include <regex>

#else

// Qt include.
#include <QRegExp>

#endif

// Args include.
#include "types.hpp"

// C++ include.
#include <algorithm>
#include <type_traits>


namespace Args {


//
// DISABLE_COPY
//

//! Macro for disabling copy.
#define DISABLE_COPY( Class ) \
	Class( const Class & ) = delete; \
	Class & operator= ( const Class & ) = delete;


//
// UNUSED
//

//! Macro to supress warning about unused argument.
#define UNUSED( Var ) (void)Var;


namespace details {

//
// asConst
//

//! Adds const to non-const objects.
template< typename T >
constexpr typename std::add_const< T >::type &
asConst( T & t ) noexcept
{
	return t;
}

template < typename T >
void asConst( const T && ) = delete;


//
// isArgument
//

//! \return Is word an argument?
static inline bool
isArgument( const String & word )
{
	return ( word.find( SL( "--" ) ) == 0 );
} // isArgument


//
// isFlag
//

//! \return Is word a flag?
static inline bool
isFlag( const String & word )
{
	if( !isArgument( word ) )
	{
		if( word.find( SL( '-' ) ) == 0 )
			return true;
	}

	return false;
} // isFlag


//
// isCorrectFlag
//

//! \return Is flag correct?
static inline bool
isCorrectFlag( const String & flag )
{
	if( flag.empty() || flag.length() > 1 )
		return false;

	static const String availableSymbols( SL( "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" ) );

	return ( availableSymbols.find( flag ) != String::npos );
} // isCorrectFlag


//
// isCorrectName
//

//! \return Is name correct?
static inline bool
isCorrectName( const String & name )
{
	if( name.empty() )
		return false;

#ifndef ARGS_QSTRING_BUILD

#ifdef ARGS_WSTRING_BUILD
	std::wregex r( L"\\s" );
#else
	std::regex r( "\\s" );
#endif

	if( std::regex_search( name, r ) )
		return false;
#else
	QRegExp r( "\\s" );

	if( r.indexIn( name ) != -1 )
		return false;
#endif

	static const String availableSymbols( SL( "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ-_" ) );

	for( const Char & c : asConst( name ) )
	{
		if( availableSymbols.find( c ) == String::npos )
			return false;
	}

	return true;
} // isCorrectName


//
// isMisspelledName
//

//! \return Is the given name a misspelling of correct name.
bool isMisspelledName( const String & misspelled,
	const String & correct )
{
	if( !misspelled.empty() && !correct.empty() )
	{
		String ms = misspelled;
		String cs = correct;

		std::sort( ms.begin(), ms.end() );
		std::sort( cs.begin(), cs.end() );

		return ( ms == cs );
	}
	else
		return false;
}

} /* namespace details */

} /* namespace Args */

#endif // ARGS__UTILS_HPP__INCLUDED

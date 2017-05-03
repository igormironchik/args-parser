
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

#ifndef ARGS__TYPES_HPP__INCLUDED
#define ARGS__TYPES_HPP__INCLUDED

#ifdef ARGS_WSTRING_BUILD

// C++ include.
#include <string>
#include <list>
#include <iostream>

#elif defined( ARGS_QSTRING_BUILD )

#else

// C++ include.
#include <string>
#include <list>
#include <iostream>

#endif


namespace Args {

#ifdef ARGS_WSTRING_BUILD

//! String type.
using String = std::wstring;

//! List of strings.
using StringList = std::list< String >;

//! Out stream type.
using OutStreamType = std::wostream;

//! Output stream.
static OutStreamType & outStream()
{
	return std::wcout;
}

#elif defined( ARGS_QSTRING_BUILD )

#else

//! String type.
using String = std::string;

//! List of strings.
using StringList = std::list< String >;

//! Out stream type.
using OutStreamType = std::ostream;

//! Output stream.
static OutStreamType & outStream()
{
	return std::cout;
}

#endif

} /* namespace Args */

#endif // ARGS__TYPES_HPP__INCLUDED

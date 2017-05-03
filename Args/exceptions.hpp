
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

#ifndef ARGS__EXCEPTIONS_HPP__INCLUDED
#define ARGS__EXCEPTIONS_HPP__INCLUDED

// C++ include.
#include <stdexcept>

// Args include.
#include "types.hpp"


namespace Args {

//
// BaseException
//

//! Base exception of the library.
class BaseException
	:	public std::logic_error
{
public:	
	explicit BaseException::BaseException( const String & what )
#ifdef ARGS_QSTRING_BUILD
		:	std::logic_error( ( (QString) what ).toLocal8Bit().toStdString() )
		,	m_what( what )
#elif defined( ARGS_WSTRING_BUILD )
		:	std::logic_error( "Please use desc() method to get exception "
				"description when compiling with std::wstring." )
		,	m_what( what )
#else
		:	std::logic_error( what )
#endif
	{
	}

	virtual BaseException::~BaseException() noexcept
	{
	}

#ifdef ARGS_WSTRING_BUILD
	//! \return What as std::wstring.
	const String & desc() const noexcept
	{
		return m_what;
	}

private:
	String m_what;
#elif defined( ARGS_QSTRING_BUILD )
	//! \return What as QString.
	const String & desc() const noexcept
	{
		return m_what;
	}

private:
	String m_what;
#else
	//! \return What?
	String desc() const
	{
		return what();
	}
#endif
}; // class BaseException


//
// HelpHasBeenPrintedException
//

//! This exception notifies about that help has been printed.
class HelpHasBeenPrintedException final
	:	public BaseException
{
public:
	HelpHasBeenPrintedException()
		:	BaseException( SL( "Help has been printed." ) )
	{
	}
}; // class HelpHasBeenPrintedException

} /* namespace Args */

#endif // ARGS__EXCEPTIONS_HPP__INCLUDED

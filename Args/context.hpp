
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

#ifndef ARGS__CONTEXT_HPP__INCLUDED
#define ARGS__CONTEXT_HPP__INCLUDED

// C++ include.
#include <utility>

// Args include.
#include "utils.hpp"
#include "types.hpp"


namespace Args {

//
// ContextInternal
//

//! What Context actually is.
typedef StringList ContextInternal;


//
// Context
//

/*!
	Context is a list of words in the command line that user
	presented with interface for interacting with them.
*/
class Context final {
public:
	Context()
		:	m_it( m_context.begin() )
	{
	}

	explicit Context( ContextInternal items );

	Context & operator = ( ContextInternal && items )
	{
		m_context = std::move( items );

		m_it = m_context.begin();

		return *this;
	}

	//! \return Iterator to the first item in the context.
	ContextInternal::iterator begin();

	//! \return Iterator to the last item in the context.
	ContextInternal::iterator end();

	//! \return Is context at end?
	bool atEnd();

	//! \return Iterator to the next item in the context.
	ContextInternal::iterator next();

	//! Put back last taken item.
	void putBack();

	//! Prepend context with new item.
	void prepend( const String & what );

private:
	DISABLE_COPY( Context )

	//! Internal representation of the context.
	ContextInternal m_context;
	//! Iterator to the current item in the context.
	ContextInternal::iterator m_it;
}; // class Context


//
// Context
//

inline
Context::Context( ContextInternal items )
	:	m_context( std::move( items ) )
	,	m_it( m_context.begin() )
{
}

inline ContextInternal::iterator
Context::begin()
{
	return m_it;
}

inline ContextInternal::iterator
Context::end()
{
	return m_context.end();
}

inline bool
Context::atEnd()
{
	return ( begin() == end() );
}

inline ContextInternal::iterator
Context::next()
{
	if( atEnd() )
		return end();
	else
		return m_it++;
}

inline void
Context::putBack()
{
	if( begin() == m_context.begin() )
		return;
	else
		--m_it;
}

inline void
Context::prepend( const String & what )
{
	m_it = m_context.insert( m_it, what );
}

} /* namespace Args */

#endif // ARGS__CONTEXT_HPP__INCLUDED

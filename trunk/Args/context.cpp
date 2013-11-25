
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
#include <Args/context.hpp>


namespace Args {

//
// Context
//

Context::Context( ContextInternal && items )
	:	m_context( items )
	,	m_it( m_context.begin() )
{
}

ContextInternal::iterator
Context::begin()
{
	return m_it;
}

ContextInternal::iterator
Context::end()
{
	return m_context.end();
}

bool
Context::atEnd()
{
	return ( begin() == end() );
}

ContextInternal::iterator
Context::next()
{
	if( atEnd() )
		return end();
	else
		return m_it++;
}

void
Context::putBack()
{
	if( begin() == m_context.begin() )
		return;
	else
		--m_it;
}

void
Context::prepend( const std::string & what )
{
	m_it = m_context.insert( m_it, what );
}

} /* namespace Args */

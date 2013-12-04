
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

#ifndef ARGS__CONTEXT_HPP__INCLUDED
#define ARGS__CONTEXT_HPP__INCLUDED

// C++ include.
#include <list>
#include <string>

// Args include.
#include <Args/utils.hpp>


namespace Args {

//
// ContextInternal
//

//! What Context actually is.
typedef std::list< std::string > ContextInternal;


//
// Context
//

/*!
	Context is a list of words in the command line that user
	presented with interface for interacting with them.
*/
class Context final {
public:
	explicit Context( const ContextInternal & items );

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
	void prepend( const std::string & what );

private:
	DISABLE_COPY( Context )

	//! Internal representation of the context.
	ContextInternal m_context;
	//! Iterator to the current item in the context.
	ContextInternal::iterator m_it;
}; // class Context

} /* namespace Args */

#endif // ARGS__CONTEXT_HPP__INCLUDED

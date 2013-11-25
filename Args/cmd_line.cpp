
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
#include <Args/cmd_line.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <utility>
#include <algorithm>


namespace Args {

//
// makeContext
//

//! Make context from the argc and argv.
ContextInternal && makeContext( int argc, char ** argv )
{
	ContextInternal context;

	// We skip first argv because of it's executable name.
	for( int i = 1; i < argc; ++i )
		context.push_back( argv[ i ] );

	return std::move( context );
} // makeContext


//
// CmdLine
//

CmdLine::CmdLine( int argc, char ** argv )
	:	m_context( makeContext( argc, argv ) )
{
}

void
CmdLine::addArg( ArgIface * arg )
{
	if( std::find( m_args.begin(), m_args.end(), arg ) == m_args.end() )
		m_args.push_back( arg );
	else
		throw BaseException( std::string( "Argument \"" ) +
			arg->name() + "\" already in the command line parser." );
}

void
CmdLine::parse()
{

}

} /* namespace Args */

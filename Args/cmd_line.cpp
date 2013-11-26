
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
	checkCorrectnessBeforeParsing();

	while( !m_context.atEnd() )
	{
		std::string word = *m_context.next();

		const size_t eqIt = word.find( '=' );

		if( eqIt != std::string::npos )
		{
			const std::string value = word.substr( eqIt + 1 );

			if( !value.empty() )
				m_context.prepend( value );

			word = word.substr( 0, eqIt );
		}

		if( isArgument( word ) )
			findArgument( word )->process( m_context );
		else if( isFlag( word ) )
		{
			for( size_t i = 1, length = word.length(); i < length; ++i )
			{
				const std::string flag = std::string( "-" ) + word[ i ];

				ArgIface * arg = findArgument( flag );

				if( i < length - 1 && arg->isWithValue() )
					throw BaseException( std::string( "Only last argument in "
						"flags combo can have value. Flags combo is\"" ) +
						word + "\"." );
				else
					arg->process( m_context );
			}
		}
		else
			throw BaseException( std::string( "Unknown argument \"" ) +
				word + "\"." );
	}

	checkCorrectnessAfterParsing();
}

void
CmdLine::checkCorrectnessBeforeParsing() const
{
	std::list< char > flags;
	std::list< std::string > names;

	std::for_each( m_args.begin(), m_args.end(),
		[ &flags, &names ] ( ArgIface * arg )
			{ arg->checkCorrectnessBeforeParsing( flags, names ); }
	);
}

void
CmdLine::checkCorrectnessAfterParsing() const
{
	std::for_each( m_args.begin(), m_args.end(),
		[] ( ArgIface * arg )
			{ arg->checkCorrectnessAfterParsing(); }
	);
}

ArgIface *
CmdLine::findArgument( const std::string & name )
{
	std::list< ArgIface* >::iterator it = std::find_if( m_args.begin(),
		m_args.end(), [ &name ] ( ArgIface * arg ) -> bool
			{ return ( arg->isItYou( name ) != nullptr ); } );

	if( it != m_args.end() )
		return (*it)->isItYou( name );
	else
		throw BaseException( std::string( "Unknown argument \"" ) +
			name + "\"." );
}

} /* namespace Args */


/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013-2014 Igor Mironchik

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

#ifndef ARGS__CMD_LINE_HPP__INCLUDED
#define ARGS__CMD_LINE_HPP__INCLUDED

// Args include.
#include <Args/utils.hpp>
#include <Args/context.hpp>
#include <Args/arg_iface.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <list>
#include <algorithm>


namespace Args {

//
// CmdLine
//

/*!
	CmdLine is class that holds all rguments and parse
	command line arguments in the correspondence with holded
	arguments.
*/
class CmdLine final {
public:
	CmdLine( int argc, char ** argv );

	//! Add argument.
	void addArg( ArgIface * arg );

	//! Parse arguments.
	void parse();

	//! \return Argument for the given name.
	ArgIface * findArgument( const std::string & name );

	//! \return All arguments.
	const std::list< ArgIface* > & arguments() const;

private:
	//! Check correctness of the arguments before parsing.
	void checkCorrectnessBeforeParsing() const;
	//! Check correctness of the arguments after parsing.
	void checkCorrectnessAfterParsing() const;

private:
	DISABLE_COPY( CmdLine )

	// Context.
	Context m_context;
	//! Arguments.
	std::list< ArgIface* > m_args;
}; // class CmdLine


//
// makeContext
//

//! Make context from the argc and argv.
static inline ContextInternal
makeContext( int argc, char ** argv )
{
	ContextInternal context;

	// We skip first argv because of it's executable name.
	for( int i = 1; i < argc; ++i )
		context.push_back( argv[ i ] );

	return context;
} // makeContext


//
// CmdLine
//

inline
CmdLine::CmdLine( int argc, char ** argv )
	:	m_context( makeContext( argc, argv ) )
{
}

inline void
CmdLine::addArg( ArgIface * arg )
{
	if( arg )
	{
		if( std::find( m_args.begin(), m_args.end(), arg ) == m_args.end() )
			m_args.push_back( arg );
		else
			throw BaseException( std::string( "Argument \"" ) +
				arg->name() + "\" already in the command line parser." );
	}
	else
		throw BaseException( std::string( "Attempt to add nullptr to the "
			"command line as argument." ) );
}

inline void
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

inline const std::list< ArgIface* > &
CmdLine::arguments() const
{
	return m_args;
}

inline void
CmdLine::checkCorrectnessBeforeParsing() const
{
	std::list< std::string > flags;
	std::list< std::string > names;

	std::for_each( m_args.begin(), m_args.end(),
		[ &flags, &names ] ( ArgIface * arg )
			{ arg->checkCorrectnessBeforeParsing( flags, names ); }
	);
}

inline void
CmdLine::checkCorrectnessAfterParsing() const
{
	std::for_each( m_args.begin(), m_args.end(),
		[] ( ArgIface * arg )
			{ arg->checkCorrectnessAfterParsing(); }
	);
}

inline ArgIface *
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

#endif // ARGS__CMD_LINE_HPP__INCLUDED

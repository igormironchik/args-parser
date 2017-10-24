
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

#ifndef ARGS__CMD_LINE_HPP__INCLUDED
#define ARGS__CMD_LINE_HPP__INCLUDED

// Args include.
#include "utils.hpp"
#include "context.hpp"
#include "arg_iface.hpp"
#include "exceptions.hpp"
#include "command.hpp"
#include "types.hpp"

// C++ include.
#include <vector>
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
	//! Command line options.
	enum CmdLineOpts {
		//! No special options.
		Empty = 0,
		//! Command should be defined.
		CommandIsRequired = 1
	}; // enum CmdLineOpts

#ifdef ARGS_WSTRING_BUILD
	CmdLine( int argc, const Char * const * argv,
		CmdLineOpts opt = Empty );
#else
	CmdLine( int argc, const char * const * argv,
		CmdLineOpts opt = Empty );
#endif

	//! Add argument. \note Developer should handle lifetime of the argument.
	void addArg( ArgIface * arg );

	//! Add argument. \note Developer should handle lifetime of the argument.
	void addArg( ArgIface & arg );

	//! Parse arguments.
	void parse();

	//! \return Argument for the given name.
	ArgIface * findArgument( const String & name );

	//! \return All arguments.
	const std::vector< ArgIface* > & arguments() const;

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
	std::vector< ArgIface* > m_args;
	//! Current command.
	Command * m_command;
	//! Option.
	CmdLineOpts m_opt;
}; // class CmdLine


//
// makeContext
//

//! Make context from the argc and argv.
static inline ContextInternal
#ifdef ARGS_WSTRING_BUILD
	makeContext( int argc, const Char * const * argv )
#else
	makeContext( int argc, const char * const * argv )
#endif
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
#ifdef ARGS_WSTRING_BUILD
	CmdLine::CmdLine( int argc, const Char * const * argv, CmdLineOpts opt )
#else
	CmdLine::CmdLine( int argc, const char * const * argv, CmdLineOpts opt )
#endif
	:	m_context( makeContext( argc, argv ) )
	,	m_command( nullptr )
	,	m_opt( opt )
{
}

inline void
CmdLine::addArg( ArgIface * arg )
{
	if( arg )
	{
		if( std::find( m_args.begin(), m_args.end(), arg ) == m_args.end() )
		{
			arg->setCmdLine( this );

			m_args.push_back( arg );
		}
		else
			throw BaseException( String( SL( "Argument \"" ) ) +
				arg->name() + SL( "\" already in the command line parser." ) );
	}
	else
		throw BaseException( String( SL( "Attempt to add nullptr to the "
			"command line as argument." ) ) );
}

inline void
CmdLine::addArg( ArgIface & arg )
{
	addArg( &arg );
}

inline void
CmdLine::parse()
{
	checkCorrectnessBeforeParsing();

	while( !m_context.atEnd() )
	{
		String word = *m_context.next();

		const String::size_type eqIt = word.find( '=' );

		if( eqIt != String::npos )
		{
			const String value = word.substr( eqIt + 1 );

			if( !value.empty() )
				m_context.prepend( value );

			word = word.substr( 0, eqIt );
		}

		if( isArgument( word ) )
			findArgument( word )->process( m_context );
		else if( isFlag( word ) )
		{
			for( String::size_type i = 1, length = word.length(); i < length; ++i )
			{
				const String flag = String( SL( "-" ) ) +

#ifdef ARGS_QSTRING_BUILD
					String( word[ i ] );
#else
					String( 1, word[ i ] );
#endif

				ArgIface * arg = findArgument( flag );

				if( i < length - 1 && arg->isWithValue() )
					throw BaseException( String( SL( "Only last argument in "
						"flags combo can have value. Flags combo is\"" ) ) +
						word + SL( "\"." ) );
				else
					arg->process( m_context );
			}
		}
		// Command?
		else
		{
			ArgIface * tmp = findArgument( word );

			if( tmp )
			{
				Command * cmd = dynamic_cast< Command* > ( tmp );

				if( cmd )
				{
					if( m_command )
						throw BaseException( String( SL( "Only one command can be "
							"specified. But you entered \"" ) ) + m_command->name() +
							SL( "\" and \"" ) + cmd->name() + SL( "\"." ) );
					else
					{
						m_command = cmd;

						m_command->process( m_context );
					}
				}
				// Argument is as a command
				else
					tmp->process( m_context );
			}
			else
				throw BaseException( String( SL( "Unknown argument \"" ) ) +
					word + SL( "\"." ) );
		}
	}

	checkCorrectnessAfterParsing();
}

inline const std::vector< ArgIface* > &
CmdLine::arguments() const
{
	return m_args;
}

inline void
CmdLine::checkCorrectnessBeforeParsing() const
{
	StringList flags;
	StringList names;

	std::vector< ArgIface* > cmds;

	std::for_each( m_args.cbegin(), m_args.cend(),
		[ &cmds, &flags, &names ] ( ArgIface * arg )
		{
			if( dynamic_cast< Command* > ( arg ) )
				cmds.push_back( arg );
			else
				arg->checkCorrectnessBeforeParsing( flags, names );
		}
	);

	std::for_each( cmds.cbegin(), cmds.cend(),
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

	if( m_opt == CommandIsRequired && !m_command )
		throw BaseException( SL( "Not specified command." ) );
}

inline ArgIface *
CmdLine::findArgument( const String & name )
{
	auto it = std::find_if( m_args.begin(),
		m_args.end(), [ &name ] ( ArgIface * arg ) -> bool
			{ return ( arg->findArgument( name ) != nullptr ); } );

	if( it != m_args.end() )
	{
		Command * tmp = dynamic_cast< Command* > ( *it );

		if( tmp )
			return tmp;
	}

	if( it != m_args.end() )
		return (*it)->findArgument( name );
	else if( m_command )
	{
		ArgIface * tmp = m_command->findChild( name );

		if( tmp )
			return tmp;
	}

	throw BaseException( String( SL( "Unknown argument \"" ) ) +
		name + SL( "\"." ) );
}

} /* namespace Args */

#endif // ARGS__CMD_LINE_HPP__INCLUDED

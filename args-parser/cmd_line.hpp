
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013-2020 Igor Mironchik

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
#include "api.hpp"
#include "utils.hpp"
#include "context.hpp"
#include "exceptions.hpp"
#include "command.hpp"
#include "help.hpp"

// C++ include.
#include <vector>
#include <algorithm>
#include <memory>


namespace Args {

namespace details {

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
// formatCorrectNamesString
//

//! \return Prepared for priniting string of correct names.
String formatCorrectNamesString( const StringList & names )
{
	if( !names.empty() )
	{
		String res;

		bool first = true;

		for( const auto & name : details::asConst( names ) )
		{
			if( !first )
				res.append( SL( " or " ) );

			res.append( name );

			first = false;
		}

		return res;
	}
	else
		return String();
}

} /* namespace details */


//
// CmdLine
//

template< typename PARENT, typename SELF, typename ARGPTR >
using CmdLineAPI = details::API< PARENT, SELF, ARGPTR, true >;

using ArgPtrToAPI = std::unique_ptr< ArgIface, details::Deleter< ArgIface > >;

/*!
	CmdLine is class that holds all rguments and parse
	command line arguments in the correspondence with holded
	arguments.
*/
class CmdLine final
	:	public CmdLineAPI< CmdLine, CmdLine, ArgPtrToAPI >
{
public:
	//! Smart pointer to the argument.
	using ArgPtr = ArgPtrToAPI;
	//! List of child arguments.
	using Arguments = std::vector< ArgPtr >;

	//! Command line options.
	enum CmdLineOpt {
		//! No special options.
		Empty = 0,
		//! Command should be defined.
		CommandIsRequired = 1,
		//! Handle positional arguments.
		HandlePositionalArguments = 2
	}; // enum CmdLineOpt

	//! Storage of command line options.
	using CmdLineOpts = int;

	//! Construct empty CmdLine.
	explicit CmdLine( CmdLineOpts opt = Empty )
		:	details::API< CmdLine, CmdLine, ArgPtr, true > ( *this, *this )
		,	m_command( nullptr )
		,	m_currCommand( nullptr )
		,	m_opt( opt )
	{
	}

#ifdef ARGS_WSTRING_BUILD
	//! Construct CmdLine with set command line arguments.
	CmdLine( int argc, const Char * const * argv,
		CmdLineOpts opt = Empty );
#else
	//! Construct CmdLine with set command line arguments.
	CmdLine( int argc, const char * const * argv,
		CmdLineOpts opt = Empty );
#endif

	virtual ~CmdLine()
	{
	}

	//! \return Parser options.
	CmdLineOpts parserOptions() const
	{
		return m_opt;
	}

	//! Add argument. \note Developer should handle lifetime of the argument.
	void addArg( ArgIface * arg );

	//! Add argument. \note Developer should handle lifetime of the argument.
	void addArg( ArgIface & arg );

	//! Add argument.
	void addArg( ArgPtr arg )
	{
		if( std::find( m_args.begin(), m_args.end(), arg ) ==
			m_args.end() )
		{
			arg->setCmdLine( this );

			m_args.push_back( std::move( arg ) );
		}
		else
			throw BaseException( String( SL( "Argument \"" ) ) +
				arg->name() + SL( "\" already in the command line parser." ) );
	}

	//! Parse arguments.
	void parse();

#ifdef ARGS_WSTRING_BUILD
	//! Parse arguments.
	void parse( int argc, const Char * const * argv )
#else
	//! Parse arguments.
	void parse( int argc, const char * const * argv )
#endif
	{
		m_context = std::move( details::makeContext( argc, argv ) );

		parse();
	}

	//! \return Positional arguments.
	const StringList & positional() const
	{
		return m_positional;
	}

	//! \return Positional string description for the help.
	const String & positionalDescription() const
	{
		return m_positionalDescription;
	}

	//! Set positional string description for the help.
	void setPositionalDescription( const String & d )
	{
		m_positionalDescription = d;
	}

	//! \return Argument for the given name.
	//! \note It's impossible to find any GroupIface with exception of Command.
	ArgIface * findArgument( const String & name )
	{
		auto it = std::find_if( m_args.begin(),
			m_args.end(), [ &name ] ( const auto & arg ) -> bool
				{ return ( arg->findArgument( name ) != nullptr ); } );

		if( it != m_args.end() )
		{
			if( (*it)->type() == ArgType::Command )
				return it->get();
			else
				return (*it)->findArgument( name );
		}
		else if( !m_prevCommand.empty() )
		{
			for( const auto & c : m_prevCommand )
			{
				ArgIface * tmp = c->findChild( name );

				if( tmp )
					return tmp;
			}
		}

		return nullptr;
	}

	//! \return Argument for the given name.
	//! \note It's impossible to find any GroupIface with exception of Command.
	const ArgIface * findArgument( const String & name ) const
	{
		auto it = std::find_if( m_args.cbegin(),
			m_args.cend(), [ &name ] ( const auto & arg ) -> bool
				{ return ( arg->findArgument( name ) != nullptr ); } );

		if( it != m_args.end() )
		{
			if( (*it)->type() == ArgType::Command )
				return it->get();
			else
				return (*it)->findArgument( name );
		}
		else if( !m_prevCommand.empty() )
		{
			for( const auto & c : m_prevCommand )
			{
				ArgIface * tmp = c->findChild( name );

				if( tmp )
					return tmp;
			}
		}

		return nullptr;
	}

	//! \return All arguments.
	const Arguments & arguments() const;

	//! \return Is given name a misspelled name of the argument.
	bool isMisspelledName(
		//! Name to check (misspelled).
		const String & name,
		//! List of possible names for the given misspelled name.
		StringList & possibleNames ) const
	{
		bool ret = false;

		std::for_each( arguments().cbegin(), arguments().cend(),
			[ & ] ( const auto & arg )
			{
				if( arg->type() == ArgType::Command )
				{
					if( arg.get() == m_command )
					{
						if( arg->isMisspelledName( name, possibleNames ) )
							ret = true;
					}
					else if( static_cast< Command* > ( arg.get() )->
						isMisspelledCommand( name, possibleNames ) )
							ret = true;
				}
				else if( arg->isMisspelledName( name, possibleNames ) )
					ret = true;
			} );

		return ret;
	}

	//! Add help.
	CmdLine & addHelp(
		//! Should exception be thrown on help printing.
		bool throwExceptionOnPrint = true,
		//! Application executable.
		const String & appExe = String(),
		//! Application description.
		const String & appDesc = String(),
		//! Line length.
		String::size_type length = 79,
		//! Positional string in the help.
		const String & posDesc = String() )
	{
		auto help = std::unique_ptr< Help, details::Deleter< ArgIface > >
			( new Help( throwExceptionOnPrint ),
				details::Deleter< ArgIface > ( true ) );

		if( !appExe.empty() )
			help->setExecutable( appExe );

		if( !appDesc.empty() )
			help->setAppDescription( appDesc );

		if( !posDesc.empty() )
			setPositionalDescription( posDesc );

		help->setLineLength( length );

		ArgPtr arg = std::move( help );

		addArg( std::move( arg ) );

		return *this;
	}

	template< typename T >
	void addHelp( T throwExceptionOnPrint,
		const String & appExe = String(),
		const String & appDesc = String(),
		String::size_type length = 79 ) = delete;

	//! \return Value of the argument.
	String value(
		//! Name of the argument. Should be full name, i.e '-a' or '--arg'
		//! or 'add' if it's a command or subcommand.
		const String & name ) const
	{
		const auto * arg = findArgument( name );

		if( arg )
		{
			switch( arg->type() )
			{
				case ArgType::Command :
					return ( static_cast< const Command* > ( arg )->value() );

				case ArgType::Arg :
					return ( static_cast< const Arg* > ( arg )->value() );

				case ArgType::MultiArg :
					return ( static_cast< const MultiArg* > ( arg )->value() );

				default :
					return String();
			}
		}
		else
			return String();
	}

	//! \return Values of the argument.
	StringList values(
		//! Name of the argument. Should be full name, i.e '-a' or '--arg'
		//! or 'add' if it's a command or subcommand.
		const String & name ) const
	{
		const auto * arg = findArgument( name );

		if( arg )
		{
			switch( arg->type() )
			{
				case ArgType::Command :
					return ( static_cast< const Command* > ( arg )->values() );

				case ArgType::Arg :
					return StringList(
						{ ( static_cast< const Arg* > ( arg )->value() ) } );

				case ArgType::MultiArg :
					return ( static_cast< const MultiArg* > ( arg )->values() );

				default :
					return StringList();
			}
		}
		else
			return StringList();
	}

	//! \return Is argument defined?
	//! \note It's impossible to check if any GroupIface is defined
	//! with exception of Command.
	bool isDefined(
		//! Name of the argument. Should be full name, i.e '-a' or '--arg'
		//! or 'add' if it's a command or subcommand.
		const String & name ) const
	{
		const auto * arg = findArgument( name );

		if( arg )
			return arg->isDefined();
		else
			return false;
	}

	//! Clear state of the arguments.
	void clear()
	{
		std::for_each( arguments().begin(), arguments().end(),
			[] ( const auto & a ) { a->clear(); } );

		m_command = nullptr;
		m_currCommand = nullptr;
		m_prevCommand.clear();
	}

private:
	//! Check correctness of the arguments before parsing.
	void checkCorrectnessBeforeParsing() const;
	//! Check correctness of the arguments after parsing.
	void checkCorrectnessAfterParsing() const;

	//! Print information about unknown argument.
	void printInfoAboutUnknownArgument( const String & word )
	{
		StringList correctNames;

		if( isMisspelledName( word, correctNames ) )
		{
			const String names = details::formatCorrectNamesString(
				correctNames );

			throw BaseException( String( SL( "Unknown argument \"" ) ) +
				word + SL( "\".\n\nProbably you mean \"" ) + names +
				SL( "\"." ) );
		}
		else
			throw BaseException( String( SL( "Unknown argument \"" ) ) +
				word + SL( "\"." ) );
	}

	//! Save all positional arguments
	void savePositionalArguments( const String & word, bool splitted, bool valuePrepended )
	{
		auto tmp = word;

		if( splitted )
		{
			tmp.append( 1, '=' );

			if( valuePrepended )
				tmp.append( *m_context.next() );
		}

		if( tmp != String( 2, '-' ) )
			m_positional.push_back( tmp );

		while( !m_context.atEnd() )
		{
			auto it = m_context.next();

			m_positional.push_back( *it );
		}
	}

private:
	DISABLE_COPY( CmdLine )

	// Context.
	Context m_context;
	//! Arguments.
	Arguments m_args;
	//! Current top command.
	Command * m_command;
	//! Current command.
	Command * m_currCommand;
	//! Previous command.
	std::vector< Command * > m_prevCommand;
	//! Option.
	CmdLineOpts m_opt;
	//! Positional arguments.
	StringList m_positional;
	//! Positional string description for the help.
	String m_positionalDescription;
}; // class CmdLine


//
// CmdLine
//

inline
#ifdef ARGS_WSTRING_BUILD
	CmdLine::CmdLine( int argc, const Char * const * argv, CmdLineOpts opt )
#else
	CmdLine::CmdLine( int argc, const char * const * argv, CmdLineOpts opt )
#endif
	:	details::API< CmdLine, CmdLine, ArgPtr, true > ( *this, *this )
	,	m_context( details::makeContext( argc, argv ) )
	,	m_command( nullptr )
	,	m_currCommand( nullptr )
	,	m_opt( opt )
{
}

inline void
CmdLine::addArg( ArgIface * arg )
{
	if( arg )
	{
		if( std::find( m_args.begin(), m_args.end(),
			ArgPtr( arg, details::Deleter< ArgIface > ( false ) ) ) ==
				m_args.end() )
		{
			arg->setCmdLine( this );

			m_args.push_back( ArgPtr( arg,
				details::Deleter< ArgIface > ( false ) ) );
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
	clear();

	checkCorrectnessBeforeParsing();

	while( !m_context.atEnd() )
	{
		String word = *m_context.next();

		const String::size_type eqIt = word.find( '=' );

		bool splitted = false;
		bool valuePrepended = false;

		if( eqIt != String::npos )
		{
			splitted = true;

			const String value = word.substr( eqIt + 1 );

			if( !value.empty() )
			{
				valuePrepended = true;
				m_context.prepend( value );
			}

			word = word.substr( 0, eqIt );
		}

		if( details::isArgument( word ) )
		{
			auto * arg = findArgument( word );

			if( arg )
				arg->process( m_context );
			else if( m_opt & HandlePositionalArguments )
				savePositionalArguments( word, splitted, valuePrepended );
			else
				printInfoAboutUnknownArgument( word );
		}
		else if( details::isFlag( word ) )
		{
			std::vector< ArgIface* > tmpArgs;
			bool failed = false;

			for( String::size_type i = 1, length = word.length(); i < length; ++i )
			{
				const String flag = String( SL( "-" ) ) +

#ifdef ARGS_QSTRING_BUILD
					String( word[ i ] );
#else
					String( 1, word[ i ] );
#endif

				auto * arg = findArgument( flag );

				if( !arg )
				{
					failed = true;

					if( !( m_opt & HandlePositionalArguments ) )
						throw BaseException( String( SL( "Unknown argument \"" ) ) +
							flag + SL( "\"." ) );
					else
						break;
				}
				else
					tmpArgs.push_back( arg );

				if( i < length - 1 && arg->isWithValue() )
				{
					failed = true;

					if( !( m_opt & HandlePositionalArguments ) )
						throw BaseException( String( SL( "Only last argument in "
							"flags combo can be with value. Flags combo is \"" ) ) +
							word + SL( "\"." ) );
					else
						break;
				}
				else
					arg->process( m_context );
			}

			if( failed && ( m_opt & HandlePositionalArguments ) )
			{
				for( auto * a : tmpArgs )
					a->clear();

				savePositionalArguments( word, splitted, valuePrepended );
			}
		}
		// Command?
		else
		{
			auto * tmp = findArgument( word );

			auto check = [this, &tmp, &word] ()
			{
				const auto & args = m_args;

				const auto it = std::find_if( args.cbegin(),
					args.cend(), [&tmp] ( const auto & arg ) -> bool
						{ return ( arg.get() == tmp ); } );

				if( m_command && it != args.cend() && !m_command->findChild( word ) )
					throw BaseException( String( SL(
							"Only one command from one level can be specified. "
							"But you defined \"" ) ) +
						word + SL( "\" and \"" ) + m_command->name() + SL( "\"." ) );
			};

			if( tmp )
			{
				if( tmp->type() == ArgType::Command )
				{
					bool stopProcessing = false;

					try {
						if( !m_prevCommand.empty() )
						{
							for( const auto & prev : m_prevCommand )
							{
								const auto & args = prev->children();

								const auto it = std::find_if( args.cbegin(),
									args.cend(), [&tmp] ( const auto & arg ) -> bool
										{ return ( arg.get() == tmp ); } );

								if( it != args.cend() && !m_currCommand->findChild( word ) )
									throw BaseException( String( SL(
											"Only one command from one level can be specified. "
											"But you defined \"" ) ) +
										word + SL( "\" and \"" ) + prev->name() + SL( "\"." ) );
							}

							check();
						}
						else
							check();
					}
					catch( const BaseException & )
					{
						if( m_opt & HandlePositionalArguments )
						{
							savePositionalArguments( word, splitted, valuePrepended );

							stopProcessing = true;
						}
						else
							throw;
					}

					if( stopProcessing )
						continue;

					if( !m_command )
					{
						m_command = static_cast< Command* > ( tmp );

						m_currCommand = m_command;

						m_prevCommand.push_back( m_command );

						m_command->process( m_context );
					}
					else
					{
						auto * cmd = static_cast< Command* > ( tmp );

						m_currCommand->setCurrentSubCommand( cmd );

						m_currCommand = cmd;

						m_prevCommand.push_back( m_currCommand );

						m_currCommand->process( m_context );
					}
				}
				else if( m_opt & HandlePositionalArguments )
					savePositionalArguments( word, splitted, valuePrepended );
				else
					printInfoAboutUnknownArgument( word );
			}
			else if( m_opt & HandlePositionalArguments )
				savePositionalArguments( word, splitted, valuePrepended );
			else
				printInfoAboutUnknownArgument( word );
		}
	}

	checkCorrectnessAfterParsing();
}

inline const CmdLine::Arguments &
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
		[ &cmds, &flags, &names ] ( const auto & arg )
		{
			if( arg->type() == ArgType::Command )
				cmds.push_back( arg.get() );
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
		[] ( const auto & arg )
			{ arg->checkCorrectnessAfterParsing(); }
	);

	if( m_opt & CommandIsRequired && !m_command )
		throw BaseException( SL( "Not specified command." ) );
}

} /* namespace Args */

#include "help_printer.hpp"

namespace Args {

inline
Help::Help( bool throwExceptionOnPrint )
	:	Arg( SL( 'h' ), SL( "help" ), true, false )
	,	m_printer( new HelpPrinter )
	,	m_throwExceptionOnPrint( throwExceptionOnPrint )
{
	setDescription( SL( "Print this help." ) );
	setLongDescription( SL( "Print this help." ) );
}

} /* namespace Args */

#endif // ARGS__CMD_LINE_HPP__INCLUDED


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

/*!
	CmdLine is class that holds all rguments and parse
	command line arguments in the correspondence with holded
	arguments.
*/
class CmdLine final
	:	public details::API< CmdLine, CmdLine,
			std::unique_ptr< ArgIface, details::Deleter< ArgIface > >, true >
{
public:
	//! Smart pointer to the argument.
	using ArgPtr = std::unique_ptr< ArgIface, details::Deleter< ArgIface > >;
	//! List of child arguments.
	using Arguments = std::vector< ArgPtr >;

	//! Command line options.
	enum CmdLineOpts {
		//! No special options.
		Empty = 0,
		//! Command should be defined.
		CommandIsRequired = 1
	}; // enum CmdLineOpts

	explicit CmdLine( CmdLineOpts opt = Empty )
		:	details::API< CmdLine, CmdLine, ArgPtr, true > ( *this, *this )
		,	m_command( nullptr )
		,	m_opt( opt )
	{
	}

#ifdef ARGS_WSTRING_BUILD
	CmdLine( int argc, const Char * const * argv,
		CmdLineOpts opt = Empty );
#else
	CmdLine( int argc, const char * const * argv,
		CmdLineOpts opt = Empty );
#endif

	virtual ~CmdLine()
	{
	}

	//! Add argument. \note Developer should handle lifetime of the argument.
	void addArg( ArgIface * arg );

	//! Add argument. \note Developer should handle lifetime of the argument.
	void addArg( ArgIface & arg );

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
	//! Reparse arguments.
	void parse( int argc, const Char * const * argv )
#else
	//! Reparse arguments.
	void parse( int argc, const char * const * argv )
#endif
	{
		m_context = std::move( details::makeContext( argc, argv ) );

		clear();

		parse();
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
		else if( m_command )
		{
			ArgIface * tmp = m_command->findChild( name );

			if( tmp )
				return tmp;
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
		else if( m_command )
		{
			const ArgIface * tmp = m_command->findChild( name );

			if( tmp )
				return tmp;
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

	//! Add Command.
	template< typename NAME >
	API< CmdLine, Command, Command::ArgPtr > addCommand(
		//! Name of the group.
		NAME && name,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		auto cmd = std::unique_ptr< Command, details::Deleter< ArgIface > > (
			new Command( std::forward< NAME > ( name ), opt ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			cmd->setDescription( desc );

		if( !longDesc.empty() )
			cmd->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			cmd->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			cmd->setValueSpecifier( valueSpecifier );

		Command & c = *cmd;

		ArgPtr arg = std::move( cmd );

		addArg( std::move( arg ) );

		return API< CmdLine, Command, Command::ArgPtr > ( *this, c );
	}

	//! Add Command.
	template< typename NAME >
	API< CmdLine, Command, Command::ArgPtr > addCommandWithDefaultValues(
		//! Name of the group.
		NAME && name,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		auto cmd = std::unique_ptr< Command, details::Deleter< ArgIface > > (
			new Command( std::forward< NAME > ( name ), opt ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			cmd->setDescription( desc );

		if( !longDesc.empty() )
			cmd->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			cmd->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			cmd->setValueSpecifier( valueSpecifier );

		Command & c = *cmd;

		ArgPtr arg = std::move( cmd );

		addArg( std::move( arg ) );

		return API< CmdLine, Command, Command::ArgPtr > ( *this, c );
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
		String::size_type length = 79 )
	{
		auto help = std::unique_ptr< Help, details::Deleter< ArgIface > >
			( new Help( throwExceptionOnPrint ),
				details::Deleter< ArgIface > ( true ) );

		if( !appExe.empty() )
			help->setExecutable( appExe );

		if( !appDesc.empty() )
			help->setAppDescription( appDesc );

		help->setLineLength( length );

		ArgPtr arg = std::move( help );

		addArg( std::move( arg ) );

		return *this;
	}

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

				case ArgType::ArgAsCommand :
					return ( static_cast< const ArgAsCommand* > ( arg )->value() );

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

				case ArgType::ArgAsCommand :
					return ( static_cast< const ArgAsCommand* > ( arg )->values() );

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

private:
	DISABLE_COPY( CmdLine )

	// Context.
	Context m_context;
	//! Arguments.
	Arguments m_args;
	//! Current command.
	Command * m_command;
	//! Option.
	CmdLineOpts m_opt;
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

		if( details::isArgument( word ) )
		{
			auto * arg = findArgument( word );

			if( arg )
				arg->process( m_context );
			else
				printInfoAboutUnknownArgument( word );
		}
		else if( details::isFlag( word ) )
		{
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
					throw BaseException( String( SL( "Unknown argument \"" ) ) +
						flag + SL( "\"." ) );

				if( i < length - 1 && arg->isWithValue() )
					throw BaseException( String( SL( "Only last argument in "
						"flags combo can be with value. Flags combo is \"" ) ) +
						word + SL( "\"." ) );
				else
					arg->process( m_context );
			}
		}
		// Command?
		else
		{
			auto * tmp = findArgument( word );

			if( tmp )
			{
				if( tmp->type() == ArgType::Command )
				{
					if( m_command )
						throw BaseException( String( SL( "Only one command can be "
							"specified. But you entered \"" ) ) + m_command->name() +
							SL( "\" and \"" ) + tmp->name() + SL( "\"." ) );
					else
					{
						m_command = static_cast< Command* > ( tmp );

						m_command->process( m_context );
					}
				}
				// Argument is as a command
				else
					tmp->process( m_context );
			}
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

	if( m_opt == CommandIsRequired && !m_command )
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

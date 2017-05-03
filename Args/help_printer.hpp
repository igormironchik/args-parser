
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

#ifndef ARGS__HELP_PRINTER_HPP__INCLUDED
#define ARGS__HELP_PRINTER_HPP__INCLUDED

// C++ include.
#include <list>
#include <algorithm>
#include <functional>

// Args include.
#include "utils.hpp"
#include "cmd_line.hpp"
#include "exceptions.hpp"
#include "group_iface.hpp"
#include "groups.hpp"
#include "command.hpp"
#include "types.hpp"


namespace Args {

class CmdLine;
class ArgIface;


//
// HelpPrinter
//

/*!
	HelpPrinter is a class that prints help.
*/
class HelpPrinter final {
public:
	HelpPrinter();

	~HelpPrinter();

	//! Print help for all arguments.
	void print(
		//! Output stream for the printing help.
		OutStreamType & to );

	//! Print help for the given argument.
	void print(
		//! Name of the argument. I.e. "-t" or "--timeout".
		const String & name,
		//! Output stream for the printing help.
		OutStreamType & to );

	//! Print help for command's argument.
	void print(
		//! Command.
		Command * cmd,
		//! Name of the argument. I.e. "-t" or "--timeout".
		const String & name,
		//! Output stream for the printing help.
		OutStreamType & to );

	//! Set executable name.
	void setExecutable( const String & exe );

	//! Set description for the application.
	void setAppDescription( const String & desc );

	//! Set command line.
	void setCmdLine( CmdLine * cmd );

	//! Set line length for the help.
	void setLineLength( String::size_type length );

private:
	//! \return List of words with usage string for the argument.
	StringList createUsageString( ArgIface * arg,
		bool required ) const;
	//! List of words from string.
	StringList splitToWords( const String & s ) const;
	//! Print string with given margins.
	void printString( OutStreamType & to,
		const StringList & words,
		String::size_type currentPos, String::size_type leftMargin,
		String::size_type rightMargin ) const;
	//! Print help for the argument.
	void print( ArgIface * arg, OutStreamType & to ) const;
	//! Sort argument.
	void sortArg( ArgIface * arg,
		std::list< Command* > & commands,
		std::list< ArgIface* > & required,
		std::list< ArgIface* > & optional,
		String::size_type & maxFlag,
		String::size_type & maxName,
		String::size_type & maxCommand,
		bool requiredAllOfGroup = false ) const;
	//! Print help only for argument.
	void printOnlyFor( ArgIface * arg, OutStreamType & to,
		String::size_type beforeDescription, String::size_type maxFlag ) const;

private:
	DISABLE_COPY( HelpPrinter )

	//! Executable name.
	String m_exeName;
	//! Application description.
	String m_appDescription;
	//! Command line.
	CmdLine * m_cmdLine;
	//! Line length.
	String::size_type m_lineLength;
}; // class HelpPrinter


//
// HelpPrinter
//

inline
HelpPrinter::HelpPrinter()
	:	m_cmdLine( 0 )
	,	m_lineLength( 79 )
{
}

inline
HelpPrinter::~HelpPrinter()
{
}

static inline void
printOffset( OutStreamType & to, String::size_type currentPos,
	String::size_type leftMargin )
{
	if( currentPos < leftMargin )
		to << String( leftMargin - currentPos, ' ' );
}

static inline void
calcMaxFlagAndName( ArgIface * arg, String::size_type & maxFlag,
	String::size_type & maxName )
{
	String::size_type f = 1;
	String::size_type n = ( !arg->argumentName().empty() ?
		arg->argumentName().length() : arg->name().length() );

	if( arg->isWithValue() )
	{
		if( n > 0 )
			n += 3 + arg->valueSpecifier().length();
		else
			f += 3 + arg->valueSpecifier().length();
	}

	if( f > maxFlag )
		maxFlag = f;

	if( n > maxName )
		maxName = n;
}

inline void
HelpPrinter::sortArg( ArgIface * arg,
	std::list< Command* > & commands,
	std::list< ArgIface* > & required,
	std::list< ArgIface* > & optional,
	String::size_type & maxFlag,
	String::size_type & maxName,
	String::size_type & maxCommand,
	bool requiredAllOfGroup ) const
{
	GroupIface * g = dynamic_cast< GroupIface* > ( arg );
	Command * cmd = dynamic_cast< Command* > ( arg );

	if( cmd )
	{
		commands.push_back( cmd );

		String::size_type length = cmd->name().length() + ( cmd->isWithValue() ?
			3 + cmd->valueSpecifier().length() : 0 );

		if( length > maxCommand )
			maxCommand = length;
	}
	else if( g )
	{
		if( g->isRequired() && dynamic_cast< AllOfGroup* > ( g ) )
			requiredAllOfGroup = true;
		else
			requiredAllOfGroup = false;

		auto f = std::bind( &HelpPrinter::sortArg, this, std::placeholders::_1,
			std::ref( commands ), std::ref( required ),
			std::ref( optional ), std::ref( maxFlag ),
			std::ref( maxName ), std::ref( maxCommand ),
			requiredAllOfGroup );

		std::for_each( g->children().cbegin(),
			g->children().cend(), f );
	}
	else
	{
		if( arg->isRequired() || requiredAllOfGroup )
			required.push_back( arg );
		else
			optional.push_back( arg );

		calcMaxFlagAndName( arg, maxFlag, maxName );
	}
}

inline void
HelpPrinter::printOnlyFor( ArgIface * arg, OutStreamType & to,
	String::size_type beforeDescription, String::size_type maxFlag ) const
{
	String::size_type pos = 0;

	if( !arg->flag().empty() )
	{
		to << ' ';
		++pos;
		to << '-' << arg->flag();
		pos += arg->flag().length() + 1;

		if( !arg->argumentName().empty() )
		{
			to << ',';
			++pos;
		}
		else if( arg->isWithValue() )
		{
			to << " <" << arg->valueSpecifier() << '>';
			pos += arg->valueSpecifier().length() + 3;
		}
	}
	else
	{
		printOffset( to, pos, maxFlag + 1 );
		pos += maxFlag + 1;
	}

	if( !arg->argumentName().empty() )
	{
		to << ' ';
		++pos;
		to << "--" << arg->argumentName();
		pos += arg->argumentName().length() + 2;

		if( arg->isWithValue() )
		{
			to << " <" << arg->valueSpecifier() << '>';
			pos += arg->valueSpecifier().length() + 3;
		}
	}
	else
	{
		to << ' ';
		++pos;
		to << arg->name();
		pos += arg->name().length();

		if( arg->isWithValue() )
		{
			to << " <" << arg->valueSpecifier() << '>';
			pos += arg->valueSpecifier().length() + 3;
		}
	}

	printString( to, splitToWords( arg->description() ), pos,
		beforeDescription, 0 );

	to << "\n" << "\n";

	to.flush();
}

inline void
HelpPrinter::print( OutStreamType & to )
{
	std::list< ArgIface* > required;
	std::list< ArgIface* > optional;
	std::list< Command* > commands;

	String::size_type maxFlag = 0;
	String::size_type maxName = 0;
	String::size_type maxCommand = 0;

	bool requiredAllOfGroup = false;

	auto f = std::bind( &HelpPrinter::sortArg, this, std::placeholders::_1,
		std::ref( commands ), std::ref( required ),
		std::ref( optional ), std::ref( maxFlag ),
		std::ref( maxName ), std::ref( maxCommand ),
		requiredAllOfGroup );

	std::for_each( m_cmdLine->arguments().cbegin(),
		m_cmdLine->arguments().cend(), f );

	maxFlag += 2;
	maxName += 2;
	maxCommand += 2;

	printString( to, splitToWords( m_appDescription ), 0, 0, 0 );

	to << "\n" << "\n";

	if( commands.empty() )
	{
		StringList usage;

		usage.push_back( m_exeName );

		bool requiredFlag = true;

		std::function< void ( ArgIface* ) > createUsageAndAppend =
			[ & ] ( ArgIface * arg )
			{
				const StringList words = createUsageString( arg,
					requiredFlag );

				usage.insert( usage.end(), words.cbegin(), words.cend() );
			};

		std::for_each( required.cbegin(), required.cend(),
			createUsageAndAppend );

		requiredFlag = false;

		std::for_each( optional.cbegin(), optional.cend(),
			createUsageAndAppend );

		to << "Usage: ";

		printString( to, usage, 7, 7, 7 );

		to << "\n" << "\n";
	}
	else
	{
		to << "Usage: " << m_exeName << " <commands>";

		if( !optional.empty() || !required.empty() )
			to << " <args>";

		to << "\n" << "\n";

		std::for_each( commands.cbegin(), commands.cend(),
			[ & ] ( Command * cmd )
			{
				String::size_type pos = 2;

				to << "  " << cmd->name();

				pos += cmd->name().length();

				if( cmd->isWithValue() )
				{
					to << " <" << cmd->valueSpecifier() << ">";

					pos += 3 + cmd->valueSpecifier().length();
				}

				printString( to, splitToWords( cmd->description() ), pos,
					maxCommand + 1, 0 );

				to << "\n";
			} );

		to << "\n" << "\n";
	}

	auto printArg = std::bind( &HelpPrinter::printOnlyFor, this,
		std::placeholders::_1, std::ref( to ), maxFlag + 1 + maxName + 2,
		maxFlag );

	if( !required.empty() )
	{
		to << "Required arguments:" << "\n";

		std::for_each( required.cbegin(), required.cend(), printArg );
	}

	if( !optional.empty() )
	{
		to << "Optional arguments:" << "\n";

		std::for_each( optional.cbegin(), optional.cend(), printArg );
	}

	to.flush();
}

inline void
HelpPrinter::print( const String & name, OutStreamType & to )
{
	try {
		ArgIface * arg = m_cmdLine->findArgument( name );

		Command * cmd = dynamic_cast < Command* > ( arg );

		if( cmd )
		{
			// Prepare global arguments.
			std::list< ArgIface* > grequired;
			std::list< ArgIface* > goptional;
			std::list< Command* > gcommands;

			String::size_type gmaxFlag = 0;
			String::size_type gmaxName = 0;
			String::size_type gmaxCommand = 0;

			bool requiredAllOfGroup = false;

			auto gf = std::bind( &HelpPrinter::sortArg, this, std::placeholders::_1,
				std::ref( gcommands ), std::ref( grequired ),
				std::ref( goptional ), std::ref( gmaxFlag ),
				std::ref( gmaxName ), std::ref( gmaxCommand ),
				requiredAllOfGroup );

			std::for_each( m_cmdLine->arguments().cbegin(),
				m_cmdLine->arguments().cend(), gf );

			gmaxFlag += 2;
			gmaxName += 2;
			gmaxCommand += 2;

			// Prepare arguments of command.
			std::list< ArgIface* > required;
			std::list< ArgIface* > optional;
			std::list< Command* > commands;

			String::size_type maxFlag = 0;
			String::size_type maxName = 0;
			String::size_type maxCommand = 0;

			requiredAllOfGroup = false;

			auto f = std::bind( &HelpPrinter::sortArg, this, std::placeholders::_1,
				std::ref( commands ), std::ref( required ),
				std::ref( optional ), std::ref( maxFlag ),
				std::ref( maxName ), std::ref( maxCommand ),
				requiredAllOfGroup );

			std::for_each( cmd->children().cbegin(),
				cmd->children().cend(), f );

			maxFlag += 2;
			maxName += 2;
			maxCommand += 2;

			// Print.
			printString( to, splitToWords( cmd->longDescription() ), 0, 0, 0 );

			to << "\n" << "\n";

			// Print command's arguments.
			auto printArg = std::bind( &HelpPrinter::printOnlyFor, this,
				std::placeholders::_1, std::ref( to ), maxFlag + 1 + maxName + 2,
				maxFlag );

			if( !required.empty() )
			{
				to << "Required arguments:" << "\n";

				std::for_each( required.cbegin(), required.cend(), printArg );
			}

			if( !optional.empty() )
			{
				to << "Optional arguments:" << "\n";

				std::for_each( optional.cbegin(), optional.cend(), printArg );
			}

			to << "\n";

			// Print global arguments.
			if( !grequired.empty() || !goptional.empty() )
			{
				to << "Global arguments:" << "\n" << "\n";

				auto printGlobalArg = std::bind( &HelpPrinter::printOnlyFor, this,
					std::placeholders::_1, std::ref( to ),
					gmaxFlag + 1 + gmaxName + 2, gmaxFlag );

				if( !grequired.empty() )
				{
					to << "Required arguments:" << "\n";

					std::for_each( grequired.cbegin(), grequired.cend(),
						printGlobalArg );
				}

				if( !goptional.empty() )
				{
					to << "Optional arguments:" << "\n";

					std::for_each( goptional.cbegin(), goptional.cend(),
						printGlobalArg );
				}
			}

			to.flush();
		}
		else
			print( arg, to );
	}
	catch( const BaseException & )
	{
		print( to );
	}
}

inline void
HelpPrinter::print( ArgIface * arg, OutStreamType & to ) const
{
	StringList usage = createUsageString( arg,
		arg->isRequired() );

	to << "Usage: ";

	std::for_each( usage.cbegin(), usage.cend(),
		[ & ] ( const String & s )
			{ to << s << ' '; }
	);

	to << "\n" << "\n";

	printString( to, splitToWords( arg->longDescription() ),
		0, 7, 7 );

	to << "\n" << "\n";

	to.flush();
}

inline void
HelpPrinter::setExecutable( const String & exe )
{
	m_exeName = exe;
}

inline void
HelpPrinter::setAppDescription( const String & desc )
{
	m_appDescription = desc;
}

inline void
HelpPrinter::setCmdLine( CmdLine * cmd )
{
	m_cmdLine = cmd;
}

inline void
HelpPrinter::setLineLength( String::size_type length )
{
	if( length > 40 )
		m_lineLength = length;
	else
		m_lineLength = 40;
}

inline StringList
HelpPrinter::createUsageString( ArgIface * arg, bool required ) const
{
	StringList result;

	String usage;

	if( !required )
		usage.append( "[ " );

	if( !arg->flag().empty() )
	{
		usage.append( "-" );
		usage.append( arg->flag() );

		if( !arg->argumentName().empty() )
		{
			usage.append( "," );

			result.push_back( usage );

			usage.clear();
		}
	}

	if( !arg->argumentName().empty() )
	{
		usage.append( "--" );
		usage.append( arg->argumentName() );
	}
	else
		usage.append( arg->name() );

	if( arg->isWithValue() )
	{
		usage.append( " <" );
		usage.append( arg->valueSpecifier() );
		usage.append( ">" );
	}

	if( !required )
		usage.append( " ]" );

	result.push_back( usage );

	return result;
}

static inline bool
isSpaceChar( const Char & c )
{
	static const String spaceChars = " \n\t\r ";

	return ( spaceChars.find( c ) != String::npos );
}

inline StringList
HelpPrinter::splitToWords( const String & s ) const
{
	String word;
	StringList result;

	std::for_each( s.cbegin(), s.cend(),
		[ &word, &result ] ( const Char & c )
		{
			if( isSpaceChar( c ) )
			{
				if( !word.empty() )
					result.push_back( word );

				word.clear();
			}
			else
				word.append( 1, c );
		}
	);

	if( !word.empty() )
		result.push_back( word );

	return result;
}

inline void
HelpPrinter::printString( OutStreamType & to,
	const StringList & words,
	String::size_type currentPos, String::size_type leftMargin,
	String::size_type rightMargin ) const
{
	const String::size_type occupied = leftMargin + rightMargin;

	String::size_type maxLineLength = ( occupied < m_lineLength ?
		m_lineLength - occupied : 0 );

	if( maxLineLength < 20 )
	{
		to << "\n\n";

		maxLineLength = m_lineLength - 20;

		currentPos = 0;
		leftMargin = 20;
		rightMargin = 0;
	}

	String::size_type length = 0;

	bool makeOffset = ( currentPos < leftMargin );

	std::for_each( words.cbegin(), words.cend(),
		[ & ] ( const String & word )
		{
			if( makeOffset )
			{
				printOffset( to, currentPos, leftMargin );

				makeOffset = false;
			}

			if( length + word.length() < maxLineLength )
			{
				length += word.length();

				to << word;

				if( length < maxLineLength )
				{
					++length;
					to << ' ';
				}
			}
			else
			{
				currentPos = 0;

				length = 0;

				maxLineLength = m_lineLength - leftMargin - rightMargin;

				to << "\n";

				printOffset( to, currentPos, leftMargin );

				to << word;

				length += word.length();

				if( length < maxLineLength )
				{
					++length;
					to << ' ';
				}
			}
		}
	);
}

inline void
HelpPrinter::print( Command * cmd, const String & name,
	OutStreamType & to )
{
	if( cmd )
	{
		ArgIface * arg = cmd->isItYourChild( name );

		if( arg )
			print( arg, to );
		else
		{
			try {
				arg = m_cmdLine->findArgument( name );

				print( arg, to );
			}
			catch( const BaseException & )
			{
				print( cmd->name(), to );
			}
		}
	}
	else
		print( to );
}

} /* namespace Args */

#endif // ARGS__HELP_PRINTER_HPP__INCLUDED

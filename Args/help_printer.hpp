
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
#include <string>
#include <list>
#include <algorithm>
#include <functional>
#include <ostream>

// Args include.
#include <Args/utils.hpp>
#include <Args/cmd_line.hpp>
#include <Args/exceptions.hpp>
#include <Args/group_iface.hpp>
#include <Args/groups.hpp>
#include <Args/command.hpp>


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
		std::ostream & to );

	//! Print help for the given argument.
	void print(
		//! Name of the argument. I.e. "-t" or "--timeout".
		const std::string & name,
		//! Output stream for the printing help.
		std::ostream & to );

	//! Print help for command's argument.
	void print(
		//! Command.
		Command * cmd,
		//! Name of the argument. I.e. "-t" or "--timeout".
		const std::string & name,
		//! Output stream for the printing help.
		std::ostream & to );

	//! Set executable name.
	void setExecutable( const std::string & exe );

	//! Set description for the application.
	void setAppDescription( const std::string & desc );

	//! Set command line.
	void setCmdLine( CmdLine * cmd );

	//! Set line length for the help.
	void setLineLength( size_t length );

private:
	//! \return List of words with usage string for the argument.
	std::list< std::string > createUsageString( ArgIface * arg,
		bool required ) const;
	//! List of words from string.
	std::list< std::string > splitToWords( const std::string & s ) const;
	//! Print string with given margins.
	void printString( std::ostream & to,
		const std::list< std::string > & words,
		size_t currentPos, size_t leftMargin, size_t rightMargin ) const;
	//! Print help for the argument.
	void print( ArgIface * arg, std::ostream & to ) const;
	//! Sort argument.
	void sortArg( ArgIface * arg,
		std::list< Command* > & commands,
		std::list< ArgIface* > & required,
		std::list< ArgIface* > & optional,
		std::size_t & maxFlag,
		std::size_t & maxName,
		std::size_t & maxCommand,
		bool requiredAllOfGroup = false ) const;
	//! Print help only for argument.
	void printOnlyFor( ArgIface * arg, std::ostream & to,
		std::size_t beforeDescription, std::size_t maxFlag ) const;

private:
	DISABLE_COPY( HelpPrinter )

	//! Executable name.
	std::string m_exeName;
	//! Application description.
	std::string m_appDescription;
	//! Command line.
	CmdLine * m_cmdLine;
	//! Line length.
	size_t m_lineLength;
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
printOffset( std::ostream & to, size_t currentPos, size_t leftMargin )
{
	if( currentPos < leftMargin )
		to << std::string( leftMargin - currentPos, ' ' );
}

static inline void
calcMaxFlagAndName( ArgIface * arg, size_t & maxFlag, size_t & maxName )
{
	size_t f = 1;
	size_t n = arg->argumentName().length();

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
	std::size_t & maxFlag,
	std::size_t & maxName,
	std::size_t & maxCommand,
	bool requiredAllOfGroup ) const
{
	GroupIface * g = dynamic_cast< GroupIface* > ( arg );
	Command * cmd = dynamic_cast< Command* > ( arg );

	if( cmd )
	{
		commands.push_back( cmd );

		std::size_t length = cmd->name().length() + ( cmd->isWithValue() ?
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
HelpPrinter::printOnlyFor( ArgIface * arg, std::ostream & to,
	std::size_t beforeDescription, std::size_t maxFlag ) const
{
	size_t pos = 0;

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

	printString( to, splitToWords( arg->description() ), pos,
		beforeDescription, 0 );

	to << std::endl << std::endl;
}

inline void
HelpPrinter::print( std::ostream & to )
{
	std::list< ArgIface* > required;
	std::list< ArgIface* > optional;
	std::list< Command* > commands;

	size_t maxFlag = 0;
	size_t maxName = 0;
	size_t maxCommand = 0;

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

	to << std::endl << std::endl;

	if( commands.empty() )
	{
		std::list< std::string > usage;

		usage.push_back( m_exeName );

		bool requiredFlag = true;

		std::function< void ( ArgIface* ) > createUsageAndAppend =
			[ & ] ( ArgIface * arg )
			{
				const std::list< std::string > words = createUsageString( arg,
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

		to << std::endl << std::endl;
	}
	else
	{
		to << "Usage: " << m_exeName << " <commands>";

		if( !optional.empty() || !required.empty() )
			to << " <args>";

		to << std::endl << std::endl;

		std::for_each( commands.cbegin(), commands.cend(),
			[ & ] ( Command * cmd )
			{
				std::size_t pos = 2;

				to << "  " << cmd->name();

				pos += cmd->name().length();

				if( cmd->isWithValue() )
				{
					to << " <" << cmd->valueSpecifier() << ">";

					pos += 3 + cmd->valueSpecifier().length();
				}

				printString( to, splitToWords( cmd->description() ), pos,
					maxCommand + 1, 0 );

				to << std::endl;
			} );

		to << std::endl << std::endl;
	}

	auto printArg = std::bind( &HelpPrinter::printOnlyFor, this,
		std::placeholders::_1, std::ref( to ), maxFlag + 1 + maxName + 2,
		maxFlag );

	if( !required.empty() )
	{
		to << "Required arguments:" << std::endl;

		std::for_each( required.cbegin(), required.cend(), printArg );
	}

	if( !optional.empty() )
	{
		to << "Optional arguments:" << std::endl;

		std::for_each( optional.cbegin(), optional.cend(), printArg );
	}
}

inline void
HelpPrinter::print( const std::string & name, std::ostream & to )
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

			size_t gmaxFlag = 0;
			size_t gmaxName = 0;
			size_t gmaxCommand = 0;

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

			size_t maxFlag = 0;
			size_t maxName = 0;
			size_t maxCommand = 0;

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

			to << std::endl << std::endl;

			// Print command's arguments.
			auto printArg = std::bind( &HelpPrinter::printOnlyFor, this,
				std::placeholders::_1, std::ref( to ), maxFlag + 1 + maxName + 2,
				maxFlag );

			if( !required.empty() )
			{
				to << "Required arguments:" << std::endl;

				std::for_each( required.cbegin(), required.cend(), printArg );
			}

			if( !optional.empty() )
			{
				to << "Optional arguments:" << std::endl;

				std::for_each( optional.cbegin(), optional.cend(), printArg );
			}

			to << std::endl;

			// Print global arguments.
			if( !grequired.empty() || !goptional.empty() )
			{
				to << "Global arguments:" << std::endl << std::endl;

				printArg = std::bind( &HelpPrinter::printOnlyFor, this,
					std::placeholders::_1, std::ref( to ),
					gmaxFlag + 1 + gmaxName + 2, gmaxFlag );

				if( !grequired.empty() )
				{
					to << "Required arguments:" << std::endl;

					std::for_each( grequired.cbegin(), grequired.cend(),
						printArg );
				}

				if( !goptional.empty() )
				{
					to << "Optional arguments:" << std::endl;

					std::for_each( goptional.cbegin(), goptional.cend(),
						printArg );
				}
			}
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
HelpPrinter::print( ArgIface * arg, std::ostream & to ) const
{
	std::list< std::string > usage = createUsageString( arg,
		arg->isRequired() );

	to << "Usage: ";

	std::for_each( usage.cbegin(), usage.cend(),
		[ & ] ( const std::string & s )
			{ to << s << ' '; }
	);

	to << std::endl << std::endl;

	printString( to, splitToWords( arg->longDescription() ),
		0, 7, 7 );

	to << std::endl << std::endl;
}

inline void
HelpPrinter::setExecutable( const std::string & exe )
{
	m_exeName = exe;
}

inline void
HelpPrinter::setAppDescription( const std::string & desc )
{
	m_appDescription = desc;
}

inline void
HelpPrinter::setCmdLine( CmdLine * cmd )
{
	m_cmdLine = cmd;
}

inline void
HelpPrinter::setLineLength( size_t length )
{
	if( length > 40 )
		m_lineLength = length;
	else
		m_lineLength = 40;
}

inline std::list< std::string >
HelpPrinter::createUsageString( ArgIface * arg, bool required ) const
{
	std::list< std::string > result;

	std::string usage;

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
isSpaceChar( const char & c )
{
	static const std::string spaceChars = " \n\t\r ";

	return ( spaceChars.find( c ) != std::string::npos );
}

inline std::list< std::string >
HelpPrinter::splitToWords( const std::string & s ) const
{
	std::string word;
	std::list< std::string > result;

	std::for_each( s.cbegin(), s.cend(),
		[ &word, &result ] ( const char & c )
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
HelpPrinter::printString( std::ostream & to,
	const std::list< std::string > & words,
	size_t currentPos, size_t leftMargin, size_t rightMargin ) const
{
	const size_t occupied = leftMargin + rightMargin;

	size_t maxLineLength = ( occupied < m_lineLength ?
		m_lineLength - occupied : 0 );

	if( maxLineLength < 20 )
	{
		to << "\n\n";

		maxLineLength = m_lineLength - 20;

		currentPos = 0;
		leftMargin = 20;
		rightMargin = 0;
	}

	size_t length = 0;

	bool makeOffset = ( currentPos < leftMargin );

	std::for_each( words.cbegin(), words.cend(),
		[ & ] ( const std::string & word )
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

				to << std::endl;

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
HelpPrinter::print( Command * cmd, const std::string & name,
	std::ostream & to )
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

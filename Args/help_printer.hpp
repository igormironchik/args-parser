
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


namespace Args {

class CmdLine;
class ArgIface;


//
// HelpPrinter
//

/*!
	HelpPrinter is a class that prints help.
*/
class HelpPrinter {
public:
	HelpPrinter();

	virtual ~HelpPrinter();

	//! Print help for all arguments.
	virtual void print(
		//! Output stream for the printing help.
		std::ostream & to );

	//! Print help for the given argument.
	virtual void print(
		//! Name of the argument. I.e. "-t" or "--timeout".
		const std::string & name,
		//! Output stream for the printing help.
		std::ostream & to );

	//! Set executable name.
	virtual void setExecutable( const std::string & exe );

	//! Set description for the application.
	virtual void setAppDescription( const std::string & desc );

	//! Set command line.
	virtual void setCmdLine( CmdLine * cmd );

	//! Set line length for the help.
	virtual void setLineLength( size_t length );

protected:
	//! \return List of words with usage string for the argument.
	std::list< std::string > createUsageString( ArgIface * arg, bool required );
	//! List of words from string.
	std::list< std::string > splitToWords( const std::string & s );
	//! Print string with given margins.
	void printString( std::ostream & to,
		const std::list< std::string > & words,
		size_t currentPos, size_t leftMargin, size_t rightMargin );

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
HelpPrinter::print( std::ostream & to )
{
	std::list< ArgIface* > required;
	std::list< ArgIface* > optional;

	size_t maxFlag = 0;
	size_t maxName = 0;

	bool requiredAllOfGroup = false;

	std::function< void( ArgIface* ) > findArgs = [ & ] ( ArgIface * arg )
		{
			const bool tmp = requiredAllOfGroup;

			GroupIface * g = dynamic_cast< GroupIface* > ( arg );

			if( g )
			{
				if( g->isRequired() && dynamic_cast< AllOfGroup* > ( g ) )
					requiredAllOfGroup = true;
				else
					requiredAllOfGroup = false;

				std::for_each( g->children().cbegin(),
					g->children().cend(), findArgs );

				requiredAllOfGroup = tmp;
			}
			else
			{
				if( arg->isRequired() || requiredAllOfGroup )
					required.push_back( arg );
				else
					optional.push_back( arg );

				calcMaxFlagAndName( arg, maxFlag, maxName );
			}


		};

	std::for_each( m_cmdLine->arguments().cbegin(),
		m_cmdLine->arguments().cend(),
		findArgs );

	maxFlag += 2;
	maxName += 2;

	const size_t beforeDescription = maxFlag + 1 + maxName + 2;

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

	printString( to, splitToWords( m_appDescription ), 0, 0, 0 );

	to << std::endl << std::endl;

	to << "Usage: ";

	printString( to, usage, 7, 7, 7 );

	to << std::endl << std::endl;

	std::function< void ( ArgIface* ) > printArg =
		[ & ] ( ArgIface * arg )
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

			printOffset( to, pos, beforeDescription );
			pos = beforeDescription;

			printString( to, splitToWords( arg->description() ), pos,
				beforeDescription, 0 );

			to << std::endl << std::endl;
		};

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
	catch( const BaseException & )
	{
		print( to );
	}
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
	m_lineLength = length;
}

inline std::list< std::string >
HelpPrinter::createUsageString( ArgIface * arg, bool required )
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
HelpPrinter::splitToWords( const std::string & s )
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
	size_t currentPos, size_t leftMargin, size_t rightMargin )
{
	size_t maxLineLength = m_lineLength -
		( currentPos < leftMargin ? leftMargin - currentPos : currentPos ) -
		rightMargin;

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

} /* namespace Args */

#endif // ARGS__HELP_PRINTER_HPP__INCLUDED

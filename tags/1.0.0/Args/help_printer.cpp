
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
#include <Args/help_printer.hpp>
#include <Args/cmd_line.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <iostream>
#include <algorithm>
#include <functional>


namespace Args {

//
// HelpPrinter
//

HelpPrinter::HelpPrinter()
	:	m_cmdLine( 0 )
	,	m_lineLength( 79 )
{
}

HelpPrinter::~HelpPrinter()
{
}

static inline void
printOffset( size_t currentPos, size_t leftMargin )
{
	if( currentPos < leftMargin )
		std::cout << std::string( leftMargin - currentPos, ' ' );
}

static inline void
calcMAxFlagAndName( ArgIface * arg, size_t & maxFlag, size_t & maxName )
{
	size_t f = 1;
	size_t n = arg->argumentName().length();

	if( n > 0 )
		n += 3 + arg->valueSpecifier().length();
	else
		f += 3 + arg->valueSpecifier().length();

	if( f > maxFlag )
		maxFlag = f;

	if( n > maxName )
		maxName = n;
}

void
HelpPrinter::print()
{
	std::list< ArgIface* > required;
	std::list< ArgIface* > optional;

	size_t maxFlag = 0;
	size_t maxName = 0;

	std::for_each( m_cmdLine->arguments().begin(),
		m_cmdLine->arguments().end(),
		[ & ] ( ArgIface * arg )
		{
			if( arg->isRequired() )
				required.push_back( arg );
			else
				optional.push_back( arg );

			calcMAxFlagAndName( arg, maxFlag, maxName );
		}
	);

	maxFlag += 2;
	maxName += 2;

	const size_t beforeDescription = maxFlag + 1 + maxName + 2;

	std::list< std::string > usage;

	usage.push_back( m_exeName );

	std::function< void ( ArgIface* ) > createUsageAndAppend =
		[ & ] ( ArgIface * arg )
		{
			const std::list< std::string > words = createUsageString( arg );

			usage.insert( usage.end(), words.begin(), words.end() );
		};

	std::for_each( required.begin(), required.end(),
		createUsageAndAppend );

	std::for_each( optional.begin(), optional.end(),
		createUsageAndAppend );

	printString( splitToWords( m_appDescription ), 0, 0, 0 );

	std::cout << std::endl << std::endl;

	std::cout << "Usage: ";

	printString( usage, 7, 7, 7 );

	std::cout << std::endl << std::endl;

	std::function< void ( ArgIface* ) > printArg =
		[=] ( ArgIface * arg )
		{
			size_t pos = 0;

			if( !arg->flag().empty() )
			{
				std::cout << ' ';
				++pos;
				std::cout << '-' << arg->flag();
				pos += arg->flag().length() + 1;

				if( !arg->argumentName().empty() )
				{
					std::cout << ',';
					++pos;
				}
				else if( arg->isWithValue() )
				{
					std::cout << " <" << arg->valueSpecifier() << '>';
					pos += arg->valueSpecifier().length() + 3;
				}
			}
			else
			{
				printOffset( pos, maxFlag + 1 );
				pos += maxFlag + 1;
			}

			if( !arg->argumentName().empty() )
			{
				std::cout << ' ';
				++pos;
				std::cout << "--" << arg->argumentName();
				pos += arg->argumentName().length() + 2;

				if( arg->isWithValue() )
				{
					std::cout << " <" << arg->valueSpecifier() << '>';
					pos += arg->valueSpecifier().length() + 3;
				}
			}

			printOffset( pos, beforeDescription );
			pos = beforeDescription;

			printString( splitToWords( arg->description() ), pos,
				beforeDescription, 0 );

			std::cout << std::endl << std::endl;
		};

	if( !required.empty() )
	{
		std::cout << "Required arguments:" << std::endl;

		std::for_each( required.begin(), required.end(), printArg );
	}

	if( !optional.empty() )
	{
		std::cout << "Optional arguments:" << std::endl;

		std::for_each( optional.begin(), optional.end(), printArg );
	}
}

void
HelpPrinter::print( const std::string & name )
{
	try {
		ArgIface * arg = m_cmdLine->findArgument( name );

		std::list< std::string > usage = createUsageString( arg );

		std::cout << "Usage: ";

		std::for_each( usage.begin(), usage.end(),
			[] ( const std::string & s )
				{ std::cout << s << ' '; }
		);

		std::cout << std::endl << std::endl;

		printString( splitToWords( arg->longDescription() ),
			0, 7, 7 );
	}
	catch( const BaseException & )
	{
		print();
	}
}

void
HelpPrinter::setExecutable( const std::string & exe )
{
	m_exeName = exe;
}

void
HelpPrinter::setAppDescription( const std::string & desc )
{
	m_appDescription = desc;
}

void
HelpPrinter::setCmdLine( CmdLine * cmd )
{
	m_cmdLine = cmd;
}

void
HelpPrinter::setLineLength( size_t length )
{
	m_lineLength = length;
}

std::list< std::string >
HelpPrinter::createUsageString( ArgIface * arg )
{
	std::list< std::string > result;

	std::string usage;

	if( !arg->isRequired() )
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

	if( !arg->isRequired() )
		usage.append( " ]" );

	result.push_back( usage );

	return result;
}

static inline bool
isSpaceChar( const char & c )
{
	static const std::string spaceChars = " \n\t\r";

	return ( spaceChars.find( c ) != std::string::npos );
}

std::list< std::string >
HelpPrinter::splitToWords( const std::string & s )
{
	std::string word;
	std::list< std::string > result;

	std::for_each( s.begin(), s.end(),
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

void
HelpPrinter::printString( const std::list< std::string > & words,
	size_t currentPos, size_t leftMargin, size_t rightMargin )
{
	size_t maxLineLength = m_lineLength -
		( currentPos < leftMargin ? leftMargin - currentPos : currentPos ) -
		rightMargin;

	size_t length = 0;

	bool makeOffset = ( currentPos < leftMargin );

	std::for_each( words.begin(), words.end(),
		[ & ] ( const std::string & word )
		{
			if( makeOffset )
			{
				printOffset( currentPos, leftMargin );

				makeOffset = false;
			}

			if( length + word.length() < maxLineLength )
			{
				length += word.length();

				std::cout << word;

				if( length < maxLineLength )
				{
					++length;
					std::cout << ' ';
				}
			}
			else
			{
				currentPos = 0;

				length = 0;

				maxLineLength = m_lineLength - leftMargin - rightMargin;

				std::cout << std::endl;

				printOffset( currentPos, leftMargin );

				std::cout << word;

				length += word.length();

				if( length < maxLineLength )
				{
					++length;
					std::cout << ' ';
				}
			}
		}
	);
}

} /* namespace Args */

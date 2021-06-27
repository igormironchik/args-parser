
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

#ifndef ARGS__HELP_PRINTER_HPP__INCLUDED
#define ARGS__HELP_PRINTER_HPP__INCLUDED

// C++ include.
#include <vector>
#include <algorithm>
#include <functional>

// Args include.
#include "utils.hpp"
#include "exceptions.hpp"
#include "group_iface.hpp"
#include "groups.hpp"
#include "command.hpp"
#include "types.hpp"
#include "help_printer_iface.hpp"
#include "cmd_line.hpp"


namespace Args {

class CmdLine;
class ArgIface;


//
// HelpPrinter
//

/*!
	HelpPrinter is a class that prints help.
*/
class HelpPrinter final
	:	public HelpPrinterIface
{
public:
	//! Smart pointer to the argument.
	using ArgPtr = std::unique_ptr< ArgIface, details::Deleter< ArgIface > >;

	HelpPrinter();

	virtual ~HelpPrinter();

	//! Print help for all arguments.
	void print(
		//! Output stream for the printing help.
		OutStreamType & to ) override;

	//! Print help for the given argument.
	void print(
		//! Name of the argument. I.e. "-t" or "--timeout".
		const String & name,
		//! Output stream for the printing help.
		OutStreamType & to,
		//! Parent command if present.
		Command * parent = nullptr ) override;

	//! Set executable name.
	void setExecutable( const String & exe ) override;

	//! Set description for the application.
	void setAppDescription( const String & desc ) override;

	//! Set command line.
	void setCmdLine( CmdLine * cmd ) override;

	//! Set line length for the help.
	void setLineLength( String::size_type length ) override;

	//! \return Argument for the given name.
	ArgIface * findArgument( const String & name ) override
	{
		return m_cmdLine->findArgument( name );
	}

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
	void sortArg( const ArgPtr & arg,
		std::vector< Command* > & commands,
		std::vector< ArgIface* > & required,
		std::vector< ArgIface* > & optional,
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
		arg->argumentName().length() : ( arg->flag().empty() ?
			arg->name().length() : 0 ) );

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
HelpPrinter::sortArg( const ArgPtr & arg,
	std::vector< Command* > & commands,
	std::vector< ArgIface* > & required,
	std::vector< ArgIface* > & optional,
	String::size_type & maxFlag,
	String::size_type & maxName,
	String::size_type & maxCommand,
	bool requiredAllOfGroup ) const
{
	GroupIface * g = dynamic_cast< GroupIface* > ( arg.get() );

	if( arg->type() == ArgType::Command )
	{
		Command * cmd = static_cast< Command* > ( arg.get() );

		commands.push_back( cmd );

		String::size_type length = cmd->name().length() + ( cmd->isWithValue() ?
			3 + cmd->valueSpecifier().length() : 0 );

		if( length > maxCommand )
			maxCommand = length;
	}
	else if( g )
	{
		if( g->isRequired() && g->type() == ArgType::AllOfGroup )
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
			required.push_back( arg.get() );
		else
			optional.push_back( arg.get() );

		calcMaxFlagAndName( arg.get(), maxFlag, maxName );
	}
}

inline void
HelpPrinter::printOnlyFor( ArgIface * arg, OutStreamType & to,
	String::size_type beforeDescription, String::size_type ) const
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
		printOffset( to, pos, 4 );
		pos = 4;
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
	else if( arg->flag().empty() && arg->argumentName().empty() )
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

namespace details {

template< typename T >
bool argNameLess( const T & a1, const T & a2 )
{
	static const Char dash = SL( '-' );

	if( !a1->name().empty() && !a2->name().empty() )
	{
		if( *( a1->name().cbegin() ) != dash &&
				*( a2->name().cbegin() ) == dash )
			return true;
		else if( *( a1->name().cbegin() ) == dash &&
				*( a2->name().cbegin() ) != dash )
			return false;
		else if( a1->argumentName().empty() && !a2->argumentName().empty() )
			return true;
		else if( !a1->argumentName().empty() && a2->argumentName().empty() )
			return false;
		else
			return ( a1->name() < a2->name() );
	}
	else
		return ( a1->name() < a2->name() );
}

} /* namespace details */

inline void
HelpPrinter::print( OutStreamType & to )
{
	std::vector< ArgIface* > required;
	std::vector< ArgIface* > optional;
	std::vector< Command* > commands;

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

	// Sort arguments by name.
	std::sort( required.begin(), required.end(),
		[] ( const auto & a1, const auto & a2 )
			{ return details::argNameLess( a1, a2 ); } );

	std::sort( optional.begin(), optional.end(),
		[] ( const auto & a1, const auto & a2 )
			{ return details::argNameLess( a1, a2 ); } );

	std::sort( commands.begin(), commands.end(),
		[] ( const auto & c1, const auto & c2 )
			{ return details::argNameLess( c1, c2 ); } );

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

				for( const auto & w : details::asConst( words ) )
					usage.push_back( w );
			};

		std::for_each( required.cbegin(), required.cend(),
			createUsageAndAppend );

		requiredFlag = false;

		std::for_each( optional.cbegin(), optional.cend(),
			createUsageAndAppend );

		to << "USAGE: ";

		printString( to, usage, 7, 7, 7 );

		to << "\n" << "\n";
	}
	else
	{
		to << "USAGE: " << m_exeName << " <command>";

		if( !optional.empty() || !required.empty() )
			to << " <options>";

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

		to << "\n";
	}

	auto printArg = std::bind( &HelpPrinter::printOnlyFor, this,
		std::placeholders::_1, std::ref( to ),
			( maxFlag == 1 ? maxName + 6 : ( maxName + 6 > maxFlag + 1 ?
				maxName + 6 : maxFlag + 1 ) ),
		maxFlag );

	if( !required.empty() )
	{
		to << "REQUIRED:" << "\n";

		std::for_each( required.cbegin(), required.cend(), printArg );
	}

	if( !optional.empty() )
	{
		to << "OPTIONAL:" << "\n";

		std::for_each( optional.cbegin(), optional.cend(), printArg );
	}

	to.flush();
}

inline void
HelpPrinter::print( const String & name, OutStreamType & to, Command * parent )
{
	auto * arg = ( parent ? parent->findChild( name ) : m_cmdLine->findArgument( name ) );

	if( parent && !arg )
		arg = m_cmdLine->findArgument( name );

	if( arg && arg->type() == ArgType::Command )
	{
		Command * cmd = static_cast < Command* > ( arg );

		// Prepare global arguments.
		std::vector< ArgIface* > grequired;
		std::vector< ArgIface* > goptional;
		std::vector< Command* > gcommands;

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

		// Sort global arguments by name.
		std::sort( grequired.begin(), grequired.end(),
			[] ( const auto & a1, const auto & a2 )
				{ return details::argNameLess( a1, a2 ); } );

		std::sort( goptional.begin(), goptional.end(),
			[] ( const auto & a1, const auto & a2 )
				{ return details::argNameLess( a1, a2 ); } );

		gmaxFlag += 2;
		gmaxName += 2;

		// Prepare arguments of command.
		std::vector< ArgIface* > required;
		std::vector< ArgIface* > optional;
		std::vector< Command* > commands;

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

		// Sort arguments by name.
		std::sort( required.begin(), required.end(),
			[] ( const auto & a1, const auto & a2 )
				{ return details::argNameLess( a1, a2 ); } );

		std::sort( optional.begin(), optional.end(),
			[] ( const auto & a1, const auto & a2 )
				{ return details::argNameLess( a1, a2 ); } );

		std::sort( commands.begin(), commands.end(),
			[] ( const auto & c1, const auto & c2 )
				{ return details::argNameLess( c1, c2 ); } );

		maxFlag += 2;
		maxName += 2;
		maxCommand += 2;

		// Print.
		printString( to, splitToWords( cmd->longDescription() ), 0, 0, 0 );

		to << "\n\n";

		if( commands.empty() )
		{
			to << "USAGE: " << name;

			if( cmd->isWithValue() )
				to << " <" << cmd->valueSpecifier() << ">";

			if( !grequired.empty() || !goptional.empty() ||
				!required.empty() || !optional.empty() )
					to << " <options>";

			to << "\n\n";
		}
		else
		{
			to << "USAGE: " << name << " <command>";

			if( !optional.empty() || !required.empty() )
				to << " <options>";

			to << "\n\n";

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

			to << "\n";
		}

		// Print command's arguments.
		auto printArg = std::bind( &HelpPrinter::printOnlyFor, this,
			std::placeholders::_1, std::ref( to ),
			( maxFlag == 1 ? maxName + 6 : ( maxName + 6 > maxFlag + 1 ?
				maxName + 6 : maxFlag + 1 ) ),
			maxFlag );

		if( !required.empty() )
		{
			to << "REQUIRED:" << "\n";

			std::for_each( required.cbegin(), required.cend(), printArg );
		}

		if( !optional.empty() )
		{
			to << "OPTIONAL:" << "\n";

			std::for_each( optional.cbegin(), optional.cend(), printArg );
		}

		// Print global arguments.
		if( !grequired.empty() || !goptional.empty() )
		{
			auto printGlobalArg = std::bind( &HelpPrinter::printOnlyFor, this,
				std::placeholders::_1, std::ref( to ),
				( gmaxFlag == 1 ? gmaxName + 6 : ( gmaxName + 6 > gmaxFlag + 1 ?
					gmaxName + 6 : gmaxFlag + 1 ) ), gmaxFlag );

			if( !grequired.empty() )
			{
				to << "GLOBAL REQUIRED:" << "\n";

				std::for_each( grequired.cbegin(), grequired.cend(),
					printGlobalArg );
			}

			if( !goptional.empty() )
			{
				to << "GLOBAL OPTIONAL:" << "\n";

				std::for_each( goptional.cbegin(), goptional.cend(),
					printGlobalArg );
			}
		}

		to.flush();
	}
	else if( arg )
		print( arg, to );
	else
		print( to );
}

inline void
HelpPrinter::print( ArgIface * arg, OutStreamType & to ) const
{
	StringList usage = createUsageString( arg,
		arg->isRequired() );

	to << "USAGE: ";

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
		usage.append( SL( "[ " ) );

	if( !arg->flag().empty() )
	{
		usage.append( SL( "-" ) );
		usage.append( arg->flag() );

		if( !arg->argumentName().empty() )
		{
			usage.append( SL( "," ) );

			result.push_back( usage );

			usage.clear();
		}
	}

	if( !arg->argumentName().empty() )
	{
		usage.append( SL( "--" ) );
		usage.append( arg->argumentName() );
	}
	else if( arg->flag().empty() && arg->argumentName().empty() )
		usage.append( arg->name() );

	if( arg->isWithValue() )
	{
		usage.append( SL( " <" ) );
		usage.append( arg->valueSpecifier() );
		usage.append( SL( ">" ) );
	}

	if( !required )
		usage.append( SL( " ]" ) );

	result.push_back( usage );

	return result;
}

static inline bool
isSpaceChar( const Char & c )
{
	static const String spaceChars = SL( " \n\t\r" );

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

} /* namespace Args */

#endif // ARGS__HELP_PRINTER_HPP__INCLUDED


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

#ifndef ARGS__HELP_HPP__INCLUDED
#define ARGS__HELP_HPP__INCLUDED

// Args include.
#include <Args/arg.hpp>
#include <Args/help_printer.hpp>
#include <Args/context.hpp>
#include <Args/utils.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <iostream>


namespace Args {

class CmdLine;


//
// Help
//

//! Help argument.
class Help final
	:	public Arg
{
public:
	explicit Help( CmdLine * cmd,
		bool throwExceptionOnPrint = true );

	//! \return Help printer.
	HelpPrinter & printer();

	//! Set executable name.
	void setExecutable( const std::string & exe );

	//! Set description for the application.
	void setAppDescription( const std::string & desc );

	//! Set line length for the help.
	void setLineLength( size_t length );

protected:
	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	void process(
		//! Context of the command line.
		Context & context ) override;

private:
	//! Printer.
	HelpPrinter m_printer;
	//! Throw or not exception?
	bool m_throwExceptionOnPrint;
}; // class Help


//
// Help
//

inline
Help::Help( CmdLine * cmd, bool throwExceptionOnPrint )
	:	Arg( 'h', "help", true )
	,	m_throwExceptionOnPrint( throwExceptionOnPrint )
{
	setDescription( "Print this help." );
	setLongDescription( "Print this help." );

	m_printer.setCmdLine( cmd );
}

inline HelpPrinter &
Help::printer()
{
	return m_printer;
}

inline void
Help::setExecutable( const std::string & exe )
{
	m_printer.setExecutable( exe );
}

inline void
Help::setAppDescription( const std::string & desc )
{
	m_printer.setAppDescription( desc );
}

inline void
Help::setLineLength( size_t length )
{
	m_printer.setLineLength( length );
}

inline void
Help::process( Context & context )
{
	if( !context.atEnd() )
	{
		const std::string arg = *context.next();

		if( isArgument( arg ) || isFlag( arg ) )
			m_printer.print( arg, std::cout );
		else
		{
			context.putBack();

			m_printer.print( std::cout );
		}
	}
	else
		m_printer.print( std::cout );

	setDefined( true );

	if( m_throwExceptionOnPrint )
		throw HelpHasBeenPrintedException();
}

} /* namespace Args */

#endif // ARGS__HELP_HPP__INCLUDED


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

#ifndef ARGS__HELP_HPP__INCLUDED
#define ARGS__HELP_HPP__INCLUDED

// Args include.
#include "arg.hpp"
#include "help_printer_iface.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "exceptions.hpp"
#include "types.hpp"

// C++ include.
#include <memory>

#ifdef ARGS_TESTING
	#ifndef ARGS_QSTRING_BUILD
		// C++ include.
		#include <sstream>
	#endif
#endif


namespace Args {

#ifdef ARGS_TESTING
	#ifdef ARGS_WSTRING_BUILD
		extern std::wstringstream g_argsOutStream;
	#elif defined( ARGS_QSTRING_BUILD )
		extern OutStreamType g_argsOutStream;
	#else
		extern std::stringstream g_argsOutStream;
	#endif
#else
	OutStreamType & g_argsOutStream = outStream();
#endif // ARGS_TESTING


//
// Help
//

//! Help argument.
class Help final
	:	public Arg
{
public:
	explicit Help( bool throwExceptionOnPrint = true );

	//! Set executable name.
	void setExecutable( const String & exe );

	//! Set description for the application.
	void setAppDescription( const String & desc );

	//! Set line length for the help.
	void setLineLength( String::size_type length );

protected:
	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	void process(
		//! Context of the command line.
		Context & context ) override;

	//! Set command line parser.
	virtual void setCmdLine( CmdLine * cmdLine )
	{
		Arg::setCmdLine( cmdLine );

		m_printer->setCmdLine( cmdLine );
	}

private:
	//! Printer.
	std::unique_ptr< HelpPrinterIface > m_printer;
	//! Throw or not exception?
	bool m_throwExceptionOnPrint;
}; // class Help


//
// Help
//

inline void
Help::setExecutable( const String & exe )
{
	m_printer->setExecutable( exe );
}

inline void
Help::setAppDescription( const String & desc )
{
	m_printer->setAppDescription( desc );
}

inline void
Help::setLineLength( String::size_type length )
{
	m_printer->setLineLength( length );
}

inline void
Help::process( Context & context )
{
	if( !context.atEnd() )
	{
		const String arg = *context.next();

		// Argument or flag.
		if( details::isArgument( arg ) || details::isFlag( arg ) )
			m_printer->print( arg, g_argsOutStream );
		// Command?
		else
		{
			auto * tmp = m_printer->findArgument( arg );

			// Command.
			if( tmp->type() == ArgType::Command )
			{
				if( !context.atEnd() )
					m_printer->print( dynamic_cast< Command* > ( tmp ),
						*context.next(), g_argsOutStream );
				else
					m_printer->print( arg, g_argsOutStream );
			}
			else if( tmp )
				m_printer->print( arg, g_argsOutStream );
			else
				m_printer->print( g_argsOutStream );
		}
	}
	else
		m_printer->print( g_argsOutStream );

	setDefined( true );

	if( m_throwExceptionOnPrint )
		throw HelpHasBeenPrintedException();
}

} /* namespace Args */

#endif // ARGS__HELP_HPP__INCLUDED

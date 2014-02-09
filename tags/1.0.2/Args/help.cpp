
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
#include <Args/help.hpp>
#include <Args/context.hpp>
#include <Args/utils.hpp>
#include <Args/exceptions.hpp>


namespace Args {

//
// Help
//

Help::Help( CmdLine * cmd, bool throwExceptionOnPrint )
	:	Arg( 'h', "help", true )
	,	m_throwExceptionOnPrint( throwExceptionOnPrint )
{
	setDescription( "Print this help." );
	setLongDescription( "Print this help." );

	m_printer.setCmdLine( cmd );
}

HelpPrinter &
Help::printer()
{
	return m_printer;
}

void
Help::process( Context & context )
{
	if( !context.atEnd() )
	{
		const std::string arg = *context.next();

		if( isArgument( arg ) || isFlag( arg ) )
			m_printer.print( arg );
		else
		{
			context.putBack();

			m_printer.print();
		}
	}
	else
		m_printer.print();

	setDefined( true );

	if( m_throwExceptionOnPrint )
		throw HelpHasBeenPrintedException();
}

} /* namespace Args */

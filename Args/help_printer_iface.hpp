
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

#ifndef ARGS__HELP_PRINTER_IFACE_HPP__INCLUDED
#define ARGS__HELP_PRINTER_IFACE_HPP__INCLUDED

// Args include.
#include "utils.hpp"


namespace Args {

class CmdLine;
class Command;
class ArgIface;


//
// HelpPrinterIface
//

/*!
	Interface for HelpPrinter.
*/
class HelpPrinterIface {
public:
	HelpPrinterIface()
	{
	}

	virtual ~HelpPrinterIface()
	{
	}

	//! Print help for all arguments.
	virtual void print(
		//! Output stream for the printing help.
		OutStreamType & to ) = 0;

	//! Print help for the given argument.
	virtual void print(
		//! Name of the argument. I.e. "-t" or "--timeout".
		const String & name,
		//! Output stream for the printing help.
		OutStreamType & to,
		//! Parent command if present.
		Command * parent = nullptr ) = 0;

	//! Set executable name.
	virtual void setExecutable( const String & exe ) = 0;

	//! Set description for the application.
	virtual void setAppDescription( const String & desc ) = 0;

	//! Set command line.
	virtual void setCmdLine( CmdLine * cmd ) = 0;

	//! Set line length for the help.
	virtual void setLineLength( String::size_type length ) = 0;

	//! \return Argument for the given name.
	virtual ArgIface * findArgument( const String & name ) = 0;

private:
	DISABLE_COPY( HelpPrinterIface )
}; // class HelpPrinterIface

} /* namespace Args */

#endif // ARGS__HELP_PRINTER_IFACE_HPP__INCLUDED

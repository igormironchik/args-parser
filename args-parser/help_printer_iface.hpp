
/*
	SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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

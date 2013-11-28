
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

#ifndef ARGS__HELP_PRINTER_HPP__INCLUDED
#define ARGS__HELP_PRINTER_HPP__INCLUDED

// C++ include.
#include <string>
#include <list>

// Args include.
#include <Args/utils.hpp>


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
	virtual void print();

	//! Print help for the given \arg name argument.
	virtual void print(
		//! Name of the argument. I.e. "-t" or "--timeout".
		const std::string & name );

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
	std::list< std::string > createUsageString( ArgIface * arg );
	//! List of words from string.
	std::list< std::string > splitToWords( const std::string & s );
	//! Print string with given margins.
	void printString( const std::list< std::string > & words,
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

} /* namespace Args */

#endif // ARGS__HELP_PRINTER_HPP__INCLUDED

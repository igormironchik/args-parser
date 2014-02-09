
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

#ifndef ARGS__HELP_HPP__INCLUDED
#define ARGS__HELP_HPP__INCLUDED

// Args include.
#include <Args/arg.hpp>
#include <Args/help_printer.hpp>


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

protected:
	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	virtual void process(
		//! Context of the command line.
		Context & context );

private:
	//! Printer.
	HelpPrinter m_printer;
	//! Throw or not exception?
	bool m_throwExceptionOnPrint;
}; // class Help

} /* namespace Args */

#endif // ARGS__HELP_HPP__INCLUDED


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

#ifndef ARGS__CMD_LINE_HPP__INCLUDED
#define ARGS__CMD_LINE_HPP__INCLUDED

// Args include.
#include <Args/utils.hpp>
#include <Args/context.hpp>
#include <Args/arg_iface.hpp>

// C++ include.
#include <list>


namespace Args {

//
// CmdLine
//

/*!
	CmdLine is class that holds all rguments and parse
	command line arguments in the correspondence with holded
	arguments.
*/
class CmdLine final {
public:
	CmdLine( int argc, char ** argv );

	//! Add argument.
	void addArg( ArgIface * arg );

	//! Parse arguments.
	void parse();

private:
	//! Check correctness of the arguments before parsing.
	void checkCorrectnessBeforeParsing() const;
	//! Check correctness of the arguments after parsing.
	void checkCorrectnessAfterParsing() const;
	//! \return Argument for the given name.
	ArgIface * findArgument( const std::string & name );

private:
	DISABLE_COPY( CmdLine )

	// Context.
	Context m_context;
	//! Arguments.
	std::list< ArgIface* > m_args;
}; // class CmdLine

} /* namespace Args */

#endif // ARGS__CMD_LINE_HPP__INCLUDED

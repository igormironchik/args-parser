
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

#ifndef ARGS__ARG_IFACE_HPP__INCLUDED
#define ARGS__ARG_IFACE_HPP__INCLUDED

// C++ include.
#include <string>
#include <list>


namespace Args {

class Context;


//
// ArgIface
//

//! Interface for arguments.
class ArgIface {

	friend class CmdLine;

public:
	virtual ~ArgIface();

	/*!
		\return Name of the argument.

		If name is empty returned value should be a flag.
		I.e. for example "-t" or "--timeout"
	*/
	virtual std::string name() const = 0;

	//! \return Is this argument with value?
	virtual bool isWithValue() const = 0;

	//! \return Is this argument required?
	virtual bool isRequired() const = 0;

	//! \return Flag.
	virtual const std::string & flag() const = 0;

	//! \return Argument name.
	virtual const std::string & argumentName() const = 0;

	//! \return Value specifier.
	virtual const std::string & valueSpecifier() const = 0;

	//! \return Description of the argument.
	virtual const std::string & description() const = 0;

	//! \return Long description of the argument.
	virtual const std::string & longDescription() const = 0;

protected:
	/*!
		\return Argument for the given name.

		\retval Pointer to the ArgIface if this argument handles
			argument with the given name.
		\retval nullptr if this argument doesn't know about
			argument with name.
	*/
	virtual ArgIface * isItYou(
		/*!
			Name of the argument. Can be for example "-t" or
			"--timeout".
		*/
		const std::string & name ) = 0;

	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	virtual void process(
		//! Context of the command line.
		Context & context ) = 0;

	/*!
		Check correctness of the argument before parsing.

		Implementation of this method must add his flag
		and name to the flags and names.
	*/
	virtual void checkCorrectnessBeforeParsing(
		//! All known flags.
		std::list< std::string > & flags,
		//! All known names.
		std::list< std::string > & names ) const = 0;

	//! Check correctness of the argument after parsing.
	virtual void checkCorrectnessAfterParsing() const = 0;
}; // class ArgIface


//
// ArgIface
//

inline
ArgIface::~ArgIface()
{
}

} /* namespace Args */

#endif // ARGS__ARG_IFACE_HPP__INCLUDED

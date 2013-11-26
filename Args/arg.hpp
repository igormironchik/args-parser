
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

#ifndef ARGS__ARG_HPP__INCLUDED
#define ARGS__ARG_HPP__INCLUDED

// Args include.
#include <Args/arg_iface.hpp>
#include <Args/utils.hpp>


namespace Args {

//
// Arg
//

/*!
	Argument with one value that can be present only once
	in the command line
*/
class Arg
	:	public ArgIface
{
public:
	//! Construct argument with flag \arg flag and name \arg name.
	Arg(
		//! Flag for this argument.
		const char & flag,
		//! Name for this argument.
		const std::string & name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	//! Construct argument only with flag \arg flag, without name.
	explicit Arg(
		//! Flag for this argument.
		const char & flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	//! Construct argument only with name \arg name, without flag.
	explicit Arg(
		//! Name for this argument.
		const std::string & name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	virtual ~Arg();

	/*!
		\return Name of the argument.

		If name is empty returned value should be a flag.
		I.e. for example "-t" or "--timeout"
	*/
	virtual const std::string & name() const;

	//! \return Is this argument with value?
	virtual bool isWithValue() const;

	//! \return Is this argument required?
	virtual bool isRequired() const;

	//! \return Is this argument defined?
	virtual bool isDefined() const;

	//! \return Value of this argument.
	virtual const std::string & value() const;

protected:
	/*!
		\return Argument for the given \arg name name.

		\retval Pointer to the ArgIface if this argument handles
			argument with the given \arg name name.
		\retval nullptr if this argument doesn't know about
			argument with name \arg name.
	*/
	virtual ArgIface * isItYou(
		/*!
			Name of the argument. Can be for example "-t" or
			"--timeout".
		*/
		const std::string & name );

	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	virtual void process(
		//! Context of the command line.
		Context & context );

	/*!
		Check correctness of the argument before parsing.

		Implementation of this method must add his flag
		and name to the \arg flags and \arg names.
	*/
	virtual void checkCorrectnessBeforeParsing(
		//! All known flags.
		std::list< std::string > & flags,
		//! All known names.
		std::list< std::string > & names ) const;

	//! Check correctness of the argument after parsing.
	virtual void checkCorrectnessAfterParsing() const;

private:
	DISABLE_COPY( Arg )

	//! Is this argument with value?
	bool m_isWithValue;
	//! Is this argument required?
	bool m_isRequired;
	//! Is this argument defined?
	bool m_isDefined;
	//! Value of this argument.
	std::string m_value;
	//! Flag of this argument.
	std::string m_flag;
	//! Name of this argument.
	std::string m_name;
}; // class ArgIface

} /* namespace Args */

#endif // ARGS__ARG_HPP__INCLUDED

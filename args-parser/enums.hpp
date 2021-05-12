
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

#ifndef ARGS__ENUMS_HPP__INCLUDED
#define ARGS__ENUMS_HPP__INCLUDED

namespace Args {

//
// ValueOptions
//

//! Options for value property.
enum class ValueOptions {
	//! No values.
	NoValue = 0,
	//! One value.
	OneValue = 1,
	//! Many values.
	ManyValues = 2
}; // enum class ValueOptions


//
// ArgType
//

//! Type of the argument.
enum class ArgType {
	//! Command.
	Command,
	//! Argument.
	Arg,
	//! Multi argument
	MultiArg,
	//! "Only one" group.
	OnlyOneGroup,
	//! "All of" group.
	AllOfGroup,
	//! "At least one" group.
	AtLeastOneGroup
}; // enum class ArgType

} /* namespace Args */

#endif // ARGS__ENUMS_HPP__INCLUDED


/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__ENUMS_HPP__INCLUDED
#define ARGS__ENUMS_HPP__INCLUDED

namespace Args
{

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

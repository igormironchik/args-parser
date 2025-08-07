
/*
    SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__ARG_IFACE_HPP__INCLUDED
#define ARGS__ARG_IFACE_HPP__INCLUDED

// Args include.
#include "enums.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace Args
{

class Context;
class CmdLine;
class GroupIface;
class Command;

//
// ArgIface
//

//! Interface for arguments.
class ArgIface
{
    friend class CmdLine;
    friend class GroupIface;
    friend class Command;

public:
    ArgIface()
        : m_cmdLine(nullptr)
    {
    }

    virtual ~ArgIface()
    {
    }

    //! \return Type of the argument.
    virtual ArgType type() const = 0;

    /*!
        \return Name of the argument.

        If name is empty returned value should be a flag.
        I.e. for example "-t" or "--timeout"
    */
    virtual String name() const = 0;

    //! \return Is this argument with value?
    virtual bool isWithValue() const = 0;

    //! \return Is this argument required?
    virtual bool isRequired() const = 0;

    //! \return Is this argument defined?
    virtual bool isDefined() const = 0;

    //! \return Flag.
    virtual const String &flag() const = 0;

    //! \return Argument name.
    virtual const String &argumentName() const = 0;

    //! \return Value specifier.
    virtual const String &valueSpecifier() const = 0;

    //! \return Description of the argument.
    virtual const String &description() const = 0;

    //! \return Long description of the argument.
    virtual const String &longDescription() const = 0;

    //! \return Command line parser.
    CmdLine *cmdLine() const
    {
        return m_cmdLine;
    }

    //! \return Is given name a misspelled name of the argument.
    virtual bool isMisspelledName(
        //! Name to check (misspelled).
        const String &name,
        //! List of possible names for the given misspelled name.
        StringList &possibleNames) const = 0;

    //! Clear state of the argument.
    virtual void clear() = 0;

protected:
    /*!
        \return Argument for the given name.

        \retval Pointer to the ArgIface if this argument handles
            argument with the given name.
        \retval nullptr if this argument doesn't know about
            argument with name.
    */
    virtual ArgIface *findArgument(
        /*!
            Name of the argument. Can be for example "-t" or
            "--timeout".
        */
        const String &name) = 0;

    /*!
        Process argument's staff, for example take values from
        context. This method invokes exactly at that moment when
        parser has found this argument.
    */
    virtual void process(
        //! Context of the command line.
        Context &context) = 0;

    /*!
        Check correctness of the argument before parsing.

        Implementation of this method must add his flag
        and name to the flags and names.
    */
    virtual void checkCorrectnessBeforeParsing(
        //! All known flags.
        StringList &flags,
        //! All known names.
        StringList &names) const = 0;

    //! Check correctness of the argument after parsing.
    virtual void checkCorrectnessAfterParsing() const = 0;

    //! Set command line parser.
    virtual void setCmdLine(CmdLine *cmdLine)
    {
        m_cmdLine = cmdLine;
    }

private:
    DISABLE_COPY(ArgIface)

    //! Command line parser.
    CmdLine *m_cmdLine;
}; // class ArgIface

} /* namespace Args */

#endif // ARGS__ARG_IFACE_HPP__INCLUDED

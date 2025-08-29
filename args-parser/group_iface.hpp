
/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__GROUP_IFACE_HPP__INCLUDED
#define ARGS__GROUP_IFACE_HPP__INCLUDED

// Args include.
#include "arg_iface.hpp"
#include "exceptions.hpp"
#include "types.hpp"
#include "utils.hpp"

// C++ include.
#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace Args
{

class Command;

//
// GroupIface
//

//! Interface for the groups.
class GroupIface : public ArgIface
{
public:
    //! Smart pointer to the argument.
    using ArgPtr = std::unique_ptr<ArgIface, details::Deleter<ArgIface>>;
    //! List of child arguments.
    using Arguments = std::vector<ArgPtr>;

    template<typename T>
    explicit GroupIface(T &&name,
                        bool required = false)
        : m_name(std::forward<T>(name))
        , m_required(required)
    {
    }

    virtual ~GroupIface()
    {
    }

    //! \return List of children.
    const Arguments &children() const
    {
        return m_children;
    }

    //! Add argument. \note Developer should handle lifetime of the argument.
    void addArg(ArgIface &arg)
    {
        addArg(ArgPtr(&arg, details::Deleter<ArgIface>(false)));
    }

    //! Add argument. \note Developer should handle lifetime of the argument.
    void addArg(ArgIface *arg)
    {
        addArg(ArgPtr(arg, details::Deleter<ArgIface>(false)));
    }

    //! Add argument.
    virtual void addArg(ArgPtr arg)
    {
        if (std::find(m_children.cbegin(), m_children.cend(), arg) == m_children.cend()) {
            if (cmdLine()) {
                arg->setCmdLine(cmdLine());
            }

            m_children.push_back(std::move(arg));
        }
    }

    /*!
        \return Name of the argument.

        If name is empty returned value should be a flag.
        I.e. for example "-t" or "--timeout"
    */
    String name() const override
    {
        return m_name;
    }

    //! \return Is this argument with value?
    bool isWithValue() const override
    {
        return false;
    }

    //! \return Is this argument required?
    bool isRequired() const override
    {
        return m_required;
    }

    //! Set required flag.
    virtual void setRequired(bool on = true)
    {
        m_required = on;
    }

    //! \return Flag.
    const String &flag() const override
    {
        return details::DummyString<>::c_string;
    }

    //! \return Argument name.
    const String &argumentName() const override
    {
        return details::DummyString<>::c_string;
    }

    //! \return Value specifier.
    const String &valueSpecifier() const override
    {
        return details::DummyString<>::c_string;
    }

    //! \return Description of the argument.
    const String &description() const override
    {
        return details::DummyString<>::c_string;
    }

    //! \return Long description of the argument.
    const String &longDescription() const override
    {
        return details::DummyString<>::c_string;
    }

    //! Clear state of the argument.
    void clear() override
    {
        std::for_each(children().begin(), children().end(), [&](const auto &ch) {
            ch->clear();
        });
    }

protected:
    /*!
        \return Argument for the given name.

        \retval Pointer to the ArgIface if this argument handles
            argument with the given name.
        \retval nullptr if this argument doesn't know about
            argument with name.

        \note Looks only in children.
    */
    ArgIface *findArgument(
        /*!
            Name of the argument. Can be for example "-t" or
            "--timeout".
        */
        const String &name) override
    {
        for (const auto &arg : details::asConst(m_children)) {
            ArgIface *tmp = arg->findArgument(name);

            if (tmp != nullptr) {
                return tmp;
            }
        }

        return nullptr;
    }

    /*!
        Process argument's staff, for example take values from
        context. This method invokes exactly at that moment when
        parser has found this argument.
    */
    void process(
        //! Context of the command line.
        Context &) override
    {
    }

    /*!
        Check correctness of the argument before parsing.

        Implementation of this method must add his flag
        and name to the flags and names.
    */
    void checkCorrectnessBeforeParsing(
        //! All known flags.
        StringList &flags,
        //! All known names.
        StringList &names) const override
    {
        for (const auto &arg : details::asConst(m_children)) {
            arg->checkCorrectnessBeforeParsing(flags, names);
        }
    }

    //! Check correctness of the argument after parsing.
    void checkCorrectnessAfterParsing() const override
    {
        for (const auto &arg : details::asConst(m_children)) {
            arg->checkCorrectnessAfterParsing();
        }

        if (isRequired() && !isDefined()) {
            throw BaseException(String(SL("Not defined required argument \"")) + name() + SL("\""));
        }
    }

    //! Set command line parser.
    void setCmdLine(CmdLine *cmdLine) override
    {
        ArgIface::setCmdLine(cmdLine);

        for (const auto &arg : details::asConst(m_children)) {
            arg->setCmdLine(cmdLine);
        }
    }

    //! \return Is given name a misspelled name of the argument.
    bool isMisspelledName(
        //! Name to check (misspelled).
        const String &name,
        //! List of possible names for the given misspelled name.
        StringList &possibleNames) const override
    {
        bool ret = false;

        std::for_each(children().cbegin(), children().cend(), [&](const auto &ch) {
            if (ch->isMisspelledName(name, possibleNames))
                ret = true;
        });

        return ret;
    }

protected:
    //! List of children.
    Arguments m_children;

private:
    DISABLE_COPY(GroupIface)

    //! Name.
    String m_name;
    //! Is required?
    bool m_required;
}; // class GroupIface

} /* namespace Args */

#endif // ARGS__GROUP_IFACE_HPP__INCLUDED

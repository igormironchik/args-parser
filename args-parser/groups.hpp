
/*
    SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

/*!
    \file

    \warning If you are including this file you should include command.hpp too.
*/

#ifndef ARGS__GROUPS_HPP__INCLUDED
#define ARGS__GROUPS_HPP__INCLUDED

// Args include.
#include "group_iface.hpp"
#include "types.hpp"

// C++ include.
#include <algorithm>

namespace Args
{

//
// OnlyOneGroup
//

//! Group of args where only one argument can be defined.
class OnlyOneGroup final : public GroupIface
{
public:
    template<typename T>
    explicit OnlyOneGroup(T &&name,
                          bool required = false)
        : GroupIface(std::forward<T>(name),
                     required)
    {
    }

    virtual ~OnlyOneGroup()
    {
    }

    //! \return Type of the argument.
    ArgType type() const override
    {
        return ArgType::OnlyOneGroup;
    }

    //! \return Is this argument defined?
    bool isDefined() const override
    {
        return std::any_of(children().cbegin(), children().cend(), [](const auto &arg) {
            return arg->isDefined();
        });
    }

protected:
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
        GroupIface::checkCorrectnessBeforeParsing(flags, names);

        for (const auto &arg : details::asConst(children())) {
            if (arg->isRequired()) {
                throw BaseException(String(SL("Required argument \""))
                                    + arg->name()
                                    + SL("\" is not allowed to be in OnlyOne group \"")
                                    + name()
                                    + SL("\"."));
            }
        }
    }

    //! Check correctness of the argument after parsing.
    void checkCorrectnessAfterParsing() const override
    {
        GroupIface::checkCorrectnessAfterParsing();

        ArgIface *defined = nullptr;

        for (const auto &arg : details::asConst(children())) {
            if (arg->isDefined()) {
                if (defined) {
                    throw BaseException(String(SL("Only one argument can "
                                                  "be defined in OnlyOne group \""))
                                        + name()
                                        + SL("\". ")
                                        + SL("Whereas defined \"")
                                        + defined->name()
                                        + SL("\" and \"")
                                        + arg->name()
                                        + SL("\"."));
                } else {
                    defined = arg.get();
                }
            }
        }
    }
}; // class OnlyOneGroup

//
// AllOfGroup
//

//! Group of args where all arguments should be defined.
class AllOfGroup final : public GroupIface
{
public:
    template<typename T>
    explicit AllOfGroup(T &&name,
                        bool required = false)
        : GroupIface(std::forward<T>(name),
                     required)
    {
    }

    virtual ~AllOfGroup()
    {
    }

    //! \return Type of the argument.
    ArgType type() const override
    {
        return ArgType::AllOfGroup;
    }

    //! \return Is this argument defined?
    bool isDefined() const override
    {
        return !std::any_of(children().cbegin(), children().cend(), [](const auto &arg) {
            return !arg->isDefined();
        });
    }

protected:
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
        GroupIface::checkCorrectnessBeforeParsing(flags, names);

        for (const auto &arg : details::asConst(children())) {
            if (arg->isRequired()) {
                throw BaseException(String(SL("Required argument \""))
                                    + arg->name()
                                    + SL("\" is not allowed to ")
                                    + SL("be in AllOf group \"")
                                    + name()
                                    + SL("\"."));
            }
        }
    }

    //! Check correctness of the argument after parsing.
    void checkCorrectnessAfterParsing() const override
    {
        GroupIface::checkCorrectnessAfterParsing();

        const bool defined = std::any_of(children().cbegin(), children().cend(), [](const auto &arg) {
            return arg->isDefined();
        });

        const bool all = std::all_of(children().cbegin(), children().cend(), [](const auto &arg) {
            return arg->isDefined();
        });

        if (defined && !all) {
            throw BaseException(String(SL("All arguments in "
                                          "AllOf group \""))
                                + name()
                                + SL("\" should be defined."));
        }
    }
}; // class AllOfGroup

//
// AtLeastOneGroup
//

//! Group of args where at least one argument should be defined.
class AtLeastOneGroup final : public GroupIface
{
public:
    template<typename T>
    explicit AtLeastOneGroup(T &&name,
                             bool required = false)
        : GroupIface(std::forward<T>(name),
                     required)
    {
    }

    virtual ~AtLeastOneGroup()
    {
    }

    //! \return Type of the argument.
    ArgType type() const override
    {
        return ArgType::AtLeastOneGroup;
    }

    //! \return Is this argument defined?
    bool isDefined() const override
    {
        return std::any_of(children().cbegin(), children().cend(), [](const auto &arg) {
            return arg->isDefined();
        });
    }

protected:
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
        GroupIface::checkCorrectnessBeforeParsing(flags, names);

        for (const auto &arg : details::asConst(children())) {
            if (arg->isRequired()) {
                throw BaseException(String(SL("Required argument \""))
                                    + arg->name()
                                    + SL("\" is not allowed to ")
                                    + SL("be in AtLeastOne group \"")
                                    + name()
                                    + SL("\"."));
            }
        }
    }
}; // class AtLeastOneGroup

} /* namespace Args */

#endif // ARGS__GROUPS_HPP__INCLUDED

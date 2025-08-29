
/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__EXCEPTIONS_HPP__INCLUDED
#define ARGS__EXCEPTIONS_HPP__INCLUDED

// C++ include.
#include <stdexcept>

// Args include.
#include "types.hpp"

namespace Args
{

//
// BaseException
//

//! Base exception of the library.
class BaseException : public std::logic_error
{
public:
    explicit BaseException(String what)
        : std::logic_error("Please use desc() method of the exception.")
        , m_what(std::move(what))
    {
    }

    virtual ~BaseException() noexcept
    {
    }

    //! \return What as std::wstring.
    const String &desc() const noexcept
    {
        return m_what;
    }

private:
    //! What happened?
    String m_what;
}; // class BaseException

//
// HelpHasBeenPrintedException
//

//! This exception notifies about that help has been printed.
class HelpHasBeenPrintedException final : public BaseException
{
public:
    HelpHasBeenPrintedException()
        : BaseException(SL("Help has been printed."))
    {
    }
}; // class HelpHasBeenPrintedException

} /* namespace Args */

#endif // ARGS__EXCEPTIONS_HPP__INCLUDED

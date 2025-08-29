
/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__VALUE_UTILS_HPP__INCLUDED
#define ARGS__VALUE_UTILS_HPP__INCLUDED

// Args include.
#include "exceptions.hpp"
#include "types.hpp"
#include "utils.hpp"

// C++ include.
#include <algorithm>

namespace Args
{

//
// eatValues
//

//! Eat values in context.
template<typename Container,
         typename Cmd,
         typename Ctx>
bool eatValues(Ctx &context,
               Container &container,
               const String &errorDescription,
               Cmd *cmdLine)
{
    if (!context.atEnd()) {
        auto begin = context.begin();

        auto last = std::find_if(context.begin(), context.end(), [&](const String &v) -> bool {
            return (cmdLine->findArgument(v) != nullptr);
        });

        if (last != begin) {
            begin = context.next();

            while (begin != last) {
                container.push_back(*begin);

                begin = context.next();
            }

            if (last != context.end()) {
                context.putBack();
            }

            return true;
        }
    }

    throw BaseException(errorDescription);
}

//
// eatOneValue
//

//! Eat one value.
template<typename Cmd,
         typename Ctx>
String eatOneValue(Ctx &context,
                   const String &errorDescription,
                   Cmd *cmdLine)
{
    if (!context.atEnd()) {
        auto val = context.next();

        if (!cmdLine->findArgument(*val)) {
            return *val;
        }

        context.putBack();
    }

    throw BaseException(errorDescription);
}

} /* namespace Args */

#endif // ARGS__VALUE_UTILS_HPP__INCLUDED

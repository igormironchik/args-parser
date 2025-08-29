
/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__UTILS_HPP__INCLUDED
#define ARGS__UTILS_HPP__INCLUDED

// Args include.
#include "types.hpp"

// C++ include.
#include <algorithm>
#include <type_traits>

namespace Args
{

//
// DISABLE_COPY
//

//! Macro for disabling copy.
#define DISABLE_COPY(Class)                                                                                            \
    Class(const Class &) = delete;                                                                                     \
    Class &operator=(const Class &) = delete;

//
// UNUSED
//

//! Macro to supress warning about unused argument.
#define UNUSED(Var) (void)Var;

namespace details
{

//
// asConst
//

//! Adds const to non-const objects.
template<typename T>
constexpr typename std::add_const<T>::type &asConst(T &t) noexcept
{
    return t;
}

template<typename T>
void asConst(const T &&) = delete;

//
// isArgument
//

//! \return Is word an argument?
static inline bool isArgument(const String &word)
{
    return (word.find(SL("--")) == 0);
} // isArgument

//
// isFlag
//

//! \return Is word a flag?
static inline bool isFlag(const String &word)
{
    if (!isArgument(word)) {
        if (word.find(SL('-')) == 0) {
            return true;
        }
    }

    return false;
} // isFlag

//
// isCorrectFlag
//

//! \return Is flag correct?
static inline bool isCorrectFlag(const String &flag)
{
    if (flag.empty() || flag.length() > 1) {
        return false;
    }

    static const String availableSymbols(
        SL("0123456789"
           "abcdefghijklmnopqrstuvwxyz"
           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));

    return (availableSymbols.find(flag) != String::npos);
} // isCorrectFlag

//
// isCorrectName
//

//! \return Is name correct?
static inline bool isCorrectName(const String &name)
{
    if (name.empty()) {
        return false;
    }

    static const String availableSymbols(
        SL("0123456789"
           "abcdefghijklmnopqrstuvwxyz"
           "ABCDEFGHIJKLMNOPQRSTUVWXYZ-_"));

    for (const Char &c : asConst(name)) {
        if (availableSymbols.find(c) == String::npos) {
            return false;
        }
    }

    return true;
} // isCorrectName

//
// isMisspelledName
//

//! \return Is the given name a misspelling of correct name.
static inline bool isMisspelledName(const String &misspelled,
                                    const String &correct)
{
    if (!misspelled.empty() && !correct.empty()) {
        String ms = misspelled;
        String cs = correct;

        std::sort(ms.begin(), ms.end());
        std::sort(cs.begin(), cs.end());

        return (ms == cs);
    } else {
        return false;
    }
}

template<typename T = void>
struct DummyString {
    static const String c_string;
};

template<typename T>
const String DummyString<T>::c_string;

} /* namespace details */

} /* namespace Args */

#endif // ARGS__UTILS_HPP__INCLUDED

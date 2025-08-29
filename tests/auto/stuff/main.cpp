
/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest.h>

// Args include.
#include <args-parser/all.hpp>

using namespace Args;

#ifdef ARGS_WSTRING_BUILD
using CHAR = String::value_type;
#else
using CHAR = char;
#endif

TEST_CASE("TestFormatCorrectNames")
{
    StringList list;

    REQUIRE(details::formatCorrectNamesString(list).empty());

    list.push_back(SL("one"));
    list.push_back(SL("two"));

    REQUIRE(details::formatCorrectNamesString(list) == SL("one or two"));
}

TEST_CASE("TestMisspelledName")
{
    REQUIRE(details::isMisspelledName(SL("oen"), SL("one")));
    REQUIRE(!details::isMisspelledName(SL(""), SL("")));
}

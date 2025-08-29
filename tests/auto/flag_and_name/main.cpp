
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

TEST_CASE("DisallowedFlags")
{
    REQUIRE(details::isCorrectFlag(SL("-")) == false);
    REQUIRE(details::isCorrectFlag(SL(" ")) == false);
    REQUIRE(details::isCorrectFlag(SL("=")) == false);
    REQUIRE(details::isCorrectFlag(SL("")) == false);
}

TEST_CASE("AllowedFlags")
{
    REQUIRE(details::isCorrectFlag(SL("a")) == true);
    REQUIRE(details::isCorrectFlag(SL("1")) == true);
}

TEST_CASE("DisallowedNames")
{
    REQUIRE(details::isCorrectName(SL("time out")) == false);
    REQUIRE(details::isCorrectName(SL("")) == false);
}

TEST_CASE("AllowedNames")
{
    REQUIRE(details::isCorrectName(SL("timeout")) == true);
    REQUIRE(details::isCorrectName(SL("time-out")) == true);
    REQUIRE(details::isCorrectName(SL("time_out")) == true);
}

TEST_CASE("IsFlagOrArgument")
{
    REQUIRE(details::isFlag(SL("-a")) == true);
    REQUIRE(details::isFlag(SL("--timeout")) == false);
    REQUIRE(details::isArgument(SL("-a")) == false);
    REQUIRE(details::isArgument(SL("--timeout")) == true);
}

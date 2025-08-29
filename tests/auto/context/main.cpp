
/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest.h>

// Args include.
#include <args-parser/all.hpp>

// C++ include.
#include <utility>

using namespace Args;

TEST_CASE("TestContext")
{
    ContextInternal internal;

    internal.push_back(SL("-a"));
    internal.push_back(SL("--timeout"));
    internal.push_back(SL("100"));
    internal.push_back(SL("--port"));
    internal.push_back(SL("4545"));
    internal.push_back(SL("--host"));
    internal.push_back(SL("localhost"));

    Context ctx(std::move(internal));

    REQUIRE(*ctx.next() == SL("-a"));

    ctx.prepend(SL("value"));

    REQUIRE(*ctx.next() == SL("value"));

    ctx.putBack();

    REQUIRE(*ctx.next() == SL("value"));
    REQUIRE(*ctx.next() == SL("--timeout"));
    REQUIRE(*ctx.next() == SL("100"));
    REQUIRE(*ctx.next() == SL("--port"));
    REQUIRE(*ctx.next() == SL("4545"));
    REQUIRE(*ctx.next() == SL("--host"));

    REQUIRE(ctx.atEnd() == false);

    REQUIRE(*ctx.next() == SL("localhost"));

    REQUIRE(ctx.atEnd() == true);
}

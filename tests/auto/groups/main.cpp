
/*
    SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
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

TEST_CASE("TestOnlyOneAllIsOk")
{
    const int argc = 3;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    OnlyOneGroup g(SL("only_one"));
    cmd.addArg(g);

    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.parse();

    REQUIRE(timeout.isDefined() == true);
    REQUIRE(timeout.value() == SL("100"));

    REQUIRE(port.isDefined() == false);

    REQUIRE(host.isDefined() == false);
}

TEST_CASE("TestOnlyOneFailed")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100"), SL("-p"), SL("4545")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    OnlyOneGroup g(SL("only_one"));
    cmd.addArg(&g);

    g.addArg(&timeout);
    g.addArg(&port);
    g.addArg(&host);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("TestOnlyOneWithRequiredFailed")
{
    const int argc = 3;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true, true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    OnlyOneGroup g(SL("only_one"));
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("TestAllOfIsOk")
{
    const int argc = 7;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100"), SL("-p"), SL("4545"), SL("-h"), SL("localhost")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AllOfGroup g(SL("all_of"));
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    cmd.parse();

    REQUIRE(timeout.isDefined() == true);
    REQUIRE(timeout.value() == SL("100"));

    REQUIRE(port.isDefined() == true);
    REQUIRE(port.value() == SL("4545"));

    REQUIRE(host.isDefined() == true);
    REQUIRE(host.value() == SL("localhost"));
}

TEST_CASE("TestAllOfFailed")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100"), SL("-p"), SL("4545")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AllOfGroup g(SL("all_of"));
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("TestAllOfFailed2")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100"), SL("-h"), SL("localhost")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AllOfGroup g(SL("all_of"));
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("TestAllOfWithRequiredFailed")
{
    const int argc = 3;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true, true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    OnlyOneGroup g(SL("all_of"));
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("TestAtLeasOneIsOk")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100"), SL("-p"), SL("4545")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AtLeastOneGroup g(SL("at_least_one"));
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    cmd.parse();

    REQUIRE(timeout.isDefined() == true);
    REQUIRE(timeout.value() == SL("100"));

    REQUIRE(port.isDefined() == true);
    REQUIRE(port.value() == SL("4545"));

    REQUIRE(host.isDefined() == false);

    REQUIRE(g.isDefined() == true);
}

TEST_CASE("TestAtLeasOneFailed")
{
    const int argc = 1;
    const CHAR *argv[argc] = {SL("program.exe")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AtLeastOneGroup g(SL("at_least_one"));
    g.setRequired(true);
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("TestAtLeasOneWithRequiredFailed")
{
    const int argc = 3;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true, true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AtLeastOneGroup g(SL("at_least_one"));
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);

    cmd.addArg(g);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("GroupsIsOk")
{
    const int argc = 9;
    const CHAR *argv[argc] =
        {SL("program.exe"), SL("-t"), SL("100"), SL("-p"), SL("4545"), SL("-h"), SL("localhost"), SL("-s"), SL("db")};

    CmdLine cmd(argc, argv);

    Arg store(Char(SL('s')), SL("store"), true);
    Arg file(Char(SL('f')), SL("file"), true);

    OnlyOneGroup onlyOne(SL("only_one"));
    onlyOne.addArg(store);
    onlyOne.addArg(file);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AllOfGroup g(SL("all_of"));
    g.setRequired(true);
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);
    g.addArg(onlyOne);

    cmd.addArg(g);

    cmd.parse();

    REQUIRE(timeout.isDefined() == true);
    REQUIRE(timeout.value() == SL("100"));

    REQUIRE(port.isDefined() == true);
    REQUIRE(port.value() == SL("4545"));

    REQUIRE(host.isDefined() == true);
    REQUIRE(host.value() == SL("localhost"));

    REQUIRE(store.isDefined() == true);
    REQUIRE(store.value() == SL("db"));

    REQUIRE(file.isDefined() == false);

    REQUIRE(g.isDefined() == true);
}

TEST_CASE("GroupsFailed")
{
    const int argc = 11;
    const CHAR *argv[argc] = {SL("program.exe"),
                              SL("-t"),
                              SL("100"),
                              SL("-p"),
                              SL("4545"),
                              SL("-h"),
                              SL("localhost"),
                              SL("-s"),
                              SL("db"),
                              SL("-f"),
                              SL("out.txt")};

    CmdLine cmd(argc, argv);

    Arg store(Char(SL('s')), SL("store"), true);
    Arg file(Char(SL('f')), SL("file"), true);

    OnlyOneGroup onlyOne(SL("only_one"));
    onlyOne.addArg(store);
    onlyOne.addArg(file);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    AllOfGroup g(SL("all_of"));
    g.setRequired(true);
    g.addArg(timeout);
    g.addArg(port);
    g.addArg(host);
    g.addArg(onlyOne);

    cmd.addArg(g);

    REQUIRE_THROWS_AS(cmd.parse(), BaseException);
}

TEST_CASE("GroupsStuff")
{
    OnlyOneGroup one(SL("only_one"));

    const GroupIface &g = one;

    REQUIRE(!g.isWithValue());
    REQUIRE(g.flag().empty());
    REQUIRE(g.argumentName().empty());
    REQUIRE(g.valueSpecifier().empty());
    REQUIRE(g.description().empty());
    REQUIRE(g.longDescription().empty());
}

TEST_CASE("GroupValue")
{
    const int argc = 1;
    const CHAR *argv[argc] = {SL("program.exe")};

    CmdLine cmd(argc, argv);

    const auto name = SL("only_one");
    const auto not_exists = SL("not_exists");

    OnlyOneGroup onlyOne(name);
    cmd.addArg(onlyOne);

    REQUIRE(cmd.value(name).empty());
    REQUIRE(cmd.value(not_exists).empty());

    REQUIRE(cmd.values(name).empty());
    REQUIRE(cmd.values(not_exists).empty());

    REQUIRE(!cmd.isDefined(not_exists));
}

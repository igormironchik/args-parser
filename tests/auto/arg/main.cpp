
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

TEST_CASE("TestAllIsOk")
{
    const int argc = 6;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t=100"), SL("--port"), SL("4545"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg d(SL("default"), true);
    d.setDefaultValue(SL("default"));
    Arg host(SL('h'), String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&port);
    cmd.addArg(&host);
    cmd.addArg(d);

    cmd.parse();

    REQUIRE(timeout.isDefined() == true);
    REQUIRE(timeout.value() == SL("100"));

    REQUIRE(port.isDefined() == true);
    REQUIRE(port.value() == SL("4545"));

    REQUIRE(host.isDefined() == true);
    REQUIRE(host.value() == SL("any"));

    REQUIRE(d.isDefined() == false);
    REQUIRE(d.value() == SL("default"));
}

TEST_CASE("TestUndefinedArg")
{
    const int argc = 7;
    const CHAR *argv[argc] =
        {SL("program.exe"), SL("-t"), SL("100"), SL("--port"), SL("4545"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg host(String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&host);

    try {
        cmd.parse();
    } catch (const BaseException &x) {
        REQUIRE(x.desc() == String(SL("Unknown argument \"--port\".")));

        return;
    }

    REQUIRE(false);
}

TEST_CASE("TestUndefinedRequiredArg")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("100"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true, true);
    Arg host(SL('h'), String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&port);
    cmd.addArg(&host);

    try {
        cmd.parse();
    } catch (const BaseException &x) {
        REQUIRE(x.desc() == String(SL("Undefined required argument \"--port\".")));

        return;
    }

    REQUIRE(false);
}

TEST_CASE("TestPositionalButNotFlags")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-ta"), SL("100"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv, CmdLine::HandlePositionalArguments);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&port);
    cmd.addArg(&host);

    cmd.parse();

    REQUIRE(timeout.isDefined() == false);
    REQUIRE(port.isDefined() == false);
    REQUIRE(host.isDefined() == false);

    REQUIRE(cmd.positional().size() == 4);
    REQUIRE(cmd.positional().front() == SL("-ta"));
    REQUIRE(cmd.positional().back() == SL("any"));
}

TEST_CASE("TestPositionalButNotFlags2")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-aa"), SL("100"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv, CmdLine::HandlePositionalArguments);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&port);
    cmd.addArg(&host);

    cmd.parse();

    REQUIRE(timeout.isDefined() == false);
    REQUIRE(port.isDefined() == false);
    REQUIRE(host.isDefined() == false);

    REQUIRE(cmd.positional().size() == 4);
    REQUIRE(cmd.positional().front() == SL("-aa"));
    REQUIRE(cmd.positional().back() == SL("any"));
}

TEST_CASE("TestPositionalButNotArg")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("--aa"), SL("100"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv, CmdLine::HandlePositionalArguments);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&port);
    cmd.addArg(&host);

    cmd.parse();

    REQUIRE(timeout.isDefined() == false);
    REQUIRE(port.isDefined() == false);
    REQUIRE(host.isDefined() == false);

    REQUIRE(cmd.positional().size() == 4);
    REQUIRE(cmd.positional().front() == SL("--aa"));
    REQUIRE(cmd.positional().back() == SL("any"));
}

TEST_CASE("TestPositionalButNotArg2")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("--"), SL("100"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv, CmdLine::HandlePositionalArguments);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&port);
    cmd.addArg(&host);

    cmd.parse();

    REQUIRE(timeout.isDefined() == false);
    REQUIRE(port.isDefined() == false);
    REQUIRE(host.isDefined() == false);

    REQUIRE(cmd.positional().size() == 3);
    REQUIRE(cmd.positional().front() == SL("100"));
    REQUIRE(cmd.positional().back() == SL("any"));
}

TEST_CASE("TestPositionalButNotArg3")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("--arg=value"), SL("100"), SL("--host"), SL("any")};

    CmdLine cmd(argc, argv, CmdLine::HandlePositionalArguments);

    Arg timeout(SL('t'), String(SL("timeout")), true);
    Arg port(SL('p'), String(SL("port")), true);
    Arg host(SL('h'), String(SL("host")), true);

    cmd.addArg(&timeout);
    cmd.addArg(&port);
    cmd.addArg(&host);

    cmd.parse();

    REQUIRE(timeout.isDefined() == false);
    REQUIRE(port.isDefined() == false);
    REQUIRE(host.isDefined() == false);

    REQUIRE(cmd.positional().size() == 4);
    REQUIRE(cmd.positional().front() == SL("--arg=value"));
    REQUIRE(cmd.positional().back() == SL("any"));
}

TEST_CASE("TestStuff")
{
    Arg a(SL('t'), String(SL("timeout")), true);

    REQUIRE(a.value().empty());
    REQUIRE(a.defaultValue().empty());

    a.setDefaultValue(SL("1"));

    REQUIRE(a.value() == SL("1"));
    REQUIRE(a.defaultValue() == SL("1"));

    a.setValue(SL("2"));

    REQUIRE(a.value() == SL("2"));
    REQUIRE(a.defaultValue() == SL("1"));

    const int argc = 1;
    const CHAR *argv[argc] = {SL("program.exe")};

    CmdLine cmd(argc, argv);
    cmd.addArg(a);

    const CmdLine &c_cmd = cmd;

    REQUIRE(!c_cmd.findArgument(SL("no")));
}

TEST_CASE("TestUndefinedValue")
{
    const int argc = 3;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-t"), SL("-t")};

    CmdLine cmd(argc, argv);

    Arg timeout(SL('t'), String(SL("timeout")), true);

    cmd.addArg(&timeout);

    try {
        cmd.parse();
    } catch (const BaseException &x) {
        REQUIRE(x.desc() == String(SL("Argument \"--timeout\" requires value that wasn't presented.")));

        return;
    }

    REQUIRE(false);
}

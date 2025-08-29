
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

TEST_CASE("TestAllIsOk1")
{
    const int argc = 10;
    const CHAR *argv[argc] = {SL("program.exe"),
                              SL("-m"),
                              SL("100"),
                              SL("-m"),
                              SL("200"),
                              SL("-m"),
                              SL("300"),
                              SL("-m"),
                              SL("400"),
                              SL("500")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), true);

    cmd.addArg(&multi);

    cmd.parse();

    REQUIRE(multi.isDefined() == true);

    const StringList values = multi.values();

    REQUIRE(multi.count() == 5);
    REQUIRE(values.size() == 5);

    REQUIRE(multi.value() == SL("100"));

    auto it = values.begin();

    REQUIRE(*it == SL("100"));
    ++it;

    REQUIRE(*it == SL("200"));
    ++it;

    REQUIRE(*it == SL("300"));
    ++it;

    REQUIRE(*it == SL("400"));
    ++it;

    REQUIRE(*it == SL("500"));
}

TEST_CASE("TestAllIsOk2")
{
    const int argc = 11;
    const CHAR *argv[argc] = {SL("program.exe"),
                              SL("-m"),
                              SL("100"),
                              SL("-m"),
                              SL("200"),
                              SL("-m"),
                              SL("300"),
                              SL("-m"),
                              SL("400"),
                              SL("500"),
                              SL("-t")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), true);
    Arg timeout(SL('t'), String(SL("timeout")));

    cmd.addArg(&multi);
    cmd.addArg(&timeout);

    cmd.parse();

    REQUIRE(multi.isDefined() == true);

    const StringList values = multi.values();

    REQUIRE(multi.count() == 5);
    REQUIRE(values.size() == 5);

    REQUIRE(multi.value() == SL("100"));

    auto it = values.begin();

    REQUIRE(*it == SL("100"));
    ++it;

    REQUIRE(*it == SL("200"));
    ++it;

    REQUIRE(*it == SL("300"));
    ++it;

    REQUIRE(*it == SL("400"));
    ++it;

    REQUIRE(*it == SL("500"));

    REQUIRE(timeout.isDefined() == true);
    REQUIRE(timeout.value().empty());
}

TEST_CASE("TestAllIsOk3")
{
    const int argc = 5;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-m"), SL("-m"), SL("-m"), SL("-m")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), false, true);

    cmd.addArg(multi);

    cmd.parse();

    REQUIRE(multi.isDefined() == true);
    REQUIRE(multi.count() == 4);
}

TEST_CASE("TestAllIsOk4")
{
    const int argc = 1;
    const CHAR *argv[argc] = {SL("program.exe")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), true, false);
    multi.setDefaultValue(SL("default"));

    cmd.addArg(multi);

    cmd.parse();

    REQUIRE(multi.isDefined() == false);
    REQUIRE(multi.value() == SL("default"));
    REQUIRE(multi.values().size() == 1);
    REQUIRE(multi.values().front() == SL("default"));
}

TEST_CASE("TestAllIsOk5")
{
    const int argc = 1;
    const CHAR *argv[argc] = {SL("program.exe")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), true, false);

    cmd.addArg(multi);

    cmd.parse();

    REQUIRE(multi.isDefined() == false);
    REQUIRE(multi.value().empty());
    REQUIRE(multi.values().size() == 0);
    REQUIRE(multi.count() == 0);
    REQUIRE(multi.defaultValue().empty());
    REQUIRE(multi.defaultValues().empty());
}

TEST_CASE("TestAllIsOk6")
{
    const int argc = 7;
    const CHAR *argv[argc] = {SL("program.exe"), SL("-m"), SL("1"), SL("-t"), SL("--not"), SL("-m"), SL("2")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), true, false);

    cmd.addArg(multi);

    cmd.parse();

    const auto v = multi.values();

    REQUIRE(multi.isDefined());
    REQUIRE(!multi.value().empty());
    REQUIRE(v.size() == 4);
    REQUIRE(multi.count() == 4);
    REQUIRE(multi.defaultValue().empty());
    REQUIRE(multi.defaultValues().empty());
    REQUIRE(v.at(0) == SL("1"));
    REQUIRE(v.at(1) == SL("-t"));
    REQUIRE(v.at(2) == SL("--not"));
    REQUIRE(v.at(3) == SL("2"));
}

TEST_CASE("NotDefinedValue")
{
    const int argc = 8;
    const CHAR *argv[argc] =
        {SL("program.exe"), SL("-m"), SL("100"), SL("-m"), SL("200"), SL("-m"), SL("300"), SL("-m")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), true);

    cmd.addArg(&multi);

    try {
        cmd.parse();
    } catch (const BaseException &x) {
        REQUIRE(x.desc()
                == String(SL("Argument \"--multi"
                             "\" requires value that wasn't presented.")));

        return;
    }

    REQUIRE(false);
}

TEST_CASE("NotDefinedValue2")
{
    const int argc = 9;
    const CHAR *argv[argc] =
        {SL("program.exe"), SL("-m"), SL("100"), SL("-m"), SL("200"), SL("-m"), SL("300"), SL("-m"), SL("-t")};

    CmdLine cmd(argc, argv);

    MultiArg multi(SL('m'), String(SL("multi")), true);
    Arg timeout(SL('t'), String(SL("timeout")));

    cmd.addArg(&multi);
    cmd.addArg(&timeout);

    try {
        cmd.parse();
    } catch (const BaseException &x) {
        REQUIRE(x.desc()
                == String(SL("Argument \"--multi"
                             "\" requires value that wasn't presented.")));

        return;
    }

    REQUIRE(false);
}

TEST_CASE("TestStuff")
{
    MultiArg multi(SL('m'), String(SL("multi")), true);

    Arg &arg = multi;

    REQUIRE(arg.defaultValue().empty());
    REQUIRE(multi.defaultValues().empty());
    REQUIRE(arg.value().empty());

    arg.setDefaultValue(SL("1"));

    REQUIRE(arg.defaultValue() == SL("1"));
    REQUIRE(multi.defaultValues().size() == 1);
    REQUIRE(multi.defaultValues().front() == SL("1"));
    REQUIRE(arg.value() == SL("1"));
    REQUIRE(multi.values().size() == 1);
    REQUIRE(multi.values().front() == SL("1"));

    arg.setValue(SL("2"));

    REQUIRE(arg.defaultValue() == SL("1"));
    REQUIRE(multi.defaultValues().size() == 1);
    REQUIRE(multi.defaultValues().front() == SL("1"));
    REQUIRE(arg.value() == SL("2"));
    REQUIRE(multi.values().size() == 1);
    REQUIRE(multi.values().front() == SL("2"));
}

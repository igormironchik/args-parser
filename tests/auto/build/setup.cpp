/*
    SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include "setup.hpp"

void SetupCMD(Args::CmdLine &cmd)
{
    cmd.addArgWithNameOnly("foo", true, true, "bar");

    cmd.parse();
}

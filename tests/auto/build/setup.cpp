/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include "setup.hpp"

void SetupCMD(Args::CmdLine &cmd)
{
    cmd.addArgWithNameOnly("foo", true, true, "bar");

    cmd.parse();
}

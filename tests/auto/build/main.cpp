/*
    SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include <args-parser/all.hpp>

#include "setup.hpp"

int main(int argc,
         char *argv[])
{
    Args::CmdLine cmd{argc, argv};

    SetupCMD(cmd);

    return 0;
}

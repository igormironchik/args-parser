
/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// Args include.
#include <args-parser/all.hpp>

// C++ include.
#include <iostream>

#ifdef ARGS_WSTRING_BUILD
int wmain(int argc,
          wchar_t **argv)
#else
int main(int argc,
         char **argv)
#endif
{
    try {
        /*
            We create Args::CmdLine instance for parsing
            command line arguments.
        */
        Args::CmdLine cmd(argc, argv);

        /*
            And create our arguments.
        */

        /*
            This is argument with flag "-h" and name "--host".
            He is with value and required.
        */
        Args::Arg host(SL('s'),
                       SL("host"),
                       // Argument is with value.
                       true,
                       // Argument is required.
                       true);
        // Set description of the argument.
        host
            .setDescription(SL("Host.\nCan be \"localhost\", \"any\" or regular IP."))
            .setLongDescription(        // We can specify long description too.
                SL("Host. This argument told to the application "
                "where to open socket for communication."))
            .setDefaultValue("localhost"); // We can set a default value.

        Args::Arg port(SL('p'), SL("port"), true, true);
        port.setDescription(SL("Port number to create socket."));

        /*
            This argument have name "--timeout" only.
            He is with value but optional.
        */
        Args::Arg timeout(SL("timeout"), true);
        //! This argument want to specify value specifier in the help. Let's do it.
        timeout
            .setValueSpecifier(SL("ms"))
            .setDescription(
                SL("Timeout before new messages will be sent "
                "in milliseconds."));

        /*
            We create help now.
        */
        Args::Help help;
        
        help.setExecutable(argv[0])     // Set executable name to the help printer.
            .setAppDescription(         //! And set description of the application.
            SL("This application just show "
               "the power of Args."));

        /*
            Now add our arguments to the command line, and parse when we're done.
        */
        cmd.addArg(host)
            .addArg(port)
            .addArg(timeout)
            .addArg(help)
            .parse();

    } catch (const Args::HelpHasBeenPrintedException &) {
        return 0;
    } catch (const Args::BaseException &x) {
        Args::outStream() << x.desc() << std::endl;

        return 1;
    }

    return 0;
}

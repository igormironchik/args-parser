[![Build Status](https://travis-ci.org/igormironchik/args-parser.svg?branch=master)](https://travis-ci.org/igormironchik/args-parser)[![Build status](https://ci.appveyor.com/api/projects/status/p9jg1bgicqvoryh1/branch/master?svg=true)](https://ci.appveyor.com/project/igormironchik/args-parser/branch/master)[![codecov](https://codecov.io/gh/igormironchik/args-parser/branch/master/graph/badge.svg)](https://codecov.io/gh/igormironchik/args-parser)[![Coverity Scan](https://scan.coverity.com/projects/14278/badge.svg)](https://scan.coverity.com/projects/igormironchik-args-parser)[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

This is Args.

Args is a small C++ header-only library for parsing command line arguments.

# Syntax

 * Argument starts with two dashes. For example `--argument`.
 * Flag starts with one dash. For example `-b`.
 * If argument has value than value can be specified after space or after equal sign.
For example `--argument value` and `--argument=value`.
 * Flags without values can be combined into one block. For example
`-abc` defines three flags `-a`, `-b` and `-c`.
 * The last flag in flag's block can be with value. Then it's possible to type the next
`-abc=value`. And here flag `-c` will have value `value`.
 * `MultiArg` class provides ability to define more than one value for argument. This type
of arguments can be specified more than once in the command line. And the resulted value
of the argument will be `StringList`.
 * `Command` class can be used to define command in command line interface.
Command is the argument without dash/dashes at the beginning, `add` for example.
 * `Command` can has children arguments or even subcommands. Subcommand can be
added using `ArgAsCommand` class.
 * In Args groups can be used to group arguments into groups to check their
definitions after parsing, so if constraint of group will be violated
exception will be thrown.
 * Args provides `Help` argument that provides help printing. `Help`
uses `-h, --help` arguments. `Help` can receive value with name
of argument or command to print help about. If `Help` will receive name
of the command as value then can be set second value with name of subcommand
or child argument to receive help about child argument.
 * If Args don't know about argument in command line interface it provides
information about possible arguments if some misspelling was in command
line interface. If Args can't assume anything about entered argument it
will just say about unknown argument through the exception and parsing will
fail.

# Different types of strings.

Since version 4.0.0 Args can be built with different strings - std::string,
std::wstring and QString.

 * To build Args with std::wstring support define ARGS_WSTRING_BUILD
 * To build Args with QString support define ARGS_QSTRING_BUILD
 * If nothing was defined then Args will be build with std::string.

# Different types of list of strings.

Args extensively uses list of string in internal structures and to return
values of arguments outside. In the code this is `StringList`
type defined in `Args/types.hpp`. By default underlying type is
`std::vector` or `QVector` when building with Qt that can be changed
to `std::list`, `QLinkedList` or `std::deque`, `QList`.

 * Define `ARGS_LIST` to build Args with `std::list`, `QLinkedList`
as `StringList`
 * Define `ARGS_DEQUE` to build Args with `std::deque`, `QList`
as `StringList`

# Q/A

Why not to add description, long description, etc into constructors of
arguments, so it will be possible to initialize argument in one line?

 * This is impossible because constructors will be ambiguous but you can use
auxiliary API that allows to define arguments in one line of code.

How can I add Args to my project?

 * The simplest way is just copy Args directory with headers to any location in
your project. With CMake you can clone entire Args project somewhere in your
project and just do `add_subdirectory()`, if you will do so you have to
add include directory path to your project with
`include_directories( ${Args_INCLUDE_DIRECTORIES} )`. With QMake you can
use `Args/Args.pri`.

 * You can clone/download Args, build and install Args with CMake. In this
case it will be possible to use `find_package( Args )` in CMakeLists.txt of
your project, and sure you can use `${Args_INCLUDE_DIRECTORIES}` in your
CMake scripts.

What does `build.rb` file for?

 * `build.rb` is Ruby program for building Args, this is project file for
Mxx_ru build system. In most cases this file is not necessary and can be simply
ignored. Long time ago I used Mxx_ru as build system in my projects and this
file is just because of historical reasons. In the subdirectories you can find
different Ruby scripts, like `prj.rb`, these files are project files for
corresponding subprojects (examples/tests) for Mxx_ru build system.

What does `runtests.rb` file for?

 * `runtests.rb` is simple Ruby script to launch all tests. This file is
copying to build directory by CMake and QMake and can be used to launch tests.
And sure with CMake you can use ctest executable to launch tests too.
          
# Example

First of all you must know that practically all classes of the Args throws exceptions on errors
and there is one specific exceptions that inform you about that that help was shown. This specific
exception (HelpHasBeenPrintedException) is needed for processing program's logic that usually stops
execution at this point.

Since version 5.0.0 Args provides two API: the old one and auxiliary API
that allows to define arguments in one line of code. Let's look.

```cpp
// Args include.
#include <Args/all.hpp>

using namespace Args;

int main( int argc, char ** argv )
{
  try {
    CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

    cmd.addCommand( "add", ValueOptions::NoValue,
          "Add file." )
        .addSubCommand( "file", true, ValueOptions::ManyValues, "File name.", "", "", "fn" )
      .end()
      .addCommand( "delete", ValueOptions::NoValue,
          "Delete file." )
        .addSubCommand( "file", true, ValueOptions::ManyValues, "File name.", "", "", "fn" )
      .end()
      .addHelp( true, argv[ 0 ],
        "This application just show power of the Args help." );

    cmd.parse();

    if( cmd.isDefined( "file" ) )
      for( const auto & fn : cmd.values( "file" ) )
        outStream() << fn << "\n";
  }
  catch( const HelpHasBeenPrintedException & )
  {
    return 0;
  }
  catch( const BaseException & x )
  {
    outStream() << x.desc() << "\n";

    return 1;
  }

  return 0;
}
```

And with the old syntax.


```cpp
// Args include.
#include <Args/all.hpp>

// C++ include.
#include <iostream>


int main( int argc, char ** argv )
{
  try {
    /*
      We create Args::CmdLine instance for parsing
      command line arguments.
    */
    Args::CmdLine cmd( argc, argv );

    /*
      And create our arguments.
    */

    /*
      This is argument with flag "-o" and name "--host".
      He is with value and required.
    */
    Args::Arg host( 'o', "host",
      // Argument is with value.
      true,
      // Argument is required.
      true );
    // Set description of the argument.
    host.setDescription( "Host. Can be \"localhost\", \"any\" or regular IP." );
    // We can specify long description too.
    host.setLongDescription( "Host. This argument told to the application "
      "where to open socket for communication." );

    Args::Arg port( 'p', "port", true, true );
    port.setDescription( "Port number to create socket." );

    /*
      This argument have name "--timeout" only.
      He is with value but optional.
    */
    Args::Arg timeout( "timeout", true );
    // This argument want to specify value specifier in the help. Let's do it.
    timeout.setValueSpecifier( "ms" );
    timeout.setDescription( "Timeout before new messages will be sent "
      "in milliseconds." );

    /*
      We create help now.
    */
    Args::Help help;
    // Set executable name to the help printer.
    help.setExecutable( argv[ 0 ] );
    // And set description of the application.
    help.setAppDescription( "This application just show "
      "the power of Args." );

    /*
      Now add our argument to the command line.
    */
    cmd.addArg( host );
    cmd.addArg( port );
    cmd.addArg( timeout );
    cmd.addArg( help );

    /*
      Now parse our arguments.
    */
    cmd.parse();

    if( timeout.isDefined() )
      auto timeoutValue = timeout.value();
  }
  catch( const Args::HelpHasBeenPrintedException & )
  {
    return 0;
  }
  catch( const Args::BaseException & x )
  {
    std::cout << x.what() << "\n";

    return 1;
  }

  return 0;
}
```

Help output for the example with the old syntax.

```
This application just show the power of Args.

USAGE: sample.help.exe -s, --host <arg> -p, --port <arg> [ -h, --help <arg> ]
       [ --timeout <ms> ]

REQUIRED:
 -s, --host <arg>   Host. Can be "localhost", "any" or regular IP.

 -p, --port <arg>   Port number to create socket.

OPTIONAL:
 -h, --help <arg>   Print this help.

     --timeout <ms> Timeout before new messages will be sent in milliseconds.
```

That's it. Use it and enjoy it. Good luck.

[![Build](https://github.com/igormironchik/args-parser/workflows/build/badge.svg)](https://github.com/igormironchik/args-parser/actions)[![codecov](https://codecov.io/gh/igormironchik/args-parser/branch/master/graph/badge.svg)](https://codecov.io/gh/igormironchik/args-parser)[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

This is args-parser.

args-parser is a small C++ header-only library for parsing command line arguments.

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
 * `Command` can has children arguments or even subcommands.
 * In args-parser groups can be used to group arguments into groups to check their
definitions after parsing, so if constraint of group will be violated
exception will be thrown.
 * args-parser provides `Help` argument that provides help printing. `Help`
uses `-h, --help` arguments. `Help` can receive value with name
of argument or command to print help about. If `Help` will receive name
of the command as value then can be set second value with name of subcommand
or child argument to receive help about child argument.
 * If args-parser don't know about argument in command line interface it provides
information about possible arguments if some misspelling was in command
line interface. If args-parser can't assume anything about entered argument it
will just say about unknown argument through the exception and parsing will
fail.

# Different types of strings.

Since version 4.0.0 args-parser can be built with different strings - std::string,
std::wstring and QString.

 * To build args-parser with std::wstring support define ARGS_WSTRING_BUILD
 * To build args-parser with QString support define ARGS_QSTRING_BUILD
 * If nothing was defined then args-parser will be build with std::string.

# Different types of list of strings.

args-parser extensively uses list of string in internal structures and to return
values of arguments outside. In the code this is `StringList`
type defined in `args-parser/types.hpp`. By default underlying type is
`std::vector` or `QVector` when building with Qt that can be changed
to `std::list`, `QLinkedList` or `std::deque`, `QList`.

 * Define `ARGS_LIST` to build args-parser with `std::list`, `QLinkedList`
as `StringList`
 * Define `ARGS_DEQUE` to build args-parser with `std::deque`, `QList`
as `StringList`

# Q/A

Why not to add description, long description, etc into constructors of
arguments, so it will be possible to initialize argument in one line?

 * This is impossible because constructors will be ambiguous but you can use
auxiliary API that allows to define arguments in one line of code.

How can I add args-parser to my project?

 * The simplest way is just copy args-parser directory with headers to any location in
your project. With CMake you can clone entire `args-parser` project somewhere in your
project and just do `add_subdirectory()`, if you will do so you have to
add include directory path to your project with
`include_directories( ${args-parser_INCLUDE_DIRECTORIES} )`.

 * You can clone/download args-parser, build and install it with CMake. In this
case it will be possible to use `find_package( args-parser )` in CMakeLists.txt of
your project.

Why should I use this library?

 * There are tons of libraries for parsing command line arguments in the
Internet. But args-parser provides possibility to define commands, for example

    ```
    git add -A
    git commit
    git push
    ```

    Help output is very user-friendly. I tried to do help very useful for user.

    If you need only simple arguments in style `--do-it <value>` then possibly
    you will find another library more useful for you, but who knows...

    This library is highly efficient in performance. On Intel (R) Core (TM)
    i5-7200U CPU @ 2.50GHz args-parser occupies 0.5 milliseconds for 1000
    arguments with value. Really, guys, do you look at performance or something
    that library can do? This library is very flexible with powerfull help
    printing. What do you need more?

# Changelog

 * 6.0.0.0

    In this version was removed `ArgAsCommand` class, and was added fully-featured
    support for sub-commands, that can be created with `Command` class, i.e.
    `Command` can has `Command` as child. So it's possible to create such cli
    as `git submodule update --init --recursive`.
    
 * 6.1.0.0
 
     Added possibility to add `Command` into groups.

# Example

First of all you must know that practically all classes of the args-parser throws exceptions on errors
and there is one specific exceptions that inform you about that that help was shown. This specific
exception (HelpHasBeenPrintedException) is needed for processing program's logic that usually stops
execution at this point.

Since version 5.0.0 args-parser provides two API: the old one and auxiliary API
that allows to define arguments in one line of code. Let's look.

```cpp
// args-parser include.
#include <args-parser/all.hpp>

using namespace Args;

int main( int argc, char ** argv )
{
  try {
    CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

    cmd.addCommand( "add", ValueOptions::NoValue, true, "Add file." )
        .addCommand( "file", ValueOptions::ManyValues, false, "File name.", "", "", "fn" )
        .end()
      .end()
      .addCommand( "delete", ValueOptions::NoValue, true, "Delete file." )
        .addCommand( "file", ValueOptions::ManyValues, false, "File name.", "", "", "fn" )
        .end()
      .end()
      .addHelp( true, argv[ 0 ],
        "This application just show power of the args-parser help." );

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
// args-parser include.
#include <args-parser/all.hpp>

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
      "the power of args-parser." );

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
This application just show the power of args-parser.

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

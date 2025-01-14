<!--
	SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
-->

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

This is `args-parser`.

`args-parser` is a small C++ header-only library for parsing command line arguments.

* [Syntax](#syntax)
* [Different types of strings.](#different-types-of-strings)
* [Different types of list of strings.](#different-types-of-list-of-strings)
* [Q/A](#qa)
  * [Why not to add description, long description, etc into constructors of arguments, so it will be possible to initialize argument in one line?](#why-not-to-add-description-long-description-etc-into-constructors-of-argumentsso-it-will-be-possible-to-initialize-argument-in-one-line)
  * [How can I add `args-parser` to my project?](#how-can-i-add-args-parser-to-my-project)
  * [Why should I use this library?](#why-should-i-use-this-library)
  * [How to print help programmatically?](#how-to-print-help-programmatically)
  * [Why doesn't `args-parser` provide bindings of variables to arguments?](#why-doesnt-args-parser-provide-bindings-of-variables-to-arguments)
  * [Why don't you provide comparisons with other CLI libraries?](#why-dont-you-provide-comparisons-with-other-cli-libraries)
* [Changelog](#changelog)
* [Example](#example)
  * [One-line syntax with commands](#one-line-syntax-with-commands)
  * [One-line syntax with arguments](#one-line-syntax-with-arguments)
  * [And with the old syntax](#and-with-the-old-syntax)

# Syntax

 * An argument starts with two dashes. For example `--argument`.
 * A flag starts with one dash. For example `-b`.
 * If an argument has a value, then the value can be specified after a space or after an equal sign.
For example `--argument value` and `--argument=value`.
 * Flags without values can be combined into one block. For example
`-abc` defines three flags `-a`, `-b` and `-c`.
 * The last flag in a flags' block can have a value. For example `-abc=value`. And here, flag `-c` will have the value `value`.
 * `MultiArg` class provides the ability to define more than one value for an argument. These types
of arguments can be specified more than once in the command line. And the resulted value
of the argument will be `StringList`.
 * `Command` class can be used to define command in command line interface.
Command is the argument without dash/dashes at the beginning, `add` for example.
 * `Command` can has children arguments or even subcommands.
 * In `args-parser` groups can be used to group arguments into groups to check their
definitions after parsing, so if constraint of group will be violated
exception will be thrown.
 * `args-parser` provides `Help` argument that provides help printing. `Help`
uses `-h, --help` arguments. `Help` can receive value with name
of argument or command to print help about. If `Help` receives the name
of a command as the value, then the name of a subcommand or a child argument
can also be provided as the next value, to get help about the subcommand/child argument.
 * If `args-parser` don't know about argument in command line interface it provides
information about possible arguments if some misspelling was in command
line interface. If `args-parser` can't assume anything about entered argument it
will just say about unknown argument through the exception and parsing will
fail.
 * With `CmdLine::HandlePositionalArguments` option provided to `CmdLine`
constructor, `args-parser` will handle positional arguments, these are such arguments
that can't be parsed with provided arguments, flags, commands and stay at the end
of the command line. Be careful with combining `MultiArg` and positional arguments,
because `MultiArg` can eat almost everything that is not a valid argument.

# Different types of strings.

Since version `4.0.0` `args-parser` can be built with different strings - `std::string`,
`std::wstring` and `QString`.

 * To build `args-parser` with `std::wstring` support define `ARGS_WSTRING_BUILD`
 * To build `args-parser` with `QString` support define `ARGS_QSTRING_BUILD`
 * If nothing was defined then `args-parser` will be build with `std::string`.

# Different types of list of strings.

`args-parser` extensively uses list of string in internal structures and to return
values of arguments outside. In the code this is `StringList`
type defined in `args-parser/types.hpp`. By default underlying type is
`std::vector` or `QVector` when building with Qt that can be changed
to `std::list`, `QLinkedList` or `std::deque`, `QList`.

 * Define `ARGS_LIST` to build `args-parser` with `std::list`, `QLinkedList`
as `StringList`
 * Define `ARGS_DEQUE` to build `args-parser` with `std::deque`, `QList`
as `StringList`

# Q/A

Why not to add description, long description, etc into constructors of arguments,
so it will be possible to initialize argument in one line?
---

 * This is impossible because constructors will be ambiguous but you can use
auxiliary API that allows to define arguments in one line of code.

How can I add `args-parser` to my project?
---

 * The simplest way is just copy `args-parser` directory with headers to any location in
your project. With CMake you can clone entire `args-parser` project somewhere in your
project and just do `add_subdirectory()`, if you will do so you have to
add include directory path to your project with
`include_directories( ${args-parser_INCLUDE_DIRECTORIES} )`.

 * You can clone/download `args-parser`, build and install it with CMake. In this
case it will be possible to use `find_package( args-parser )` in `CMakeLists.txt` of
your project.

 * You can use `Conan` package manager.

Why should I use this library?
---

 * There are tons of libraries for parsing command line arguments on the
Internet. But `args-parser` also provides the possibility to define commands, like

    ```
    git add -A
    git commit
    git push
    ```

    Help output is very user-friendly.

    If you need only simple arguments in style `--do-it <value>` then possibly
    you will find another library more useful for you, but who knows...

How to print help programmatically?
---

 * For those, who use old style syntax the answer should be on the plate, as developer
can look at the code of `Help` class, that implements help argument. But for those, who
prefer one-line syntax the answer can be not so evident, but it's so too. It doesn't matter
what syntax you use, new (one-line) syntax is just a wrapper for old one. I.e. line:

   ```cpp
   cmd.addHelp( true, argv[ 0 ],
     "This application just show power of the Args help." );
   ```

   Is just a wrapper around of `Help` class. For printing help `args-parser` uses
   `HelpPrinter` class. So developer can use it for printing help in some rare cases,
   as:

   ```cpp
   HelpPrinter printer;

   printer.setExecutable( argv[ 0 ] );
   printer.setAppDescription( "This application just show power of the Args help." );
   printer.setLineLength( length );
   printer.setCmdLine( cmd );

   printer.print( outStream() );
   ```

Why doesn't `args-parser` provide bindings of variables to arguments?
---

 * This is a question of why doesn't `args-parser` provide validators? It's the same.
I decided that this aspect is very application specific. There is no need for such library
to do any conversions of arguments' values, to bind them to some variables. This will
do API very complex only. I know what you will say: this is very nice feature, it helps...
Really? How often and how much it helped you? Arguments parser should handle the string
that user provided to the application, it should separate arguments, commands, values,
store it in internal state of parser, and allow to developer just write some `if`
operators to choose branch of the program logic. What will give you, as to developer,
if values will be bind to concrete variables? Will not you write the same code with `if`
operators? So why I should do the library more complex?

Why don't you provide comparisons with other CLI libraries?
---

 * I found only one library at GitHub that can compete with `args-parser`, and this is
[CLI11](https://github.com/CLIUtils/CLI11). And here is the question of the taste more.
But `CLI11` can handle commands as usual arguments, it's doesn't matter how much times
they present in command line, whereas `args-parser` handles commands as commands. Theirs
approach possibly more flexible, but when I designed `args-parser` I thought on commands
as on some action to do in application's logic, whereas arguments are for data. I can do
the same, but is it needed?

   `CLI11` has possibility to set formatter of the help, `args-parser` allow to set
   custom `HelpPrinterIface` on the `Help` argument. But who and when will do it?
   And I believe that help in `args-parser` is a little better than in `CLI11`.

   `CLI11` works more with callbacks, whereas `args-parser` uses hierarchy of
   classes with polymorphism. In `args-parser` you can inherit from any argument's
   class, override methods you need and receive something very application specific.
   And again, this is more question of taste.
   
   Uh, oh, I found one more interesting arguments parsing library, This is
   [Taywee/Args](https://github.com/Taywee/args). Guys, this is a question of taste.
   And, as said in `CLI11` documentation about `Taywee/Args`, `args-parser` as
   `CLI11` less verbose. `Taywee/Args` has benchmark to compare performance with
   `TCLAP` and `boost::program_options.`, I want to say that `args-parser` 2 times
   faster than `Taywee/Args`, dry numbers says that `Tyawee/Args` runs 0.635314
   second, whereas `args-parser` runs 0.346813 second.
   
   What I want to say about minuses of `args-parser`is that I don't support other
   delimiter characters than `-` for flags and `--` for arguments.

# Changelog

| Version | Changes |
| --- | --- |
| **6.3.4** | Fixed issue with `MSVC` when globally defined `::Command` class was detected as friend of `ArgIface` instead of `Args::Command` |
| **6.3.3** | Minor fix for compilation with `-Werror=shadow` |
| **6.3.2** | Fixed multiple definitions when included from different compile units. |
| **6.3.1** | Improved performance. Added possibility to set positional arguments string for the help. Added benchmark. |
| **6.3.0** | Added possibility to handle positional arguments. |
| **6.2.0.1** | Ready for use with Qt6. |
| **6.2.0.0** | Allowed to inherit from some classes. Added `addArg()` methods into API. |
| **6.1.1.1** | Added possibility to set up custom help printer in help argument. |
| **6.1.1.0** | Improved API with new syntax, now it's impossible to mess with end() methods. Fixed issue with printing help of global argument under command. |
| **6.1.0.0** | Added possibility to add `Command` into groups. |
| **6.0.0.0** | In this version was removed `ArgAsCommand` class, and was added fully-featured support for sub-commands, that can be created with `Command` class, i.e. `Command` can has `Command` as child. So it's possible to create such cli as `git submodule update --init --recursive`. |

# Example

First of all you must know that practically all classes of the `args-parser` throws exceptions on errors
and there is one specific exceptions that inform you about that that help was shown. This specific
exception (HelpHasBeenPrintedException) is needed for processing program's logic that usually stops
execution at this point.

Since version `5.0.0` `args-parser` provides two API: the old one and auxiliary API
that allows to define arguments in one line of code. Let's look.

## One-line syntax with commands

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

## One-line syntax with arguments

```cpp
#include <args-parser/all.hpp>

using namespace Args;

void process( bool b, const std::string & value )
{
  outStream() << "Boolean: " << b << " , value: \"" << value << "\"\n";
}

int main( int argc, char ** argv )
{
  try {
    CmdLine cmd( argc, argv );

    cmd.addArgWithFlagAndName( 'b', "bool", false, false, "Boolean flag",
        "Boolean flag, used without value" )
      .addArgWithFlagAndName( 'v', "value", true, false, "With value",
        "Argument with value", "", "VAL" )
      .addHelp( true, argv[ 0 ], "CLI with boolean and value." );

    cmd.parse();

    bool b = false;
    std::string value;

    if( cmd.isDefined( "-b" ) )
      b = true;

    if( cmd.isDefined( "-v" ) )
      value = cmd.value( "-v" );

    process( b, value );
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

## And with the old syntax


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

```txt
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

*That's it. Use it and enjoy it. Good luck.*

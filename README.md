This is Args.

Args is a small C++ header-only library for parsing command line arguments.


# Compilling

Just run build.rb which is a Ruby program and a project file or use QMake project.


# Requirements

For compiling Args you should have installed:

 * Ruby programming language (http://www.ruby-lang.org/en/)
 * mxx_ru Ruby gem (https://sourceforge.net/p/mxxru/wiki/Home/)

or
 * QMake, CMake

# Syntax

 * Argument starts with two dashes. For example ```--argument```.
 * Flag starts with one dash. For example ```-b```.
 * If argument has value than value can be specified after space or after equal sign.
For example ```--argument value``` and ```--argument=value```.
 * Flags without values can be combined into one block. For example
```-abc``` defines three flags ```-a```, ```-b``` and ```-c```.
 * The last flag in flag's block can be with value. Then it's possible to type the next
```-abc=value```. And here flag ```-c``` will have value ```value```.
 * MultiArg class provide ability to define more than one value for argument. This type
of arguments can be specified more than once in the command line. And the resulted value
of the argument will be ```std::list< std::string >```.

# Different types of strings.

Since version 4.0.0 Args can be built with different strings - std::string,
std::wstring and QString.

 * To build Args with std::wstring support define ARGS_WSTRING_BUILD
 * To build Args with QString support define ARGS_QSTRING_BUILD
 * If nothing was defined then Args will be build with std::string.
          
# Example

First of all you must know that practically all classes of the Args throws exceptions on errors
and there is one specific exceptions that inform you about that that help was shown. This specific
exception (HelpHasBeenPrintedException) is needed for processing program's logic that usually stops
execution at this point.

```
// Args include.
#include <Args/arg.hpp>
#include <Args/help.hpp>
#include <Args/cmd_line.hpp>
#include <Args/help_printer.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <iostream>


int main( int argc, char ** argv )
{
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
  //! This argument want to specify value specifier in the help. Let's do it.
  timeout.setValueSpecifier( "ms" );
  timeout.setDescription( "Timeout before new messages will be sent "
    "in milliseconds." );

  /*
    We create help now.
  */
  Args::Help help;
  // Set executable name to the help printer.
  help.setExecutable( argv[ 0 ] );
  //! And set description of the application.
  help.setAppDescription( "This application just show "
    "the power of Args." );

  try {
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
  }
  catch( const Args::HelpHasBeenPrintedException & )
  {
    return 0;
  }
  catch( const Args::BaseException & x )
  {
    std::cout << x.what();

    return 1;
  }

  return 0;
}
```

# Help output

```
This application just show the power of Args.

Usage: sample.help.exe -s, --host <arg> -p, --port <arg>
       [ --timeout <ms> ] [ -h, --help <arg> ]

Required arguments:
 -o, --host <arg>   Host. Can be "localhost", "any" or regular IP.

 -p, --port <arg>   Port number to create socket.

Optional arguments:
     --timeout <ms> Timeout before new messages will be sent in milliseconds.

 -h, --help <arg>   Print this help.
```

That's it. Use it and enjoy it. Good luck.

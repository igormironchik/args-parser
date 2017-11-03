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
  try {
    // We create Args::CmdLine instance for parsing
    //  command line arguments.
    Args::CmdLine cmd( argc, argv );

    //
    // And create our arguments.
    //

    // This is argument with flag "-o" and name "--host".
    // He is with value and required.
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

    // This argument have name "--timeout" only.
    // He is with value but optional.
    Args::Arg timeout( "timeout", true );
    // This argument want to specify value specifier in the help. Let's do it.
    timeout.setValueSpecifier( "ms" );
    timeout.setDescription( "Timeout before new messages will be sent "
      "in milliseconds." );

    // We create help now.
    Args::Help help;
    // Set executable name to the help printer.
    help.setExecutable( argv[ 0 ] );
    // And set description of the application.
    help.setAppDescription( "This application just show "
      "the power of Args." );

	// Now add our argument to the command line.
	cmd.addArg( host );
	cmd.addArg( port );
	cmd.addArg( timeout );
	cmd.addArg( help );

	// Now parse our arguments.
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
	std::cout << x.desc() << "\n";

	return 1;
  }

  return 0;
}

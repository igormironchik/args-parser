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

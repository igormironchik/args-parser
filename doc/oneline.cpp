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

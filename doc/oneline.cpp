// Args include.
#include <Args/all.hpp>

using namespace Args;

int main( int argc, char ** argv )
{
  try {
    CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

    cmd.addCommand( "add", ValueOptions::NoValue,
          "Add file." )
        .addAllOfGroup( "file group" )
          .addSubCommand( "file", false, ValueOptions::NoValue )
          .addArgWithFlagAndName( 'f', "file", true, false,
            "Name of the file.", "", "",
            "fn" )
        .end()
        .addArgWithFlagOnly( 'd', false, false, "Do job." )
      .end()
      .addCommand( "delete", ValueOptions::NoValue,
          "Delete file." )
        .addArgWithFlagOnly( 'd', false, false,
          "Do NOT job." )
      .end()
      .addArgWithFlagAndName( 'r', "recursive", false, false,
        "Do operation recursively?" )
      .addHelp( true, argv[ 0 ],
        "This application just show power of the Args help." );

    cmd.parse();

    if( cmd.isDefined( "-f" ) )
      const auto file = cmd.value( "-f" );
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

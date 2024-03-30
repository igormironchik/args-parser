

/*
	SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Args include.
#include <args-parser/all.hpp>

// C++ include.
#include <iostream>

using namespace Args;


#ifdef ARGS_WSTRING_BUILD
int wmain( int argc, wchar_t ** argv )
#else
int main( int argc, char ** argv )
#endif
{
	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		Command add( SL( "add" ), ValueOptions::NoValue, true );
		add.setDescription( SL( "Add item." ) );
		add.setLongDescription( SL( "Add item for monitoring. The type "
			"of item is defined by subcommand." ) );

		Command del( SL( "del" ), ValueOptions::NoValue, true );
		del.setDescription( SL( "Delete item." ) );
		del.setLongDescription( SL( "Delete item from monitoring. The type "
			"of item is defined by subcommand." ) );

		Command file( SL( "file" ), ValueOptions::ManyValues );
		file.setDescription( SL( "File item." ) );
		file.setLongDescription( SL( "File item to work with." ) );
		file.setValueSpecifier( SL( "fn" ) );

		Arg arg( SL( 'w' ), SL( "work" ), false );
		arg.setDescription( SL( "Do the work fith files." ) );
		arg.setLongDescription( SL( "Do the work set in the configuration with files." ) );

		file.addArg( arg );

		add.addArg( file );
		del.addArg( file );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( argv[0] );

		cmd.addArg( add );
		cmd.addArg( del );
		cmd.addArg( help );

		cmd.parse();

		Args::outStream() << SL( "Files:\n\n" );

		for( const auto & name: file.values() )
			Args::outStream() << name << SL( "\n" );

		Args::outStream() << SL( "\n" );
	}
	catch( const HelpHasBeenPrintedException & )
	{
	}
	catch( const BaseException & x )
	{
		Args::outStream() << x.desc() << SL( "\n" );
	}

	return 0;
}

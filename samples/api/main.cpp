

/*
	SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Args include.
#include <args-parser/all.hpp>


using namespace Args;


#ifdef ARGS_WSTRING_BUILD
int wmain( int argc, wchar_t ** argv )
#else
int main( int argc, char ** argv )
#endif
{
	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		cmd.addCommand( SL( "add" ), ValueOptions::ManyValues, false,
				SL( "Add file(s)." ), SL( "Add file(s) for monitoring." ), SL( "" ), SL( "fn" ) )
			.end()
			.addCommand( SL( "del" ), ValueOptions::ManyValues, false,
				SL( "Delete file(s)." ), SL( "Delete file(s) from monitoring." ), SL( "" ), SL( "fn" ) )
			.end()
			.addHelp( true, argv[ 0 ],
				SL( "This application just show power of the Args help." ) );

		cmd.parse();
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

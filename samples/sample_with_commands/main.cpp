

/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013-2020 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// Args include.
#include <Args/all.hpp>

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

		add.addCommand( file );
		del.addCommand( file );

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

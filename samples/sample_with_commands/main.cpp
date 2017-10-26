

/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013-2017 Igor Mironchik

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

		Arg file( SL( 'f' ), SL( "file" ), true, true );
		file.setDescription( SL( "Name of the file." ) );
		file.setValueSpecifier( SL( "fn" ) );

		Command add( SL( "add" ) );
		add.setDescription( SL( "Add new file." ) );
		add.addArg( file );

		Arg d1( SL( 'd' ), false, false );
		d1.setDescription( SL( "Do job." ) );
		add.addArg( d1 );

		Command del( SL( "delete" ) );
		del.setDescription( SL( "Delete file." ) );
		del.addArg( file );

		Arg d2( SL( 'd' ), false, false );
		d2.setDescription( SL( "Do NOT job." ) );
		del.addArg( d2 );

		Arg recursieve( SL( 'r' ), SL( "recurcieve" ), false, false );
		recursieve.setDescription( SL( "Do operation recurcively?" ) );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( argv[0] );

		cmd.addArg( add );
		cmd.addArg( del );
		cmd.addArg( recursieve );
		cmd.addArg( help );

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

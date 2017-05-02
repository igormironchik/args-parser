

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
#include <Args/arg.hpp>
#include <Args/cmd_line.hpp>
#include <Args/help.hpp>
#include <Args/command.hpp>

// C++ include.
#include <iostream>

using namespace Args;


int main( int argc, char ** argv )
{
	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		Arg file( 'f', "file", true, true );
		file.setDescription( "Name of the file." );
		file.setValueSpecifier( "fn" );

		Command add( "add" );
		add.setDescription( "Add new file." );
		add.addArg( file );

		Arg d1( 'd', false, false );
		d1.setDescription( "Do job." );
		add.addArg( d1 );

		Command del( "delete" );
		del.setDescription( "Delete file." );
		del.addArg( file );

		Arg d2( 'd', false, false );
		d2.setDescription( "Do NOT job." );
		del.addArg( d2 );

		Arg recursieve( 'r', "recurcieve", false, false );
		recursieve.setDescription( "Do operation recurcively?" );

		Help help;
		help.setAppDescription(
			"This application just show power of the Args help." );
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
		std::cout << x.what();
	}

	return 0;
}

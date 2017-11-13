

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


using namespace Args;


#ifdef ARGS_WSTRING_BUILD
int wmain( int argc, wchar_t ** argv )
#else
int main( int argc, char ** argv )
#endif
{
	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		cmd.addCommand( SL( "add" ), ValueOptions::NoValue,
					SL( "Add file." ) )
				.addArgWithFlagAndName( SL( 'f' ), SL( "file" ), true, true,
					SL( "Name of the file." ), SL( "" ), SL( "" ),
					SL( "fn" ) )
				.addArgWithFlagOnly( SL( 'd' ), false, false, SL( "Do job." ) )
			.end()
			.addCommand( SL( "delete" ), ValueOptions::NoValue,
					SL( "Delete file." ) )
				.addArgWithFlagOnly( SL( 'd' ), false, false,
					SL( "Do NOT job." ) )
			.end()
			.addArgWithFlagAndName( SL( 'r' ), SL( "recursive" ), false, false,
				SL( "Do operation recursively?" ) )
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

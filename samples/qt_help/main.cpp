
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

// Qt include.
#include <QString>
#include <QTextStream>


int main( int argc, char ** argv )
{
	try {
		/*
			We create Args::CmdLine instance for parsing
			command line arguments.
		*/
		Args::CmdLine cmd( argc, argv );

		/*
			And create our arguments.
		*/

		/*
			This is argument with flag "-h" and name "--host".
			He is with value and required.
		*/
		Args::Arg host( QChar( 's' ), QLatin1String( "host" ),
			// Argument is with value.
			true,
			// Argument is required.
			true );
		// Set description of the argument.
		host.setDescription( QLatin1String( "Host. Can be \"localhost\", "
			"\"any\" or regular IP." ) );
		// We can specify long description too.
		host.setLongDescription( QLatin1String( "Host. This argument told to the "
			"application where to open socket for communication." ) );

		Args::Arg port( QChar( 'p' ), QLatin1String( "port" ), true, true );
		port.setDescription( QLatin1String( "Port number to create socket." ) );

		/*
			This argument have name "--timeout" only.
			He is with value but optional.
		*/
		Args::Arg timeout( QLatin1String( "timeout" ), true );
		// This argument want to specify value specifier in the help. Let's do it.
		timeout.setValueSpecifier( QLatin1String( "ms" ) );
		timeout.setDescription( QLatin1String( "Timeout before new messages will be "
			"sent in milliseconds." ) );

		/*
			We create help now.
		*/
		Args::Help help;
		// Set executable name to the help printer.
		help.setExecutable( argv[ 0 ] );
		// And set description of the application.
		help.setAppDescription( QLatin1String( "This application just show "
			"the power of Args." ) );

		/*
			Now add our argument to the command line.
		*/
		cmd.addArg( host );
		cmd.addArg( port );
		cmd.addArg( timeout );
		cmd.addArg( help );

		/*
			Now parse our arguments.
		*/
		cmd.parse();
	}
	catch( const Args::HelpHasBeenPrintedException & )
	{
		return 0;
	}
	catch( const Args::BaseException & x )
	{
		QTextStream out( stdout );

		out << x.desc() << endl;

		return 1;
	}

	return 0;
}

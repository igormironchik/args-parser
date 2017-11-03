
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

// UnitTest include.
#include <UnitTest/unit_test.hpp>

// Args include.
#include <Args/all.hpp>

#ifdef ARGS_QSTRING_BUILD
	#include <QString>
	#include <QTextStream>
#else
	#include <sstream>
#endif

namespace Args {

#ifdef ARGS_WSTRING_BUILD
	std::wstringstream g_argsOutStream;
#elif defined( ARGS_QSTRING_BUILD )
	QString g_string;
	QTextStream g_argsOutStream( &g_string );
#else
	std::stringstream g_argsOutStream;
#endif

} /* namespace Args */


using namespace Args;

#ifdef ARGS_WSTRING_BUILD
	using CHAR = String::value_type;
#else
	using CHAR = char;
#endif


TEST( HelpTestCase, TestSimpleHelp )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd( argc, argv );

		Arg host( SL( 's' ), SL( "host" ),
			true, true );
		host.setDescription( SL( "Host. Can be \"localhost\", "
			"\"any\" or regular IP." ) );
		host.setLongDescription( SL( "Host. This argument told to the "
			"application where to open socket for communication." ) );

		Arg port( SL( 'p' ), SL( "port" ), true, true );
		port.setDescription( SL( "Port number to create socket." ) );

		Arg timeout( SL( "timeout" ), true );
		timeout.setValueSpecifier( SL( "ms" ) );
		timeout.setDescription( SL( "Timeout before new messages will be "
			"sent in milliseconds." ) );

		Help help;
		help.setExecutable( SL( "executable" ) );
		help.setAppDescription( SL( "This application just show "
			"the power of Args." ) );

		cmd.addArg( host );
		cmd.addArg( port );
		cmd.addArg( timeout );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"This application just show the power of Args. \n"
			"\n"
			"Usage: executable -s, --host <arg> -p, --port <arg> [ -h, \n"
			"       --help <arg> ] [ --timeout <ms> ] \n"
			"\n"
			"Required arguments:\n"
			" -s, --host <arg>   Host. Can be \"localhost\", \"any\" or regular IP. \n"
			"\n"
			" -p, --port <arg>   Port number to create socket. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg>   Print this help. \n"
			"\n"
			"     --timeout <ms> Timeout before new messages will be sent in milliseconds. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"This application just show the power of Args. \n"
			"\n"
			"Usage: executable -s, --host <arg> -p, --port <arg> [ -h, \n"
			"       --help <arg> ] [ --timeout <ms> ] \n"
			"\n"
			"Required arguments:\n"
			" -s, --host <arg>   Host. Can be \"localhost\", \"any\" or regular IP. \n"
			"\n"
			" -p, --port <arg>   Port number to create socket. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg>   Print this help. \n"
			"\n"
			"     --timeout <ms> Timeout before new messages will be sent in milliseconds. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestArgHelp )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h=-s" ) };

	try {
		CmdLine cmd( argc, argv );

		Arg host( SL( 's' ), SL( "host" ),
			true, true );
		host.setDescription( SL( "Host. Can be \"localhost\", "
			"\"any\" or regular IP." ) );
		host.setLongDescription( SL( "Host. This argument told to the "
			"application where to open socket for communication." ) );

		Arg port( SL( 'p' ), SL( "port" ), true, true );
		port.setDescription( SL( "Port number to create socket." ) );

		Arg timeout( SL( "timeout" ), true );
		timeout.setValueSpecifier( SL( "ms" ) );
		timeout.setDescription( SL( "Timeout before new messages will be "
			"sent in milliseconds." ) );

		Help help;
		help.setExecutable( SL( "executable" ) );
		help.setAppDescription( SL( "This application just show "
			"the power of Args." ) );

		cmd.addArg( host );
		cmd.addArg( port );
		cmd.addArg( timeout );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Usage: -s, --host <arg> \n"
			"\n"
			"       Host. This argument told to the application where to open socket \n"
			"       for communication. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Usage: -s, --host <arg> \n"
			"\n"
			"       Host. This argument told to the application where to open socket \n"
			"       for communication. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestMisspelling )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "--hots" ) };

	try {
		CmdLine cmd( argc, argv );

		Arg host( SL( 's' ), SL( "host" ),
			true, true );
		host.setDescription( SL( "Host. Can be \"localhost\", "
			"\"any\" or regular IP." ) );
		host.setLongDescription( SL( "Host. This argument told to the "
			"application where to open socket for communication." ) );

		Arg port( SL( 'p' ), SL( "port" ), true, true );
		port.setDescription( SL( "Port number to create socket." ) );

		Arg timeout( SL( "timeout" ), true );
		timeout.setValueSpecifier( SL( "ms" ) );
		timeout.setDescription( SL( "Timeout before new messages will be "
			"sent in milliseconds." ) );

		Help help;
		help.setExecutable( SL( "executable" ) );
		help.setAppDescription( SL( "This application just show "
			"the power of Args." ) );

		cmd.addArg( host );
		cmd.addArg( port );
		cmd.addArg( timeout );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const BaseException & x )
	{
		CHECK_CONDITION( x.desc() ==
			SL( "Unknown argument \"--hots\".\n"
				"\n"
				"Probably you mean \"--host\"." ) )

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpWithCommands )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		Arg file( SL( 'f' ), SL( "file" ), true, true );
		file.setDescription( SL( "Name of the file." ) );
		file.setValueSpecifier( SL( "fn" ) );

		Command add( SL( "add" ) );
		add.setDescription( SL( "Add new file." ) );
		add.addArg( file );

		Arg d1( Char( SL( 'd' ) ), false, false );
		d1.setDescription( SL( "Do job." ) );
		add.addArg( d1 );

		Command del( SL( "delete" ) );
		del.setDescription( SL( "Delete file." ) );
		del.addArg( file );

		Arg d2( Char( SL( 'd' ) ), false, false );
		d2.setDescription( SL( "Do NOT job." ) );
		del.addArg( d2 );

		Arg recursieve( SL( 'r' ), SL( "recurcieve" ), false, false );
		recursieve.setDescription( SL( "Do operation recurcively?" ) );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( SL( "executable" )  );

		cmd.addArg( add );
		cmd.addArg( del );
		cmd.addArg( recursieve );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"This application just show power of the Args help. \n"
			"\n"
			"Usage: executable <command> <args>\n"
			"\n"
			"  add    Add new file. \n"
			"  delete Delete file. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n"
			"\n"
			" -r, --recurcieve Do operation recurcively? \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"This application just show power of the Args help. \n"
			"\n"
			"Usage: executable <command> <args>\n"
			"\n"
			"  add    Add new file. \n"
			"  delete Delete file. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n"
			"\n"
			" -r, --recurcieve Do operation recurcively? \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpOfCommand )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h=add" ) };

	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		Arg file( SL( 'f' ), SL( "file" ), true, true );
		file.setDescription( SL( "Name of the file." ) );
		file.setValueSpecifier( SL( "fn" ) );

		Command add( SL( "add" ) );
		add.setDescription( SL( "Add new file." ) );
		add.addArg( file );

		Arg d1( Char( SL( 'd' ) ), false, false );
		d1.setDescription( SL( "Do job." ) );
		add.addArg( d1 );

		Command del( SL( "delete" ) );
		del.setDescription( SL( "Delete file." ) );
		del.addArg( file );

		Arg d2( Char( SL( 'd' ) ), false, false );
		d2.setDescription( SL( "Do NOT job." ) );
		del.addArg( d2 );

		Arg recursieve( SL( 'r' ), SL( "recurcieve" ), false, false );
		recursieve.setDescription( SL( "Do operation recurcively?" ) );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( SL( "executable" )  );

		cmd.addArg( add );
		cmd.addArg( del );
		cmd.addArg( recursieve );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Add new file. \n"
			"\n"
			"Required arguments:\n"
			" -f, --file <fn> Name of the file. \n"
			"\n"
			"Optional arguments:\n"
			" -d              Do job. \n"
			"\n"
			"\n"
			"Global arguments:\n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n"
			"\n"
			" -r, --recurcieve Do operation recurcively? \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Add new file. \n"
			"\n"
			"Required arguments:\n"
			" -f, --file <fn> Name of the file. \n"
			"\n"
			"Optional arguments:\n"
			" -d              Do job. \n"
			"\n"
			"\n"
			"Global arguments:\n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n"
			"\n"
			" -r, --recurcieve Do operation recurcively? \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpOfArgOfCommand )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h=add" ), SL( "-f" ) };

	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		Arg file( SL( 'f' ), SL( "file" ), true, true );
		file.setDescription( SL( "Name of the file." ) );
		file.setValueSpecifier( SL( "fn" ) );

		Command add( SL( "add" ) );
		add.setDescription( SL( "Add new file." ) );
		add.addArg( file );

		Arg d1( Char( SL( 'd' ) ), false, false );
		d1.setDescription( SL( "Do job." ) );
		add.addArg( d1 );

		Command del( SL( "delete" ) );
		del.setDescription( SL( "Delete file." ) );
		del.addArg( file );

		Arg d2( Char( SL( 'd' ) ), false, false );
		d2.setDescription( SL( "Do NOT job." ) );
		del.addArg( d2 );

		Arg recursieve( SL( 'r' ), SL( "recurcieve" ), false, false );
		recursieve.setDescription( SL( "Do operation recurcively?" ) );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( SL( "executable" )  );

		cmd.addArg( add );
		cmd.addArg( del );
		cmd.addArg( recursieve );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Usage: -f, --file <fn> \n"
			"\n"
			"       Name of the file. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Usage: -f, --file <fn> \n"
			"\n"
			"       Name of the file. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpOfSubcommand )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h=add" ), SL( "file" ) };

	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		Command add( SL( "add" ) );
		add.setDescription( SL( "Add new file." ) );

		ArgAsCommand file( SL( "file" ), false, ValueOptions::OneValue );
		file.setDescription( SL( "Add file." ) );
		file.setLongDescription( SL( "Add file. File can exist but if "
			"it's not so new file will be created." ) );
		add.addArg( file );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( SL( "executable" )  );

		cmd.addArg( add );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Usage: [ file <arg> ] \n"
			"\n"
			"       Add file. File can exist but if it's not so new file will be \n"
			"       created. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Usage: [ file <arg> ] \n"
			"\n"
			"       Add file. File can exist but if it's not so new file will be \n"
			"       created. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpOfArgAsCommand )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h=add" ) };

	try {
		CmdLine cmd( argc, argv );

		ArgAsCommand add( SL( "add" ) );
		add.setDescription( SL( "Add new file." ) );
		add.setLongDescription( SL( "Add file. File can exist but if it's "
			"not so new file will be created." ) );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( SL( "executable" )  );

		cmd.addArg( add );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Usage: [ add ] \n"
			"\n"
			"       Add file. File can exist but if it's not so new file will be \n"
			"       created. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Usage: [ add ] \n"
			"\n"
			"       Add file. File can exist but if it's not so new file will be \n"
			"       created. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestWrongArgForHelp )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ), SL( "add" ) };

	try {
		CmdLine cmd( argc, argv );

		Arg host( SL( 's' ), SL( "host" ),
			true, true );
		host.setDescription( SL( "Host. Can be \"localhost\", "
			"\"any\" or regular IP." ) );
		host.setLongDescription( SL( "Host. This argument told to the "
			"application where to open socket for communication." ) );

		Arg port( SL( 'p' ), SL( "port" ), true, true );
		port.setDescription( SL( "Port number to create socket." ) );

		Arg timeout( SL( "timeout" ), true );
		timeout.setValueSpecifier( SL( "ms" ) );
		timeout.setDescription( SL( "Timeout before new messages will be "
			"sent in milliseconds." ) );

		Help help;
		help.setExecutable( SL( "executable" ) );
		help.setAppDescription( SL( "This application just show "
			"the power of Args." ) );

		cmd.addArg( host );
		cmd.addArg( port );
		cmd.addArg( timeout );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"This application just show the power of Args. \n"
			"\n"
			"Usage: executable -s, --host <arg> -p, --port <arg> [ -h, \n"
			"       --help <arg> ] [ --timeout <ms> ] \n"
			"\n"
			"Required arguments:\n"
			" -s, --host <arg>   Host. Can be \"localhost\", \"any\" or regular IP. \n"
			"\n"
			" -p, --port <arg>   Port number to create socket. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg>   Print this help. \n"
			"\n"
			"     --timeout <ms> Timeout before new messages will be sent in milliseconds. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"This application just show the power of Args. \n"
			"\n"
			"Usage: executable -s, --host <arg> -p, --port <arg> [ -h, \n"
			"       --help <arg> ] [ --timeout <ms> ] \n"
			"\n"
			"Required arguments:\n"
			" -s, --host <arg>   Host. Can be \"localhost\", \"any\" or regular IP. \n"
			"\n"
			" -p, --port <arg>   Port number to create socket. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg>   Print this help. \n"
			"\n"
			"     --timeout <ms> Timeout before new messages will be sent in milliseconds. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestRequiredAllOfGroupHelp )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd;

		cmd.addAllOfGroup( SL( "group" ), true )
				.addArgWithFlagOnly( SL( 'a' ), true, false,
					SL( "Argument." ) )
			.end()
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Test help. \n"
			"\n"
			"Usage: executable -a <arg> [ -h, --help <arg> ] \n"
			"\n"
			"Required arguments:\n"
			" -a <arg>         Argument. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Test help. \n"
			"\n"
			"Usage: executable -a <arg> [ -h, --help <arg> ] \n"
			"\n"
			"Required arguments:\n"
			" -a <arg>         Argument. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestNotRequiredAllOfGroupHelp )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd;

		cmd.addAllOfGroup( SL( "group" ) )
				.addArgWithFlagOnly( SL( 'a' ), true, false,
					SL( "Argument." ) )
			.end()
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Test help. \n"
			"\n"
			"Usage: executable [ -a <arg> ] [ -h, --help <arg> ] \n"
			"\n"
			"Optional arguments:\n"
			" -a <arg>         Argument. \n"
			"\n"
			" -h, --help <arg> Print this help. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Test help. \n"
			"\n"
			"Usage: executable [ -a <arg> ] [ -h, --help <arg> ] \n"
			"\n"
			"Optional arguments:\n"
			" -a <arg>         Argument. \n"
			"\n"
			" -h, --help <arg> Print this help. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestArgAsCommandHelp )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ), SL( "sub" ) };

	try {
		CmdLine cmd;

		cmd.addArgAsCommand( SL( "sub" ), true, ValueOptions::OneValue,
				SL( "ArgAsCommand." ) )
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Usage: sub <arg> \n"
			"\n"
			"       ArgAsCommand. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Usage: sub <arg> \n"
			"\n"
			"       ArgAsCommand. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestFlagWithBigVSHelp )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd;

		cmd.addArgWithFlagOnly( SL( 'a' ), true, false,
				SL( "Argument." ), SL( "" ), SL( "" ), SL( "bin|lib|shared" ) )
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Test help. \n"
			"\n"
			"Usage: executable [ -a <bin|lib|shared> ] [ -h, --help <arg> ] \n"
			"\n"
			"Optional arguments:\n"
			" -a <bin|lib|shared> Argument. \n"
			"\n"
			" -h, --help <arg>    Print this help. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Test help. \n"
			"\n"
			"Usage: executable [ -a <bin|lib|shared> ] [ -h, --help <arg> ] \n"
			"\n"
			"Optional arguments:\n"
			" -a <bin|lib|shared> Argument. \n"
			"\n"
			" -h, --help <arg>    Print this help. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestFlagWithBigVSHelp2 )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd;

		cmd.addArgWithFlagOnly( SL( 'a' ), true, false,
				SL( "Argument." ), SL( "" ), SL( "" ), SL( "bin|lib|shared" ) )
			.addArgWithNameOnly( SL( "name" ), false, false, SL( "Name." ) )
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Test help. \n"
			"\n"
			"Usage: executable [ -a <bin|lib|shared> ] [ -h, --help <arg> ] \n"
			"       [ --name ] \n"
			"\n"
			"Optional arguments:\n"
			" -a <bin|lib|shared> Argument. \n"
			"\n"
			" -h, --help <arg>    Print this help. \n"
			"\n"
			"     --name          Name. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Test help. \n"
			"\n"
			"Usage: executable [ -a <bin|lib|shared> ] [ -h, --help <arg> ] \n"
			"       [ --name ] \n"
			"\n"
			"Optional arguments:\n"
			" -a <bin|lib|shared> Argument. \n"
			"\n"
			" -h, --help <arg>    Print this help. \n"
			"\n"
			"     --name          Name. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestFlagAndNameWithBigVSHelp )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd;

		cmd.addArgWithFlagAndName( SL( 'a' ), SL( "arg" ), true, false,
				SL( "Argument." ), SL( "" ), SL( "" ), SL( "bin|lib|shared" ) )
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Test help. \n"
			"\n"
			"Usage: executable [ -a, --arg <bin|lib|shared> ] [ -h, --help <arg> ] \n"
			"\n"
			"Optional arguments:\n"
			" -a, --arg <bin|lib|shared> Argument. \n"
			"\n"
			" -h, --help <arg>           Print this help. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Test help. \n"
			"\n"
			"Usage: executable [ -a, --arg <bin|lib|shared> ] [ -h, --help <arg> ] \n"
			"\n"
			"Optional arguments:\n"
			" -a, --arg <bin|lib|shared> Argument. \n"
			"\n"
			" -h, --help <arg>           Print this help. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpWithCommandAndArgAsCommand )
{
	const int argc = 2;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ) };

	try {
		CmdLine cmd;

		cmd.addCommand( SL( "cmd" ), ValueOptions::OneValue, SL( "Command." ) )
			.end()
			.addArgAsCommand( SL( "arg" ), true, ValueOptions::OneValue,
				SL( "Argument." ) )
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Test help. \n"
			"\n"
			"Usage: executable <command> <args>\n"
			"\n"
			"  cmd <arg> Command. \n"
			"\n"
			"Required arguments:\n"
			"     arg <arg>    Argument. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Test help. \n"
			"\n"
			"Usage: executable <command> <args>\n"
			"\n"
			"  cmd <arg> Command. \n"
			"\n"
			"Required arguments:\n"
			"     arg <arg>    Argument. \n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpWithCommandAndArgAsCommandForCmd )
{
	const int argc = 3;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ), SL( "cmd" ) };

	try {
		CmdLine cmd;

		cmd.addCommand( SL( "cmd" ), ValueOptions::OneValue, SL( "Command." ) )
			.end()
			.addArgAsCommand( SL( "arg" ), true, ValueOptions::OneValue,
				SL( "Argument." ) )
			.addHelp( true, SL( "executable" ), SL( "Test help." ) );

		cmd.parse( argc, argv );
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Command. \n"
			"\n"
			"Global arguments:\n\n"
			"Required arguments:\n"
			"     arg <arg>    Argument. \n\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Command. \n"
			"\n"
			"Global arguments:\n\n"
			"Required arguments:\n"
			"     arg <arg>    Argument. \n\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

TEST( HelpTestCase, TestHelpOfCommand2 )
{
	const int argc = 4;
	const CHAR * argv[ argc ] = { SL( "program.exe" ),
		SL( "-h" ), SL( "add" ), SL( "dummy" ) };

	try {
		CmdLine cmd( argc, argv, CmdLine::CommandIsRequired );

		Arg file( SL( 'f' ), SL( "file" ), true, true );
		file.setDescription( SL( "Name of the file." ) );
		file.setValueSpecifier( SL( "fn" ) );

		Command add( SL( "add" ) );
		add.setDescription( SL( "Add new file." ) );
		add.addArg( file );

		Arg d1( Char( SL( 'd' ) ), false, false );
		d1.setDescription( SL( "Do job." ) );
		add.addArg( d1 );

		Command del( SL( "delete" ) );
		del.setDescription( SL( "Delete file." ) );
		del.addArg( file );

		Arg d2( Char( SL( 'd' ) ), false, false );
		d2.setDescription( SL( "Do NOT job." ) );
		del.addArg( d2 );

		Arg recursieve( SL( 'r' ), SL( "recurcieve" ), false, false );
		recursieve.setDescription( SL( "Do operation recurcively?" ) );

		Help help;
		help.setAppDescription(
			SL( "This application just show power of the Args help." ) );
		help.setExecutable( SL( "executable" )  );

		cmd.addArg( add );
		cmd.addArg( del );
		cmd.addArg( recursieve );
		cmd.addArg( help );

		cmd.parse();
	}
	catch( const HelpHasBeenPrintedException & )
	{
#ifdef ARGS_QSTRING_BUILD
		CHECK_CONDITION( g_string ==
			"Add new file. \n"
			"\n"
			"Required arguments:\n"
			" -f, --file <fn> Name of the file. \n"
			"\n"
			"Optional arguments:\n"
			" -d              Do job. \n"
			"\n"
			"\n"
			"Global arguments:\n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n"
			"\n"
			" -r, --recurcieve Do operation recurcively? \n\n" )

		g_string.clear();
#else
		CHECK_CONDITION( g_argsOutStream.str() == SL(
			"Add new file. \n"
			"\n"
			"Required arguments:\n"
			" -f, --file <fn> Name of the file. \n"
			"\n"
			"Optional arguments:\n"
			" -d              Do job. \n"
			"\n"
			"\n"
			"Global arguments:\n"
			"\n"
			"Optional arguments:\n"
			" -h, --help <arg> Print this help. \n"
			"\n"
			" -r, --recurcieve Do operation recurcively? \n\n" ) )

		g_argsOutStream.str( SL( "" ) );
#endif

		return;
	}

	CHECK_CONDITION( false )
}

int main()
{

	// Suppressing warning.
	outStream();

	RUN_ALL_TESTS()

	return 0;
}

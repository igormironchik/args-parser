
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013 Igor Mironchik

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
#include <Args/context.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <algorithm>


namespace Args {

//
// Arg
//

Arg::Arg( char flag, const std::string & name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_flag( 1, flag )
	,	m_name( name )
	,	m_isDefined( false )
	,	m_valueSpecifier( "arg" )
{
}

Arg::Arg( char flag, const char * name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_flag( 1, flag )
	,	m_name( name )
	,	m_isDefined( false )
	,	m_valueSpecifier( "arg" )
{
}

Arg::Arg( char flag,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_flag( 1, flag )
	,	m_isDefined( false )
	,	m_valueSpecifier( "arg" )
{
}

Arg::Arg( const std::string & name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_name( name )
	,	m_isDefined( false )
	,	m_valueSpecifier( "arg" )
{
}

Arg::~Arg()
{
}

ArgIface *
Arg::isItYou( const std::string & name )
{
	if( isArgument( name ) && name.substr( 2 ) == m_name )
		return this;
	else if( isFlag( name ) && name.substr( 1 ) == m_flag )
		return this;
	else
		return nullptr;
}

void
Arg::process( Context & context )
{
	if( !isDefined() )
	{
		if( !isWithValue() )
			setDefined( true );
		else
		{
			if( !context.atEnd() )
			{
				setValue( *context.next() );
				setDefined( true );
			}
			else
				throw BaseException( std::string( "Argument \"" ) +
					name() + "\" requires value but it's not presented." );
		}
	}
	else
		throw BaseException( std::string( "Argument \"" ) +
			name() + "\" already defined." );
}

std::string
Arg::name() const
{
	if( !m_name.empty() )
		return "--" + m_name;
	else
		return "-" + m_flag;
}

void
Arg::checkCorrectnessBeforeParsing( std::list< std::string > & flags,
	std::list< std::string > & names ) const
{
	if( !m_flag.empty() )
	{
		if( isCorrectFlag( m_flag ) )
		{
			auto it = std::find( flags.begin(), flags.end(), m_flag );

			if( it != flags.end() )
				throw BaseException( std::string( "Redefinition of argument "
					"witg flag \"" ) + m_flag + "\"." );
			else
				flags.push_back( m_flag );
		}
		else
			throw BaseException( std::string( "Dissallowed flag \"" ) +
				m_flag + "\"." );
	}

	if( !m_name.empty() )
	{
		if( isCorrectName( m_name ) )
		{
			auto it = std::find( names.begin(), names.end(), m_name );

			if( it != names.end() )
				throw BaseException( std::string( "Redefinition of argument "
					"with name \"" ) + m_name + "\"." );
			else
				names.push_back( m_name );
		}
		else
			throw BaseException( std::string( "Dissallowed name \"" ) +
				m_name + "\"." );
	}

	if( m_flag.empty() && m_name.empty() )
		throw BaseException( std::string( "Arguments with empty flag and name "
			"are dissallowed." ) );
}

void
Arg::checkCorrectnessAfterParsing() const
{
	if( isRequired() && !isDefined() )
		throw BaseException( std::string( "Undefined required argument \"" ) +
			name() + "\"." );
}

bool
Arg::isWithValue() const
{
	return m_isWithValue;
}

void
Arg::setWithValue( bool on )
{
	m_isWithValue = on;
}

bool
Arg::isRequired() const
{
	return m_isRequired;
}

void
Arg::setRequired( bool on )
{
	m_isRequired = on;
}

bool
Arg::isDefined() const
{
	return m_isDefined;
}

void
Arg::setDefined( bool on )
{
	m_isDefined = on;
}

const std::string &
Arg::value() const
{
	return m_value;
}

void
Arg::setValue( const std::string & v )
{
	m_value = v;
}

const std::string &
Arg::flag() const
{
	return m_flag;
}

void
Arg::setFlag( char f )
{
	m_flag = std::string( 1, f );
}

const std::string &
Arg::argumentName() const
{
	return m_name;
}

void
Arg::setArgumentName( const std::string & name )
{
	m_name = name;
}

const std::string &
Arg::valueSpecifier() const
{
	return m_valueSpecifier;
}

void
Arg::setValueSpecifier( const std::string & vs )
{
	m_valueSpecifier = vs;
}

const std::string &
Arg::description() const
{
	return m_description;
}

void
Arg::setDescription( const std::string & desc )
{
	m_description = desc;
}

const std::string &
Arg::longDescription() const
{
	if( !m_longDescription.empty() )
		return m_longDescription;
	else
		return m_description;
}

void
Arg::setLongDescription( const std::string & desc )
{
	m_longDescription = desc;
}

} /* namespace Args */

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

Arg::Arg( const char & flag, const std::string & name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_flag( 1, flag )
	,	m_name( name )
{
}

Arg::Arg( const char & flag,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_flag( 1, flag )
{
}

Arg::Arg( const std::string & name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_name( name )
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
	if( !isWithValue() )
		m_isDefined = true;
	else
	{
		if( !context.atEnd() )
		{
			m_value = *context.next();
			m_isDefined = true;
		}
		else
			throw BaseException( std::string( "Argument \"" ) +
				name() + "\" requires value but it's not presented." );
	}
}

const std::string &
Arg::name() const
{
	if( !m_name.empty() )
		return m_name;
	else
		return m_flag;
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

bool
Arg::isRequired() const
{
	return m_isRequired;
}

bool
Arg::isDefined() const
{
	return m_isDefined;
}

const std::string &
Arg::value() const
{
	return m_value;
}

} /* namespace Args */

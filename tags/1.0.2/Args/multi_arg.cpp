
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
#include <Args/multi_arg.hpp>
#include <Args/context.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <algorithm>


namespace Args {

//
// MultiArg
//

MultiArg::MultiArg( char flag, const std::string & name,
	bool isWithValue, bool isRequired )
	:	Arg( flag, name, isWithValue, isRequired )
	,	m_count( 0 )
{
}

MultiArg::MultiArg( char flag, const char * name,
	bool isWithValue, bool isRequired )
	:	Arg( flag, name, isWithValue, isRequired )
	,	m_count( 0 )
{
}

MultiArg::MultiArg( char flag,
	bool isWithValue, bool isRequired )
	:	Arg( flag, isWithValue, isRequired )
	,	m_count( 0 )
{
}

MultiArg::MultiArg( const std::string & name,
	bool isWithValue, bool isRequired )
	:	Arg( name, isWithValue, isRequired )
	,	m_count( 0 )
{
}

MultiArg::~MultiArg()
{
}

const std::string &
MultiArg::value() const
{
	if( !m_values.empty() )
		return m_values.front();
	else
		return Arg::value();
}

const std::list< std::string > &
MultiArg::values() const
{
	return m_values;
}

size_t
MultiArg::count() const
{
	if( !isWithValue() )
		return m_count;
	else
		return m_values.size();
}

void
MultiArg::process( Context & context )
{
	if( isWithValue() )
	{
		if( !context.atEnd() )
		{
			auto begin = context.begin();

			auto last = std::find_if( context.begin(), context.end(),
				[] ( const std::string & v ) -> bool
					{ return ( isArgument( v ) || isFlag( v ) ); }
			);

			if( last != begin )
			{
				setDefined( true );

				begin = context.next();

				while( begin !=last )
				{
					m_values.push_back( *begin );

					begin = context.next();
				}

				if( last != context.end() )
					context.putBack();
			}
			else
				throw BaseException( std::string( "Argument \"" ) +
					name() + "\" require value that wasn't presented." );
		}
		else
			throw BaseException( std::string( "Argument \"" ) +
				name() + "\" require value that wasn't presented." );
	}
	else
	{
		setDefined( true );

		++m_count;
	}
}

} /* namespace Args */


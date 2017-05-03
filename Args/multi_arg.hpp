
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

#ifndef ARGS__MULTI_ARG_HPP__INCLUDED
#define ARGS__MULTI_ARG_HPP__INCLUDED

// Args include.
#include "arg.hpp"
#include "context.hpp"
#include "exceptions.hpp"
#include "value_utils.hpp"
#include "utils.hpp"
#include "types.hpp"

// C++ include.
#include <utility>


namespace Args {

//
// MultiArg
//

/*!
	MultiArg is a class that presents argument in the command
	line taht can be presented more than once or can have more
	than one value.
*/
class MultiArg
	:	public Arg
{
public:
	//! Construct argument with flag and name.
	template< typename T >
	MultiArg(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		T && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	//! Construct argument only with flag, without name.
	explicit MultiArg(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	//! Construct argument only with name, without flag.
	template< typename T >
	explicit MultiArg(
		//! Name for this argument.
		T && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	virtual ~MultiArg();

	//! \return First value of this argument.
	virtual const String & value() const;

	//! \return All values for this argument.
	virtual const StringList & values() const;

	/*!
		\return Count of times that this argument was presented
		in the command line if argument without value or count of values
		if argument is with values.
	*/
	size_t count() const;


protected:
	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	void process(
		//! Context of the command line.
		Context & context ) override;

private:
	DISABLE_COPY( MultiArg )

	//! Values of this argument.
	StringList m_values;
	//! Counter.
	size_t m_count;
}; // class MultiArg


//
// MultiArg
//

template< typename T >
MultiArg::MultiArg( Char flag, T && name,
	bool isWithValue, bool isRequired )
	:	Arg( flag, std::forward< T > ( name ), isWithValue, isRequired )
	,	m_count( 0 )
{
}

inline
MultiArg::MultiArg( Char flag,
	bool isWithValue, bool isRequired )
	:	Arg( flag, isWithValue, isRequired )
	,	m_count( 0 )
{
}

template< typename T >
MultiArg::MultiArg( T && name,
	bool isWithValue, bool isRequired )
	:	Arg( std::forward< T > ( name ), isWithValue, isRequired )
	,	m_count( 0 )
{
}

inline
MultiArg::~MultiArg()
{
}

inline const String &
MultiArg::value() const
{
	if( !m_values.empty() )
		return m_values.front();
	else
		return Arg::value();
}

inline const StringList &
MultiArg::values() const
{
	return m_values;
}

inline size_t
MultiArg::count() const
{
	if( !isWithValue() )
		return m_count;
	else
		return m_values.size();
}

inline void
MultiArg::process( Context & context )
{
	if( isWithValue() )
	{
		setDefined( eatValues( context, m_values,
			String( "Argument \"" ) +
				name() + "\" require value that wasn't presented.",
			cmdLine() ) );
	}
	else
	{
		setDefined( true );

		++m_count;
	}
}

} /* namespace Args */

#endif // ARGS__MULTI_ARG_HPP__INCLUDED


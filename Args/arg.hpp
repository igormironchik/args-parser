
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

#ifndef ARGS__ARG_HPP__INCLUDED
#define ARGS__ARG_HPP__INCLUDED

// Args include.
#include <Args/arg_iface.hpp>
#include <Args/utils.hpp>
#include <Args/context.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <algorithm>


namespace Args {

//
// Arg
//

/*!
	Argument with one value that can be present only once
	in the command line
*/
class Arg
	:	public ArgIface
{
public:
	//! Construct argument with flag and name.
	template< typename T >
	Arg(
		//! Flag for this argument.
		char flag,
		//! Name for this argument.
		T && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	//! Construct argument only with flag, without name.
	explicit Arg(
		//! Flag for this argument.
		char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	//! Construct argument only with name, without flag.
	template< typename T >
	explicit Arg(
		//! Name for this argument.
		T && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	virtual ~Arg();

	/*!
		\return Name of the argument.

		If name is empty returned value should be a flag.
		I.e. for example "-t" or "--timeout"
	*/
	std::string name() const override;

	//! \return Is this argument with value?
	bool isWithValue() const override;
	//! Set is this argument with value.
	void setWithValue( bool on = true );

	//! \return Is this argument required?
	bool isRequired() const override;
	//! Set required.
	void setRequired( bool on = true );

	//! \return Is this argument defined?
	bool isDefined() const override;
	//! Set defined.
	void setDefined( bool on = true );

	//! \return Value of this argument.
	virtual const std::string & value() const;
	//! Set value.
	void setValue( const std::string & v );

	//! \return Flag.
	const std::string & flag() const override;
	//! Set flag.
	void setFlag( char f );

	//! \return Argument name.
	const std::string & argumentName() const override;
	//! Set argument name.
	void setArgumentName( const std::string & name );

	//! \return Value specifier.
	const std::string & valueSpecifier() const override;
	//! Set value specifier.
	void setValueSpecifier( const std::string & vs );

	//! \return Description of the argument.
	const std::string & description() const override;
	//! Set description.
	void setDescription( const std::string & desc );

	//! \return Long description of the argument.
	const std::string & longDescription() const override;
	//! Set long description.
	void setLongDescription( const std::string & desc );

protected:
	/*!
		\return Argument for the given name.

		\retval Pointer to the ArgIface if this argument handles
			argument with the given name.
		\retval nullptr if this argument doesn't know about
			argument with name.
	*/
	ArgIface * isItYou(
		/*!
			Name of the argument. Can be for example "-t" or
			"--timeout".
		*/
		const std::string & name ) override;

	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	void process(
		//! Context of the command line.
		Context & context ) override;

	/*!
		Check correctness of the argument before parsing.

		Implementation of this method must add his flag
		and name to the flags and names.
	*/
	void checkCorrectnessBeforeParsing(
		//! All known flags.
		std::list< std::string > & flags,
		//! All known names.
		std::list< std::string > & names ) const override;

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override;

private:
	DISABLE_COPY( Arg )

	//! Is this argument with value?
	bool m_isWithValue;
	//! Is this argument required?
	bool m_isRequired;
	//! Is this argument defined?
	bool m_isDefined;
	//! Value of this argument.
	std::string m_value;
	//! Flag of this argument.
	std::string m_flag;
	//! Name of this argument.
	std::string m_name;
	//! Value specifier.
	std::string m_valueSpecifier;
	//! Description.
	std::string m_description;
	//! Long description.
	std::string m_longDescription;
}; // class Arg


//
// Arg
//

template< typename T >
Arg::Arg( char flag, T && name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_isDefined( false )
	,	m_flag( 1, flag )
	,	m_name( std::forward< T > ( name ) )
	,	m_valueSpecifier( "arg" )
{
}

inline
Arg::Arg( char flag,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_isDefined( false )
	,	m_flag( 1, flag )
	,	m_valueSpecifier( "arg" )
{
}

template< typename T >
Arg::Arg( T && name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_isDefined( false )
	,	m_name( std::forward< T > ( name ) )
	,	m_valueSpecifier( "arg" )
{
}

inline
Arg::~Arg()
{
}

inline ArgIface *
Arg::isItYou( const std::string & name )
{
	if( isArgument( name ) && name.substr( 2 ) == m_name )
		return this;
	else if( isFlag( name ) && name.substr( 1 ) == m_flag )
		return this;
	else
		return nullptr;
}

inline void
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

inline std::string
Arg::name() const
{
	if( !m_name.empty() )
		return "--" + m_name;
	else
		return "-" + m_flag;
}

inline void
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

inline void
Arg::checkCorrectnessAfterParsing() const
{
	if( isRequired() && !isDefined() )
		throw BaseException( std::string( "Undefined required argument \"" ) +
			name() + "\"." );
}

inline bool
Arg::isWithValue() const
{
	return m_isWithValue;
}

inline void
Arg::setWithValue( bool on )
{
	m_isWithValue = on;
}

inline bool
Arg::isRequired() const
{
	return m_isRequired;
}

inline void
Arg::setRequired( bool on )
{
	m_isRequired = on;
}

inline bool
Arg::isDefined() const
{
	return m_isDefined;
}

inline void
Arg::setDefined( bool on )
{
	m_isDefined = on;
}

inline const std::string &
Arg::value() const
{
	return m_value;
}

inline void
Arg::setValue( const std::string & v )
{
	m_value = v;
}

inline const std::string &
Arg::flag() const
{
	return m_flag;
}

inline void
Arg::setFlag( char f )
{
	m_flag = std::string( 1, f );
}

inline const std::string &
Arg::argumentName() const
{
	return m_name;
}

inline void
Arg::setArgumentName( const std::string & name )
{
	m_name = name;
}

inline const std::string &
Arg::valueSpecifier() const
{
	return m_valueSpecifier;
}

inline void
Arg::setValueSpecifier( const std::string & vs )
{
	m_valueSpecifier = vs;
}

inline const std::string &
Arg::description() const
{
	return m_description;
}

inline void
Arg::setDescription( const std::string & desc )
{
	m_description = desc;
}

inline const std::string &
Arg::longDescription() const
{
	if( !m_longDescription.empty() )
		return m_longDescription;
	else
		return m_description;
}

inline void
Arg::setLongDescription( const std::string & desc )
{
	m_longDescription = desc;
}

} /* namespace Args */

#endif // ARGS__ARG_HPP__INCLUDED

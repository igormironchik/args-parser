
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
#include "arg_iface.hpp"
#include "utils.hpp"
#include "context.hpp"
#include "exceptions.hpp"
#include "value_utils.hpp"
#include "types.hpp"

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
		Char flag,
		//! Name for this argument.
		T && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false );

	//! Construct argument only with flag, without name.
	explicit Arg(
		//! Flag for this argument.
		Char flag,
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
	String name() const override;

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
	virtual const String & value() const;
	//! Set value.
	virtual void setValue( const String & v );

	//! \return Flag.
	const String & flag() const override;
	//! Set flag.
	void setFlag( Char f );

	//! \return Argument name.
	const String & argumentName() const override;
	//! Set argument name.
	void setArgumentName( const String & name );

	//! \return Value specifier.
	const String & valueSpecifier() const override;
	//! Set value specifier.
	void setValueSpecifier( const String & vs );

	//! \return Description of the argument.
	const String & description() const override;
	//! Set description.
	void setDescription( const String & desc );

	//! \return Long description of the argument.
	const String & longDescription() const override;
	//! Set long description.
	void setLongDescription( const String & desc );

	//! \return Default value.
	virtual const String & defaultValue() const
	{
		return m_defaultValue;
	}

	//! Set default value.
	virtual void setDefaultValue( const String & v )
	{
		m_defaultValue = v;
	}

protected:
	/*!
		\return Argument for the given name.

		\retval Pointer to the ArgIface if this argument handles
			argument with the given name.
		\retval nullptr if this argument doesn't know about
			argument with name.
	*/
	ArgIface * findArgument(
		/*!
			Name of the argument. Can be for example "-t" or
			"--timeout".
		*/
		const String & name ) override;

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
		StringList & flags,
		//! All known names.
		StringList & names ) const override;

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override;

	//! \return Is given name a misspelled name of the argument.
	bool isMisspelledName(
		//! Name to check (misspelled).
		const String & name,
		//! List of possible names for the given misspelled name.
		StringList & possibleNames ) const override
	{
		if( !argumentName().empty() )
		{
			if( details::isMisspelledName( name,
				String( SL( "--" ) ) + argumentName() ) )
			{
				possibleNames.push_back( String( SL( "--" ) ) + argumentName() );

				return true;
			}
		}

		return false;
	}

private:
	DISABLE_COPY( Arg )

	//! Is this argument with value?
	bool m_isWithValue;
	//! Is this argument required?
	bool m_isRequired;
	//! Is this argument defined?
	bool m_isDefined;
	//! Value of this argument.
	String m_value;
	//! Flag of this argument.
	String m_flag;
	//! Name of this argument.
	String m_name;
	//! Value specifier.
	String m_valueSpecifier;
	//! Description.
	String m_description;
	//! Long description.
	String m_longDescription;
	//! Default value.
	String m_defaultValue;
}; // class Arg


//
// Arg
//

template< typename T >
Arg::Arg( Char flag, T && name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_isDefined( false )
	,	m_flag( 1, flag )
	,	m_name( std::forward< T > ( name ) )
	,	m_valueSpecifier( SL( "arg" ) )
{
}

inline
Arg::Arg( Char flag,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_isDefined( false )
	,	m_flag( 1, flag )
	,	m_valueSpecifier( SL( "arg" ) )
{
}

template< typename T >
Arg::Arg( T && name,
	bool isWithValue, bool isRequired )
	:	m_isWithValue( isWithValue )
	,	m_isRequired( isRequired )
	,	m_isDefined( false )
	,	m_name( std::forward< T > ( name ) )
	,	m_valueSpecifier( SL( "arg" ) )
{
}

inline
Arg::~Arg()
{
}

inline ArgIface *
Arg::findArgument( const String & name )
{
	if( details::isArgument( name ) && name.substr( 2 ) == m_name )
		return this;
	else if( details::isFlag( name ) && name.substr( 1 ) == m_flag )
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
			setValue( eatOneValue( context,
				String( SL( "Argument \"" ) ) +	name() +
					SL( "\" requires value that wasn't presented." ),
				cmdLine() ) );

			setDefined( true );
		}
	}
	else
		throw BaseException( String( SL( "Argument \"" ) ) +
			name() + SL( "\" already defined." ) );
}

inline String
Arg::name() const
{
	if( !m_name.empty() )
		return SL( "--" ) + m_name;
	else
		return SL( "-" ) + m_flag;
}

inline void
Arg::checkCorrectnessBeforeParsing( StringList & flags,
	StringList & names ) const
{
	if( !m_flag.empty() )
	{
		if( details::isCorrectFlag( m_flag ) )
		{
			const String flag = String( SL( "-" ) ) + m_flag;

			auto it = std::find( flags.begin(), flags.end(), flag );

			if( it != flags.end() )
				throw BaseException( String( SL( "Redefinition of argument "
					"witg flag \"" ) ) + flag + SL( "\"." ) );
			else
				flags.push_back( flag );
		}
		else
			throw BaseException( String( SL( "Disallowed flag \"-" ) ) +
				m_flag + SL( "\"." ) );
	}

	if( !m_name.empty() )
	{
		if( details::isCorrectName( m_name ) )
		{
			const String name = String( SL( "--" ) ) + m_name;

			auto it = std::find( names.begin(), names.end(), name );

			if( it != names.end() )
				throw BaseException( String( SL( "Redefinition of argument "
					"with name \"" ) ) + name + SL( "\"." ) );
			else
				names.push_back( name );
		}
		else
			throw BaseException( String( SL( "Disallowed name \"--" ) ) +
				m_name + SL( "\"." ) );
	}

	if( m_flag.empty() && m_name.empty() )
		throw BaseException( String( SL( "Arguments with empty flag and name "
			"are disallowed." ) ) );
}

inline void
Arg::checkCorrectnessAfterParsing() const
{
	if( isRequired() && !isDefined() )
		throw BaseException( String( SL( "Undefined required argument \"" ) ) +
			name() + SL( "\"." ) );
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

inline const String &
Arg::value() const
{
	if( !m_value.empty() )
		return m_value;
	else
		return m_defaultValue;
}

inline void
Arg::setValue( const String & v )
{
	m_value = v;
}

inline const String &
Arg::flag() const
{
	return m_flag;
}

inline void
Arg::setFlag( Char f )
{
	m_flag = String( 1, f );
}

inline const String &
Arg::argumentName() const
{
	return m_name;
}

inline void
Arg::setArgumentName( const String & name )
{
	m_name = name;
}

inline const String &
Arg::valueSpecifier() const
{
	return m_valueSpecifier;
}

inline void
Arg::setValueSpecifier( const String & vs )
{
	m_valueSpecifier = vs;
}

inline const String &
Arg::description() const
{
	return m_description;
}

inline void
Arg::setDescription( const String & desc )
{
	m_description = desc;
}

inline const String &
Arg::longDescription() const
{
	if( !m_longDescription.empty() )
		return m_longDescription;
	else
		return m_description;
}

inline void
Arg::setLongDescription( const String & desc )
{
	m_longDescription = desc;
}

} /* namespace Args */

#endif // ARGS__ARG_HPP__INCLUDED

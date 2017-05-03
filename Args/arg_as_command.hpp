
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

#ifndef ARGS__ARG_AS_COMMAND_HPP__INCLUDED
#define ARGS__ARG_AS_COMMAND_HPP__INCLUDED

// Args include.
#include "arg_iface.hpp"
#include "utils.hpp"
#include "exceptions.hpp"
#include "cmd_line.hpp"
#include "enums.hpp"
#include "types.hpp"


namespace Args {

//
// ArgAsCommand
//

/*!
	This is argument with name without dashes. It can has no value,
	one value or several values.
*/
class ArgAsCommand
	:	public ArgIface
{
public:
	template< typename T >
	explicit ArgAsCommand( T && name,
		bool required = false,
		ValueOptions opt = ValueOptions::NoValue )
		:	m_name( std::forward< T > ( name ) )
		,	m_opt( opt )
		,	m_required( required )
		,	m_defined( false )
	{
		if( isArgument( name ) || isFlag( name ) )
			throw BaseException( String( SL( "ArgAsCommand's name can't "
				"start with \"-\" whereas you are trying to set name to \"" ) ) +
				name + SL( "\"." ) );

		switch( m_opt )
		{
			case ValueOptions::OneValue :
			{
				m_valueSpecifier = SL( "arg" );
			}
				break;

			case ValueOptions::ManyValues :
			{
				m_valueSpecifier = SL( "args" );
			}
				break;

			default :
				break;
		}
	}

	~ArgAsCommand()
	{
	}

	/*!
		\return Name of the argument.

		If name is empty returned value should be a flag.
		I.e. for example "-t" or "--timeout"
	*/
	String name() const override
	{
		return m_name;
	}

	bool isWithValue() const override
	{
		switch( m_opt )
		{
			case ValueOptions::OneValue :
			case ValueOptions::ManyValues :
				return true;

			default :
				return false;
		}
	}

	bool isRequired() const override
	{
		return m_required;
	}

	//! \return Is this argument defined?
	bool isDefined() const override
	{
		return m_defined;
	}

	//! \return Flag.
	const String & flag() const override
	{
		return m_emptyString;
	}

	const String & argumentName() const override
	{
		return m_emptyString;
	}

	//! \return Value specifier.
	const String & valueSpecifier() const override
	{
		return m_valueSpecifier;
	}

	//! Set value specifier.
	void setValueSpecifier( const String & s )
	{
		m_valueSpecifier = s;
	}

	//! \return Description of the argument.
	const String & description() const override
	{
		return m_desc;
	}

	//! Set description.
	void setDescription( const String & desc )
	{
		m_desc = desc;
	}

	//! \return Long description of the argument.
	const String & longDescription() const override
	{
		if( !m_longDesc.empty() )
			return m_longDesc;
		else
			return m_desc;
	}

	//! Set long description.
	void setLongDescription( const String & desc )
	{
		m_longDesc = desc;
	}

	//! \return First value of this argument.
	const String & value() const
	{
		if( !m_values.empty() )
			return m_values.front();
		else
			return m_emptyString;
	}

	//! \return All values for this argument.
	const StringList & values() const
	{
		return m_values;
	}

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
		const String & name ) override
	{
		if( m_name == name )
			return this;
		else
			return nullptr;
	}

	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	void process(
		//! Context of the command line.
		Context & context ) override
	{
		if( !isDefined() )
		{
			m_defined = true;

			switch( m_opt )
			{
				case ValueOptions::ManyValues :
				{
					eatValues( context, m_values,
						String( SL( "Argument \"" ) ) +
							m_name + SL( "\" require value that wasn't presented." ),
						cmdLine() );
				}
					break;

				case ValueOptions::OneValue :
				{
					try {
						m_values.push_back( eatOneValue( context, cmdLine() ) );
					}
					catch( const BaseException & )
					{
						throw BaseException( String( SL( "Argument \"" ) ) +
							m_name + SL( "\" require value that wasn't presented." ) );
					}
				}
					break;

				default :
					break;
			}
		}
		else
			throw BaseException( String( SL( "Argument \"" ) ) + m_name +
				SL( "\" already defined." ) );
	}

	/*!
		Check correctness of the argument before parsing.

		Implementation of this method must add his flag
		and name to the flags and names.
	*/
	void checkCorrectnessBeforeParsing(
		//! All known flags.
		StringList & flags,
		//! All known names.
		StringList & names ) const override
	{
		UNUSED( flags )

		if( isCorrectName( m_name ) )
		{
			auto it = std::find( names.begin(), names.end(), m_name );

			if( it != names.end() )
				throw BaseException( String( SL( "Redefinition of argument "
					"with name \"" ) ) + m_name + SL( "\"." ) );
			else
				names.push_back( m_name );
		}
		else
			throw BaseException( String( SL( "Dissallowed name \"" ) ) +
				m_name + SL( "\" for the argument." ) );
	}

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override
	{
		if( isRequired() && !isDefined() )
			throw BaseException( String( SL( "Undefined required argument \"" ) ) +
				m_name + SL( "\"." ) );
	}

private:
	DISABLE_COPY( ArgAsCommand )

	//! Dummy empty string.
	static const String m_emptyString;

	//! Name.
	String m_name;
	//! Option.
	ValueOptions m_opt;
	//! Is required?
	bool m_required;
	//! Is defined?
	bool m_defined;
	//! Value specifier.
	String m_valueSpecifier;
	//! Description.
	String m_desc;
	//! Long description.
	String m_longDesc;
	//! Values.
	StringList m_values;
}; // class ArgAsCommand

const String ArgAsCommand::m_emptyString;

} /* namespace Args */

#endif // ARGS__ARG_AS_COMMAND_HPP__INCLUDED

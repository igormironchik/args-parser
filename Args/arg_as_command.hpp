
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

// C++ include.
#include <list>


namespace Args {

//
// ArgAsCommand
//

//! This is argument with name without dashes. It can has no value,
//! one value or several values.
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
			throw BaseException( std::string( "ArgAsCommand's name can't "
				"start with \"-\" whereas you are trying to set name to \"" ) +
				name + "\"." );

		switch( m_opt )
		{
			case ValueOptions::OneValue :
			{
				m_valueSpecifier = "arg";
			}
				break;

			case ValueOptions::ManyValues :
			{
				m_valueSpecifier = "args";
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
	std::string name() const override
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
	const std::string & flag() const override
	{
		return m_emptyString;
	}

	const std::string & argumentName() const override
	{
		return m_emptyString;
	}

	//! \return Value specifier.
	const std::string & valueSpecifier() const override
	{
		return m_valueSpecifier;
	}

	//! Set value specifier.
	void setValueSpecifier( const std::string & s )
	{
		m_valueSpecifier = s;
	}

	//! \return Description of the argument.
	const std::string & description() const override
	{
		return m_desc;
	}

	//! Set description.
	void setDescription( const std::string & desc )
	{
		m_desc = desc;
	}

	//! \return Long description of the argument.
	const std::string & longDescription() const override
	{
		if( !m_longDesc.empty() )
			return m_longDesc;
		else
			return m_desc;
	}

	//! Set long description.
	void setLongDescription( const std::string & desc )
	{
		m_longDesc = desc;
	}

	//! \return First value of this argument.
	const std::string & value() const
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
		const std::string & name ) override
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
						std::string( "Argument \"" ) +
							m_name + "\" require value that wasn't presented.",
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
						throw BaseException( std::string( "Argument \"" ) +
							m_name + "\" require value that wasn't presented." );
					}
				}
					break;

				default :
					break;
			}
		}
		else
			throw BaseException( std::string( "Argument \"" ) + m_name +
				"\" already defined." );
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
				throw BaseException( std::string( "Redefinition of argument "
					"with name \"" ) + m_name + "\"." );
			else
				names.push_back( m_name );
		}
		else
			throw BaseException( std::string( "Dissallowed name \"" ) +
				m_name + "\" for the argument." );
	}

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override
	{
		if( isRequired() && !isDefined() )
			throw BaseException( std::string( "Undefined required argument \"" ) +
				m_name + "\"." );
	}

private:
	DISABLE_COPY( ArgAsCommand )

	//! Dummy empty string.
	static const std::string m_emptyString;

	//! Name.
	std::string m_name;
	//! Option.
	ValueOptions m_opt;
	//! Is required?
	bool m_required;
	//! Is defined?
	bool m_defined;
	//! Value specifier.
	std::string m_valueSpecifier;
	//! Description.
	std::string m_desc;
	//! Long description.
	std::string m_longDesc;
	//! Values.
	StringList m_values;
}; // class ArgAsCommand

const std::string ArgAsCommand::m_emptyString;

} /* namespace Args */

#endif // ARGS__ARG_AS_COMMAND_HPP__INCLUDED

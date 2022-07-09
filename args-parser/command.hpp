
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013-2020 Igor Mironchik

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

#ifndef ARGS__COMMAND_HPP__INCLUDED
#define ARGS__COMMAND_HPP__INCLUDED

// Args include.
#include "api.hpp"
#include "group_iface.hpp"
#include "utils.hpp"
#include "context.hpp"
#include "value_utils.hpp"
#include "enums.hpp"
#include "types.hpp"


namespace Args {

//
// Command
//

//! Command in the command line interface.
class Command
	:	public GroupIface
{
	friend class CmdLine;
	friend class HelpPrinter;
	friend class Help;

public:
	template< typename T >
	explicit Command( T && nm,
		ValueOptions opt = ValueOptions::NoValue,
		bool isSubCommandRequired = false )
		:	GroupIface( std::forward< T > ( nm ) )
		,	m_opt( opt )
		,	m_isDefined( false )
		,	m_isSubCommandRequired( isSubCommandRequired )
		,	m_subCommand( nullptr )
	{
		if( details::isArgument( name() ) || details::isFlag( name() ) )
			throw BaseException( String( SL( "Command's name can't "
				"start with \"-\" whereas you are trying to set name to \"" ) ) +
				name() + SL( "\"." ) );

		if( name().empty() )
			throw BaseException(
				String( SL( "Command can't be with empty name." ) ) );

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

	virtual ~Command()
	{
	}

	//! \return Type of the argument.
	ArgType type() const override
	{
		return ArgType::Command;
	}

	//! \return Is this command defined?
	bool isDefined() const override
	{
		return m_isDefined;
	}

	//! \return Is this argument with value?
	bool isWithValue() const override
	{
		return ( m_opt == ValueOptions::OneValue ||
			m_opt == ValueOptions::ManyValues );
	}

	//! Set required flag.
	void setRequired( bool on = true ) override
	{
		UNUSED( on )
	}

	//! \return Value specifier.
	const String & valueSpecifier() const override
	{
		return m_valueSpecifier;
	}
	//! Set value specifier.
	void setValueSpecifier( const String & vs )
	{
		m_valueSpecifier = vs;
	}

	//! \return Description of the argument.
	const String & description() const override
	{
		return m_description;
	}
	//! Set description.
	void setDescription( const String & desc )
	{
		m_description = desc;
	}

	//! \return Long description of the argument.
	const String & longDescription() const override
	{
		if( !m_longDescription.empty() )
			return m_longDescription;
		else
			return m_description;
	}
	//! Set long description.
	void setLongDescription( const String & desc )
	{
		m_longDescription = desc;
	}

	//! \return First value of this argument.
	const String & value() const
	{
		if( !m_values.empty() )
			return m_values.front();
		else if( !m_defaultValues.empty() )
			return m_defaultValues.front();
		else
			return details::DummyString<>::c_string;
	}

	//! \return All values for this argument.
	const StringList & values() const
	{
		if( !m_values.empty() )
			return m_values;
		else
			return m_defaultValues;
	}

	//! \return Default value.
	const String & defaultValue() const
	{
		if( !m_defaultValues.empty() )
			return m_defaultValues.front();
		else
			return details::DummyString<>::c_string;
	}

	//! Set default value. \note Value will be pushed back to the list
	//! of default values.
	void setDefaultValue( const String & v )
	{
		m_defaultValues.push_back( v );
	}

	//! \return Default values.
	const StringList & defaultValues() const
	{
		return m_defaultValues;
	}

	//! Set default values.
	void setDefaultValues( const StringList & v )
	{
		m_defaultValues = v;
	}

	//! \return Is given name a misspelled name of the argument.
	bool isMisspelledName(
		//! Name to check (misspelled).
		const String & nm,
		//! List of possible names for the given misspelled name.
		StringList & possibleNames ) const override
	{
		bool ret = false;

		if( details::isMisspelledName( nm, name() ) )
		{
			possibleNames.push_back( name() );

			ret = true;
		}

		if( GroupIface::isMisspelledName( nm, possibleNames ) )
			return true;
		else
			return ret;
	}

	//! \return Is given name a misspelled name of the command.
	bool isMisspelledCommand(
		//! Name to check (misspelled).
		const String & nm,
		//! List of possible names for the given misspelled name.
		StringList & possibleNames ) const
	{
		if( details::isMisspelledName( nm, name() ) )
		{
			possibleNames.push_back( name() );

			return true;
		}
		else
			return false;
	}

	//! Clear state of the argument.
	void clear() override
	{
		m_isDefined = false;

		m_values.clear();

		GroupIface::clear();
	}

	using GroupIface::addArg;

	//! Add argument.
	void addArg( ArgPtr arg ) override
	{
		if( arg->type() == ArgType::Command && m_opt != ValueOptions::NoValue )
			throw BaseException( String( SL( "Addition of commands to command with "
				"value is disallowed." ) ) );

		if( std::find( m_children.cbegin(), m_children.cend(), arg ) ==
			m_children.cend() )
		{
			if( cmdLine() )
				arg->setCmdLine( cmdLine() );

			m_children.push_back( std::move( arg ) );
		}
	}

protected:
	/*!
		\return Argument for the given name.

		\retval this if the given name is the name of the command.

		\note Doesn't look in the children.
	*/
	ArgIface * findArgument(
		/*!
			Name of the argument. Can be for example "-t" or
			"--timeout".
		*/
		const String & n ) override
	{
		if( name() == n )
			return this;
		else
			return nullptr;
	}

	/*!
		\return Argument for the given name.

		\retval Pointer to the ArgIface if this argument handles
			argument with the given name.
		\retval nullptr if this argument doesn't know about
			argument with name.

		\note Looks only in children.
	*/
	ArgIface * findChild(
		/*!
			Name of the argument. Can be for example "-t" or
			"--timeout".
		*/
		const String & name )
	{
		auto * arg = GroupIface::findArgument( name );

		if( !arg && m_subCommand )
			return m_subCommand->findChild( name );

		return arg;
	}

	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	void process(
		//! Context of the command line.
		Context & ctx ) override
	{
		m_isDefined = true;

		switch( m_opt )
		{
			case ValueOptions::ManyValues :
			{
				eatValues( ctx, m_values,
					String( SL( "Command \"" ) ) +
						name() + SL( "\" requires value that wasn't presented." ),
					cmdLine() );
			}
				break;

			case ValueOptions::OneValue :
			{
				m_values.push_back( eatOneValue( ctx,
					String( SL( "Command \"" ) ) + name() +
						SL( "\" requires value that wasn't presented." ),
					cmdLine() ) );
			}
				break;

			default :
				break;
		}
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
		if( details::isCorrectName( name() ) )
		{
			auto it = std::find( names.begin(), names.end(), name() );

			if( it != names.end() )
				throw BaseException( String( SL( "Redefinition of command "
					"with name \"" ) ) + name() + SL( "\"." ) );
			else
				names.push_back( name() );
		}
		else
			throw BaseException( String( SL( "Disallowed name \"" ) ) +
				name() + SL( "\" for the command." ) );

		StringList ftmp = flags;
		StringList ntmp = names;

		GroupIface::checkCorrectnessBeforeParsing( ftmp, ntmp );
	}

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override
	{
		if( isDefined() )
			GroupIface::checkCorrectnessAfterParsing();

		if( isDefined() && m_isSubCommandRequired && !m_subCommand )
			throw BaseException( String( SL( "Wasn't defined required sub-command of command \"" ) ) +
				name() + SL( "\"." ) );
	}

	//! Set current subcommand.
	void setCurrentSubCommand( Command * sub )
	{
		m_subCommand = sub;
	}

private:
	DISABLE_COPY( Command )

	//! Option.
	ValueOptions m_opt;
	//! Value specifier.
	String m_valueSpecifier;
	//! Description.
	String m_description;
	//! Long description.
	String m_longDescription;
	//! Is defined.
	bool m_isDefined;
	//! Is sub-command required?
	bool m_isSubCommandRequired;
	//! Values.
	StringList m_values;
	//! Default values.
	StringList m_defaultValues;
	//! Current sub-command.
	Command * m_subCommand;
}; // class Command

} /* namespace Args */

#endif // ARGS__COMMAND_HPP__INCLUDED


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

#ifndef ARGS__COMMAND_HPP__INCLUDED
#define ARGS__COMMAND_HPP__INCLUDED

// Args include.
#include <Args/group_iface.hpp>
#include <Args/utils.hpp>
#include <Args/context.hpp>
#include <Args/value_utils.hpp>

// C++ include.
#include <list>


namespace Args {

//
// Command
//

//! Command in the command line interface.
class Command final
	:	public GroupIface
{
	friend class CmdLine;
	friend class HelpPrinter;

public:
	//! Option to the command.
	enum CommandOption {
		//! Without value.
		CommandWithoutValue = 1,
		//! With single value.
		CommandWithSingleValue = 2,
		//! With many values.
		CommandWithManyValues = 3
	}; // enum CommandOption

	explicit Command( const std::string & name,
		CommandOption opt = CommandWithoutValue )
		:	GroupIface( name )
		,	m_opt( opt )
		,	m_isDefined( false )
	{
		switch( m_opt )
		{
			case CommandWithSingleValue :
			{
				m_valueSpecifier = "arg";
			}
				break;

			case CommandWithManyValues :
			{
				m_valueSpecifier = "args";
			}
				break;

			default :
				break;
		}
	}

	virtual ~Command()
	{
	}

	//! \return Is this command defined?
	bool isDefined() const override
	{
		return m_isDefined;
	}

	//! \return Is this argument with value?
	bool isWithValue() const override
	{
		return ( m_opt == CommandWithSingleValue ||
			m_opt == CommandWithManyValues );
	}

	//! Set required flag.
	void setRequired( bool on = true ) override
	{
		UNUSED( on )
	}

	//! \return Value specifier.
	const std::string & valueSpecifier() const override
	{
		return m_valueSpecifier;
	}
	//! Set value specifier.
	void setValueSpecifier( const std::string & vs )
	{
		m_valueSpecifier = vs;
	}

	//! \return Description of the argument.
	const std::string & description() const override
	{
		return m_description;
	}
	//! Set description.
	void setDescription( const std::string & desc )
	{
		m_description = desc;
	}

	//! \return Long description of the argument.
	const std::string & longDescription() const override
	{
		if( !m_longDescription.empty() )
			return m_longDescription;
		else
			return m_description;
	}
	//! Set long description.
	void setLongDescription( const std::string & desc )
	{
		m_longDescription = desc;
	}

	//! \return First value of this argument.
	const std::string & value() const
	{
		if( !m_values.empty() )
			return m_values.front();
		else
			return m_dummyEmptyString;
	}

	//! \return All values for this argument.
	const std::list< std::string > & values() const
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
		const std::string & n ) override
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
	*/
	ArgIface * isItYourChild(
		/*!
			Name of the argument. Can be for example "-t" or
			"--timeout".
		*/
		const std::string & name )
	{
		return GroupIface::isItYou( name );
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
			case CommandWithManyValues :
			{
				eatValues( ctx, m_values,
					std::string( "Command \"" ) +
						name() + "\" require value that wasn't presented." );
			}
				break;

			case CommandWithSingleValue :
			{
				if( !ctx.atEnd() )
				{
					auto val = ctx.next();

					if( !isArgument( *val ) && !isFlag( *val ) )
						m_values.push_back( *val );
					else
					{
						ctx.putBack();

						throw BaseException( std::string( "Command \"" ) +
							name() + "\" require value that wasn't presented." );
					}
				}
				else
					throw BaseException( std::string( "Command \"" ) +
						name() + "\" require value that wasn't presented." );
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
		std::list< std::string > & flags,
		//! All known names.
		std::list< std::string > & names ) const override
	{
		if( isCorrectName( name() ) )
		{
			auto it = std::find( names.begin(), names.end(), name() );

			if( it != names.end() )
				throw BaseException( std::string( "Redefinition of command "
					"with name \"" ) + name() + "\"." );
			else
				names.push_back( name() );
		}
		else
			throw BaseException( std::string( "Dissallowed name \"" ) +
				name() + "\" for the command." );

		std::list< std::string > ftmp = flags;
		std::list< std::string > ntmp = names;

		GroupIface::checkCorrectnessBeforeParsing( ftmp, ntmp );
	}

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override
	{
		if( isDefined() )
			GroupIface::checkCorrectnessAfterParsing();
	}

private:
	DISABLE_COPY( Command )

	//! Option.
	CommandOption m_opt;
	//! Value specifier.
	std::string m_valueSpecifier;
	//! Description.
	std::string m_description;
	//! Long description.
	std::string m_longDescription;
	//! Is defined.
	bool m_isDefined;
	//! Values.
	std::list< std::string > m_values;
}; // class Command

} /* namespace Args */

#endif // ARGS__COMMAND_HPP__INCLUDED

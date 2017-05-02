
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

/*!
	\file

	\warning If you are including this file you should include command.hpp too.
*/

#ifndef ARGS__GROUP_IFACE_HPP__INCLUDED
#define ARGS__GROUP_IFACE_HPP__INCLUDED

// Args include.
#include <Args/arg_iface.hpp>
#include <Args/utils.hpp>
#include <Args/exceptions.hpp>

// C++ include.
#include <list>
#include <type_traits>
#include <algorithm>
#include <utility>


namespace Args {

class Command;


//
// GroupIface
//

//! Interface for the groups.
class GroupIface
	:	public ArgIface
{
public:
	//! Dummy empty string.
	static const std::string m_dummyEmptyString;

public:
	//! List of child arguments.
	typedef std::list< ArgIface* > Arguments;

	template< typename T >
	explicit GroupIface( T && name,
		bool required = false )
		:	m_name( std::forward< T > ( name ) )
		,	m_required( required )
	{
	}

	virtual ~GroupIface()
	{
	}

	//! \return List of children.
	const Arguments & children() const
	{
		return m_children;
	}

	//! Add argument.
	template< typename T >
	typename std::enable_if< std::is_base_of< ArgIface, T >::value &&
		!std::is_base_of< Command, T >::value >::type
	addArg( T & arg )
	{
		if( dynamic_cast< Command* > ( &arg ) )
			throw BaseException( std::string( "Commands not allowed in groups. "
				"You are trying to add command \"" ) + arg.name() +
				"\" to group \"" + name() + "\"." );

		if( std::find( m_children.cbegin(), m_children.cend(), &arg ) ==
			m_children.cend() )
				m_children.push_back( &arg );
	}

	//! Add argument.
	template< typename T >
	typename std::enable_if< std::is_base_of< ArgIface, T >::value &&
		!std::is_base_of< Command, T >::value >::type
	addArg( T * arg )
	{
		if( dynamic_cast< Command* > ( arg ) )
			throw BaseException( std::string( "Commands not allowed in groups. "
				"You are trying to add command \"" ) + arg->name() +
				"\" to group \"" + name() + "\"." );

		if( std::find( m_children.cbegin(), m_children.cend(), arg ) ==
			m_children.cend() )
				m_children.push_back( arg );
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

	//! \return Is this argument with value?
	bool isWithValue() const override
	{
		return false;
	}

	//! \return Is this argument required?
	bool isRequired() const override
	{
		return m_required;
	}

	//! Set required flag.
	virtual void setRequired( bool on = true )
	{
		m_required = on;
	}

	//! \return Flag.
	const std::string & flag() const override
	{
		return m_dummyEmptyString;
	}

	//! \return Argument name.
	const std::string & argumentName() const override
	{
		return m_dummyEmptyString;
	}

	//! \return Value specifier.
	const std::string & valueSpecifier() const override
	{
		return m_dummyEmptyString;
	}

	//! \return Description of the argument.
	const std::string & description() const override
	{
		return m_dummyEmptyString;
	}

	//! \return Long description of the argument.
	const std::string & longDescription() const override
	{
		return m_dummyEmptyString;
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
		for( auto & arg : m_children )
		{
			ArgIface * tmp = arg->isItYou( name );

			if( tmp != nullptr )
				return tmp;
		}

		return nullptr;
	}

	/*!
		Process argument's staff, for example take values from
		context. This method invokes exactly at that moment when
		parser has found this argument.
	*/
	void process(
		//! Context of the command line.
		Context & ) override
	{
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
		for( const auto & arg : m_children )
			arg->checkCorrectnessBeforeParsing( flags, names );
	}

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override
	{
		for( const auto & arg : m_children )
			arg->checkCorrectnessAfterParsing();

		if( isRequired() && !isDefined() )
			throw BaseException(
				std::string( "Not defined required argument \"" ) +
				name() + "\"" );
	}

private:
	DISABLE_COPY( GroupIface )

	//! List of children.
	Arguments m_children;
	//! Name.
	std::string m_name;
	//! Is required?
	bool m_required;
}; // class GroupIface

const std::string GroupIface::m_dummyEmptyString;

} /* namespace Args */

#endif // ARGS__GROUP_IFACE_HPP__INCLUDED


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

#ifndef ARGS__API_HPP__INCLUDED
#define ARGS__API_HPP__INCLUDED

// Args include.
#include "types.hpp"
#include "arg.hpp"
#include "multi_arg.hpp"
#include "groups.hpp"
#include "arg_as_command.hpp"


namespace Args {

namespace details {

//
// API
//

//! Auxiliary API.
template< typename PARENT, typename SELF, typename ARGPTR >
class API {
	friend class CmdLine;

protected:
	API( PARENT & parent, SELF & self )
		:	m_parent( parent )
		,	m_self( self )
	{
	}

public:
	virtual ~API()
	{
	}

	//! Add OnlyOneGroup to this group.
	template< typename NAME >
	API< API< PARENT, SELF, ARGPTR >, OnlyOneGroup, ARGPTR > addOnlyOneGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = ARGPTR(
			new OnlyOneGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		m_self.addArg( group );

		return API< API< PARENT, SELF, ARGPTR >, OnlyOneGroup, ARGPTR >
			( *this, *group );
	}

	//! Add AllOfGroup to this group.
	template< typename NAME >
	API< API< PARENT, SELF, ARGPTR >, AllOfGroup, ARGPTR > addAllOfGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = ARGPTR(
			new AllOfGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		m_self.addArg( group );

		return API< API< PARENT, SELF, ARGPTR >, AllOfGroup, ARGPTR >
			( *this, *group );
	}

	//! Add AtLeastOneGroup to this group.
	template< typename NAME >
	API< API< PARENT, SELF, ARGPTR >, AtLeastOneGroup, ARGPTR > addAtLeastOneGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = ARGPTR(
			new AtLeastOneGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		m_self.addArg( group );

		return API< API< PARENT, SELF, ARGPTR >, AtLeastOneGroup, ARGPTR >
			( *this, *group );
	}

	//! Add argument.
	template< typename NAME, typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addArg(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{

		auto arg = ARGPTR(
			new Arg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( std::forward< DESC > ( desc ) );

		if( !longDesc.empty() )
			arg->setLongDescription( std::forward< LDESC > ( longDesc ) );

		if( !defaultValue.empty() )
			arg->setDefaultValue( std::forward< DEFVAL > ( defaultValue ) );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( std::forward< VS > ( valueSpecifier ) );

		m_self.addArg( arg );

		return *this;
	}

	//! Add argument with flag only.
	template< typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addArgWithFlagOnly(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{

		auto arg = ARGPTR(
			new Arg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( std::forward< DESC > ( desc ) );

		if( !longDesc.empty() )
			arg->setLongDescription( std::forward< LDESC > ( longDesc ) );

		if( !defaultValue.empty() )
			arg->setDefaultValue( std::forward< DEFVAL > ( defaultValue ) );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( std::forward< VS > ( valueSpecifier ) );

		m_self.addArg( arg );

		return *this;
	}

	//! Add argument with name only.
	template< typename NAME, typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addArgWithNameOnly(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{

		auto arg = ARGPTR(
			new Arg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( std::forward< DESC > ( desc ) );

		if( !longDesc.empty() )
			arg->setLongDescription( std::forward< LDESC > ( longDesc ) );

		if( !defaultValue.empty() )
			arg->setDefaultValue( std::forward< DEFVAL > ( defaultValue ) );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( std::forward< VS > ( valueSpecifier ) );

		m_self.addArg( arg );

		return *this;
	}

	//! Add multi argument.
	template< typename NAME, typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addMultiArg(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{

		auto arg = ARGPTR(
			new MultiArg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( std::forward< DESC > ( desc ) );

		if( !longDesc.empty() )
			arg->setLongDescription( std::forward< LDESC > ( longDesc ) );

		if( !defaultValue.empty() )
			arg->setDefaultValue( std::forward< DEFVAL > ( defaultValue ) );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( std::forward< VS > ( valueSpecifier ) );

		m_self.addArg( arg );

		return *this;
	}

	//! Add multi argument with flag only.
	template< typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addMultiArgWithFlagOnly(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{

		auto arg = ARGPTR(
			new MultiArg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( std::forward< DESC > ( desc ) );

		if( !longDesc.empty() )
			arg->setLongDescription( std::forward< LDESC > ( longDesc ) );

		if( !defaultValue.empty() )
			arg->setDefaultValue( std::forward< DEFVAL > ( defaultValue ) );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( std::forward< VS > ( valueSpecifier ) );

		m_self.addArg( arg );

		return *this;
	}

	//! Add multi argument with name only.
	template< typename NAME, typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addMultiArgWithNameOnly(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{

		auto arg = ARGPTR(
			new MultiArg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( std::forward< DESC > ( desc ) );

		if( !longDesc.empty() )
			arg->setLongDescription( std::forward< LDESC > ( longDesc ) );

		if( !defaultValue.empty() )
			arg->setDefaultValue( std::forward< DEFVAL > ( defaultValue ) );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( std::forward< VS > ( valueSpecifier ) );

		m_self.addArg( arg );

		return *this;
	}

	//! Add argument as command.
	template< typename NAME, typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addArgAsCommand(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{
		auto arg = ARGPTR(
			new ArgAsCommand( std::forward< NAME > ( name ), required,
				opt ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( std::forward< DESC > ( desc ) );

		if( !longDesc.empty() )
			arg->setLongDescription( std::forward< LDESC > ( longDesc ) );

		if( !defaultValue.empty() )
			arg->setDefaultValue( std::forward< DEFVAL > ( defaultValue ) );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( std::forward< VS > ( valueSpecifier ) );

		m_self.addArg( arg );

		return *this;
	}

	//! Add sub command.
	template< typename NAME, typename DESC = String, typename LDESC = String,
		typename DEFVAL = String, typename VS = String >
	API< PARENT, SELF, ARGPTR > & addSubCommand(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		DESC && desc = String(),
		//! Long description.
		LDESC && longDesc = String(),
		//! Default value.
		DEFVAL && defaultValue = String(),
		//! Value specifier.
		VS && valueSpecifier = String() )
	{
		return addArgAsCommand( std::forward< NAME > ( name ),
			required, opt, std::forward< DESC > ( desc ),
			std::forward< LDESC > ( longDesc ),
			std::forward< DEFVAL > ( defaultValue ),
			std::forward< VS > ( valueSpecifier ) );
	}

	//! End this group. \return Parent object.
	PARENT & end() const
	{
		return m_parent;
	}

private:
	//! Parent.
	PARENT & m_parent;
	//! Self object.
	SELF & m_self;
}; // class API

} /* namespace details */

} /* namespace Args */

#endif // ARGS__API_HPP__INCLUDED

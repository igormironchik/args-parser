
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
template< typename PARENT, typename SELF, typename ARGPTR,
	bool RETPARENT = false >
class API {
public:
	API( PARENT & parent, SELF & self )
		:	m_parent( parent )
		,	m_self( self )
	{
	}

	virtual ~API()
	{
	}

	//! Add OnlyOneGroup.
	template< typename NAME >
	API< API< PARENT, SELF, ARGPTR >, OnlyOneGroup, ARGPTR > addOnlyOneGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = std::unique_ptr< OnlyOneGroup, details::Deleter< ArgIface > > (
			new OnlyOneGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		OnlyOneGroup & g = *group;

		ARGPTR a = std::move( group );

		m_self.addArg( std::move( a ) );

		return API< API< PARENT, SELF, ARGPTR >, OnlyOneGroup, ARGPTR >
			( *this, g );
	}

	//! Add AllOfGroup.
	template< typename NAME >
	API< API< PARENT, SELF, ARGPTR >, AllOfGroup, ARGPTR > addAllOfGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = std::unique_ptr< AllOfGroup, details::Deleter< ArgIface > > (
			new AllOfGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		AllOfGroup & g = *group;

		ARGPTR a = std::move( group );

		m_self.addArg( std::move( a ) );

		return API< API< PARENT, SELF, ARGPTR >, AllOfGroup, ARGPTR >
			( *this, g );
	}

	//! Add AtLeastOneGroup.
	template< typename NAME >
	API< API< PARENT, SELF, ARGPTR >, AtLeastOneGroup, ARGPTR > addAtLeastOneGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = std::unique_ptr< AtLeastOneGroup, details::Deleter< ArgIface > > (
			new AtLeastOneGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		AtLeastOneGroup & g = *group;

		ARGPTR a = std::move( group );

		m_self.addArg( std::move( a ) );

		return API< API< PARENT, SELF, ARGPTR >, AtLeastOneGroup, ARGPTR >
			( *this, g );
	}

	//! Add argument.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addArgWithFlagAndName(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< Arg, details::Deleter< ArgIface > > (
			new Arg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add argument with flag only.
	API< PARENT, SELF, ARGPTR, RETPARENT > & addArgWithFlagOnly(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< Arg, details::Deleter< ArgIface > > (
			new Arg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add argument with name only.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addArgWithNameOnly(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< Arg, details::Deleter< ArgIface > > (
			new Arg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add multi argument.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addMultiArg(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add multi argument with flag only.
	API< PARENT, SELF, ARGPTR, RETPARENT > & addMultiArgWithFlagOnly(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add multi argument with name only.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addMultiArgWithNameOnly(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add multi argument.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addMultiArgWithDefaulValues(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add multi argument with flag only.
	API< PARENT, SELF, ARGPTR, RETPARENT > & addMultiArgWithFlagOnlyAndDefaultValues(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add multi argument with name only.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addMultiArgWithNameOnlyAndDefaultValues(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add argument as command.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addArgAsCommand(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		auto arg = std::unique_ptr< ArgAsCommand, details::Deleter< ArgIface > > (
			new ArgAsCommand( std::forward< NAME > ( name ), required,
				opt ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add argument as command.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addArgAsCommandWithDefaulValues(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		auto arg = std::unique_ptr< ArgAsCommand, details::Deleter< ArgIface > > (
			new ArgAsCommand( std::forward< NAME > ( name ), required,
				opt ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return *this;
	}

	//! Add sub command.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addSubCommand(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		return addArgAsCommand( std::forward< NAME > ( name ),
			required, opt, desc, longDesc,
			defaultValue, valueSpecifier );
	}

	//! Add sub command.
	template< typename NAME >
	API< PARENT, SELF, ARGPTR, RETPARENT > & addSubCommandWithDefaultValues(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		return addArgAsCommandWithDefaulValues( std::forward< NAME > ( name ),
			required, opt, desc, longDesc,
			defaultValues, valueSpecifier );
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


//! Auxiliary API.
template< typename PARENT, typename SELF, typename ARGPTR >
class API< PARENT, SELF, ARGPTR, true > {
public:
	API( PARENT & parent, SELF & self )
		:	m_parent( parent )
		,	m_self( self )
	{
	}

	virtual ~API()
	{
	}

	//! Add OnlyOneGroup.
	template< typename NAME >
	API< PARENT, OnlyOneGroup, ARGPTR > addOnlyOneGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = std::unique_ptr< OnlyOneGroup, details::Deleter< ArgIface > > (
			new OnlyOneGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		OnlyOneGroup & g = *group;

		ARGPTR a = std::move( group );

		m_self.addArg( std::move( a ) );

		return API< PARENT, OnlyOneGroup, ARGPTR >
			( *( static_cast< PARENT* > ( this ) ), g );
	}

	//! Add AllOfGroup.
	template< typename NAME >
	API< PARENT, AllOfGroup, ARGPTR > addAllOfGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = std::unique_ptr< AllOfGroup, details::Deleter< ArgIface > > (
			new AllOfGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		AllOfGroup & g = *group;

		ARGPTR a = std::move( group );

		m_self.addArg( std::move( a ) );

		return API< PARENT, AllOfGroup, ARGPTR >
			( *( static_cast< PARENT* > ( this ) ), g );
	}

	//! Add AtLeastOneGroup.
	template< typename NAME >
	API< PARENT, AtLeastOneGroup, ARGPTR > addAtLeastOneGroup(
		//! Name of the group.
		NAME && name,
		//! Is group required?
		bool required = false )
	{
		auto group = std::unique_ptr< AtLeastOneGroup, details::Deleter< ArgIface > > (
			new AtLeastOneGroup( std::forward< NAME > ( name ), required ),
			details::Deleter< ArgIface > ( true ) );

		AtLeastOneGroup & g = *group;

		ARGPTR a = std::move( group );

		m_self.addArg( std::move( a ) );

		return API< PARENT, AtLeastOneGroup, ARGPTR >
			( *( static_cast< PARENT* > ( this ) ), g );
	}

	//! Add argument.
	template< typename NAME >
	PARENT & addArgWithFlagAndName(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< Arg, details::Deleter< ArgIface > > (
			new Arg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add argument with flag only.
	PARENT & addArgWithFlagOnly(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< Arg, details::Deleter< ArgIface > > (
			new Arg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add argument with name only.
	template< typename NAME >
	PARENT & addArgWithNameOnly(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< Arg, details::Deleter< ArgIface > > (
			new Arg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add multi argument.
	template< typename NAME >
	PARENT & addMultiArg(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add multi argument with flag only.
	PARENT & addMultiArgWithFlagOnly(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add multi argument with name only.
	template< typename NAME >
	PARENT & addMultiArgWithNameOnly(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add multi argument.
	template< typename NAME >
	PARENT & addMultiArgWithDefaulValues(
		//! Flag for this argument.
		Char flag,
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add multi argument with flag only.
	PARENT & addMultiArgWithFlagOnlyAndDefaultValues(
		//! Flag for this argument.
		Char flag,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( flag, isWithValue, isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add multi argument with name only.
	template< typename NAME >
	PARENT & addMultiArgWithNameOnlyAndDefaultValues(
		//! Name for this argument.
		NAME && name,
		//! Is this argument with value?
		bool isWithValue = false,
		//! Is this argument required?
		bool isRequired = false,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{

		auto arg = std::unique_ptr< MultiArg, details::Deleter< ArgIface > > (
			new MultiArg( std::forward< NAME > ( name ), isWithValue,
				isRequired ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add argument as command.
	template< typename NAME >
	PARENT & addArgAsCommand(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		auto arg = std::unique_ptr< ArgAsCommand, details::Deleter< ArgIface > > (
			new ArgAsCommand( std::forward< NAME > ( name ), required,
				opt ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValue.empty() )
			arg->setDefaultValue( defaultValue );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add argument as command.
	template< typename NAME >
	PARENT & addArgAsCommandWithDefaulValues(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		auto arg = std::unique_ptr< ArgAsCommand, details::Deleter< ArgIface > > (
			new ArgAsCommand( std::forward< NAME > ( name ), required,
				opt ),
			details::Deleter< ArgIface > ( true ) );

		if( !desc.empty() )
			arg->setDescription( desc );

		if( !longDesc.empty() )
			arg->setLongDescription( longDesc );

		if( !defaultValues.empty() )
			arg->setDefaultValues( defaultValues );

		if( !valueSpecifier.empty() )
			arg->setValueSpecifier( valueSpecifier );

		ARGPTR a = std::move( arg );

		m_self.addArg( std::move( a ) );

		return m_parent;
	}

	//! Add sub command.
	template< typename NAME >
	PARENT & addSubCommand(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const String & defaultValue = String(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		return addArgAsCommand( std::forward< NAME > ( name ),
			required, opt, desc, longDesc,
			defaultValue, valueSpecifier );
	}

	//! Add sub command.
	template< typename NAME >
	PARENT & addSubCommandWithDefaultValues(
		//! Name.
		NAME && name,
		//! Is required?
		bool required = false,
		//! Value type.
		ValueOptions opt = ValueOptions::NoValue,
		//! Description of the argument.
		const String & desc = String(),
		//! Long description.
		const String & longDesc = String(),
		//! Default value.
		const StringList & defaultValues = StringList(),
		//! Value specifier.
		const String & valueSpecifier = String() )
	{
		return addArgAsCommandWithDefaulValues( std::forward< NAME > ( name ),
			required, opt, desc, longDesc,
			defaultValues, valueSpecifier );
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

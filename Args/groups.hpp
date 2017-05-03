
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

#ifndef ARGS__GROUPS_HPP__INCLUDED
#define ARGS__GROUPS_HPP__INCLUDED

// Args include.
#include "group_iface.hpp"

// C++ include.
#include <algorithm>


namespace Args {

//
// OnlyOneGroup
//

//! Group of args where only one argument can be defined.
class OnlyOneGroup final
	:	public GroupIface
{
public:
	template< typename T >
	explicit OnlyOneGroup( T && name,
		bool required = false )
		:	GroupIface( std::forward< T > ( name ), required )
	{
	}

	virtual ~OnlyOneGroup()
	{
	}

	//! \return Is this argument defined?
	bool isDefined() const override
	{
		for( const auto & arg : children() )
		{
			if( arg->isDefined() )
				return true;
		}

		return false;
	}

protected:
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
		GroupIface::checkCorrectnessBeforeParsing( flags, names );

		for( const auto & arg : children() )
		{
			if( arg->isRequired() )
				throw BaseException( std::string( "Required argument \"" ) +
					arg->name() +
					"\" is not allowed to " +
					"be in OnlyOne group \"" + name() + "\"." );
		}
	}

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override
	{
		GroupIface::checkCorrectnessAfterParsing();

		ArgIface * defined = nullptr;

		for( const auto & arg : children() )
		{
			if( arg->isDefined() )
			{
				if( defined )
					throw BaseException( std::string( "Only one argument can "
						"be defined in OnlyOne group \"" ) + name() + "\". " +
						"Whereas defined \"" + defined->name() +
						"\" and \"" + arg->name() + "\"." );
				else
					defined = arg;
			}
		}
	}
}; // class OnlyOneGroup


//
// AllOfGroup
//

//! Group of args where all arguments should be defined.
class AllOfGroup final
	:	public GroupIface
{
public:
	template< typename T >
	explicit AllOfGroup( T && name,
		bool required = false )
		:	GroupIface( std::forward< T > ( name ), required )
	{
	}

	virtual ~AllOfGroup()
	{
	}

	//! \return Is this argument defined?
	bool isDefined() const override
	{
		for( const auto & arg : children() )
		{
			if( !arg->isDefined() )
				return false;
		}

		return true;
	}

protected:
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
		GroupIface::checkCorrectnessBeforeParsing( flags, names );

		for( const auto & arg : children() )
		{
			if( arg->isRequired() )
				throw BaseException( std::string( "Required argument \"" ) +
					arg->name() +
					"\" is not allowed to " +
					"be in AllOf group \"" + name() + "\"." );
		}
	}

	//! Check correctness of the argument after parsing.
	void checkCorrectnessAfterParsing() const override
	{
		GroupIface::checkCorrectnessAfterParsing();

		bool defined = false;

		const bool all = std::all_of( children().cbegin(), children().cend(),
			[ & ] ( const auto & arg )
			{
				if( arg->isDefined() )
					defined = true;

				return arg->isDefined();
			} );

		if( defined && !all )
			throw BaseException( std::string( "All arguments in "
				"AllOf group \"" ) + name() + "\" should be defined." );
	}
}; // class AllOfGroup


//
// AtLeastOneGroup
//

//! Group of args where at least one argument should be defined.
class AtLeastOneGroup final
	:	public GroupIface
{
public:
	template< typename T >
	explicit AtLeastOneGroup( T && name,
		bool required = false )
		:	GroupIface( std::forward< T > ( name ), required )
	{
	}

	virtual ~AtLeastOneGroup()
	{
	}

	//! \return Is this argument defined?
	bool isDefined() const override
	{
		for( const auto & arg : children() )
		{
			if( arg->isDefined() )
				return true;
		}

		return false;
	}

protected:
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
		GroupIface::checkCorrectnessBeforeParsing( flags, names );

		for( const auto & arg : children() )
		{
			if( arg->isRequired() )
				throw BaseException( std::string( "Required argument \"" ) +
					arg->name() +
					"\" is not allowed to " +
					"be in AtLeastOne group \"" + name() + "\"." );
		}
	}
}; // class AtLeastOneGroup

} /* namespace Args */

#endif // ARGS__GROUPS_HPP__INCLUDED

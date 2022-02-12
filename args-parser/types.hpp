
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

#ifndef ARGS__TYPES_HPP__INCLUDED
#define ARGS__TYPES_HPP__INCLUDED

#ifdef ARGS_WSTRING_BUILD
	// C++ include.
	#include <string>
	#include <iostream>

	#ifdef ARGS_LIST
		#include <list>
	#elif defined( ARGS_DEQUE )
		#include <deque>
	#else
		#include <vector>
	#endif

#elif defined( ARGS_QSTRING_BUILD )
	// Qt include.
	#include <QString>
	#include <QTextStream>

	#ifdef ARGS_LIST
		#include <QLinkedList>
	#elif defined( ARGS_DEQUE )
		#include <QList>
	#else
		#include <QVector>
	#endif

#else
	// C++ include.
	#include <string>
	#include <iostream>

	#ifdef ARGS_LIST
		#include <list>
	#elif defined( ARGS_DEQUE )
		#include <deque>
	#else
		#include <vector>
	#endif

#endif


namespace Args {

#ifdef ARGS_WSTRING_BUILD

//! String type.
using String = std::wstring;

//! Char type.
using Char = String::value_type;

//! Out stream type.
using OutStreamType = std::wostream;

//! Output stream.
static OutStreamType & outStream()
{
	return std::wcout;
}

#define SL(str) L##str

//! List of strings.
#ifdef ARGS_LIST
	using StringList = std::list< String >;
#elif defined( ARGS_DEQUE )
	using StringList = std::deque< String >;
#else
	using StringList = std::vector< String >;
#endif

#elif defined( ARGS_QSTRING_BUILD )

//! Char type.
using Char = QChar;

//! Out stream type.
using OutStreamType = QTextStream;

class String {
public:
	using size_type = int;

	String()
	{
	}

	String( size_type size, Char ch )
		:	m_str( size, ch )
	{
	}

	String( const char * str )
		:	m_str( str )
	{
	}

	String( const QString & other )
		:	m_str( other )
	{
	}


	String( const Char * unicode, size_type size = -1 )
		:	m_str( unicode, size )
	{
	}

	String( Char ch )
		:	m_str( ch )
	{
	}

	String( QLatin1String str )
		:	m_str( str )
	{
	}

	String( const QByteArray & ba )
		:	m_str( ba )
	{
	}

	operator QString ()
	{
		return m_str;
	}

	operator QString () const
	{
		return m_str;
	}

	inline bool empty() const
	{
		return m_str.isEmpty();
	}

	static const int npos = -1;

	inline int find( Char ch ) const
	{
		return m_str.indexOf( ch );
	}

	inline int find( const String & str ) const
	{
		return m_str.indexOf( str.m_str );
	}

	QString::iterator begin()
	{
		return m_str.begin();
	}

	QString::iterator end()
	{
		return m_str.end();
	}

	QString::const_iterator begin() const
	{
		return m_str.begin();
	}

	QString::const_iterator end() const
	{
		return m_str.end();
	}

	QString::const_iterator cbegin() const
	{
		return m_str.begin();
	}

	QString::const_iterator cend() const
	{
		return m_str.end();
	}

	size_type length() const
	{
		return m_str.length();
	}

	String substr( size_type pos, size_type count = npos ) const
	{
		return m_str.mid( pos, count );
	}

	friend bool operator == ( const String & s1, const String & s2 )
	{
		return ( s1.m_str == s2.m_str );
	}

	friend bool operator != ( const String & s1, const String & s2 )
	{
		return ( s1.m_str != s2.m_str );
	}

	friend String operator + ( const String & s1, const String & s2 )
	{
		return String( s1.m_str + s2.m_str );
	}

	friend String operator + ( const String & s1, const char * s2 )
	{
		return String( s1.m_str + s2 );
	}

	friend String operator + ( const char * s1, const String & s2 )
	{
		return String( s1 + s2.m_str );
	}

	friend String operator + ( const String & s1, const char ch )
	{
		return String( s1.m_str + ch );
	}

	friend String operator + ( const char ch, const String & s2 )
	{
		return String( ch + s2.m_str );
	}

	friend bool operator < ( const String & s1, const String & s2 )
	{
		return s1.m_str < s2.m_str;
	}

	friend OutStreamType & operator << ( OutStreamType & to,
		const String & what )
	{
		to << what.m_str;

		return to;
	}

	const Char operator [] ( size_type pos ) const
	{
		return m_str[ pos ];
	}

	String & append( const String & other )
	{
		m_str.append( other.m_str );

		return *this;
	}

	String & append( size_type count, Char ch )
	{
		m_str.append( QString( count, ch ) );

		return *this;
	}

	void clear()
	{
		m_str.clear();
	}

private:
	//! Actual string.
	QString m_str;
}; // class String

//! Output stream.
static OutStreamType & outStream()
{
	static QTextStream stream( stdout );

	return stream;
}

#define SL(str) str

//! List of strings.
#ifdef ARGS_LIST
	using StringList = QLinkedList< String >;
#elif defined( ARGS_DEQUE )
	using StringList = QList< String >;
#else
	using StringList = QVector< String >;
#endif

#else

//! String type.
using String = std::string;

//! Char type.
using Char = String::value_type;

//! Out stream type.
using OutStreamType = std::ostream;

//! Output stream.
static OutStreamType & outStream()
{
	return std::cout;
}

#define SL(str) str

//! List of strings.
#ifdef ARGS_LIST
	using StringList = std::list< String >;
#elif defined( ARGS_DEQUE )
	using StringList = std::deque< String >;
#else
	using StringList = std::vector< String >;
#endif

#endif


namespace details {

//
// Deleter
//

//! Deleter.
template< typename T >
class Deleter {
public:
	explicit Deleter( bool shouldIDelete )
		:	m_delete( shouldIDelete )
	{
	}

	void operator () ( T * obj ) noexcept
	{
		if( m_delete )
			delete obj;
	}

private:
	//! Should deleter delete object?
	bool m_delete;
}; // class Deleter

} /* namespace details */

} /* namespace Args */

#endif // ARGS__TYPES_HPP__INCLUDED


/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013 Igor Mironchik

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

#ifndef UNITTEST__UNIT_TEST_HPP__INCLUDED
#define UNITTEST__UNIT_TEST_HPP__INCLUDED

// C++ include.
#include <string>
#include <map>
#include <list>
#include <memory>
#include <stdexcept>
#include <sstream>


//
// Test
//

//! Test.
class Test {
public:
	explicit Test( const std::string & name );

	virtual ~Test();

	//! \return name of the test.
	const std::string & name() const;

	//! Run test.
	void runTest();

	//! Test body.
	virtual void testBody() = 0;

private:
	//! Name of the test.
	std::string m_name;
}; // class Test


//
// TestInfo
//

//! Test info.
class TestInfo final {
public:
	TestInfo( const std::string & testCaseName,
		const std::shared_ptr< Test > & test );
}; // class TestInfo


//
// TestCase
//

//! Test case.
class TestCase final {
public:
	explicit TestCase( const std::string & name );

	~TestCase();

	//! \return Name of the test case.
	const std::string & name() const;

	//! Add new test.
	void addTest( const std::shared_ptr< Test > & test );

	//! Run all tests.
	void runAllTests();

private:
	TestCase( const TestCase & );

	TestCase & operator= ( const TestCase & );

private:
	//! Tests.
	std::list< std::shared_ptr< Test > > m_tests;
	//! Test case name.
	std::string m_name;
}; // class TestCase


//
// UnitTest
//

//! Unit test.
class UnitTest final {
private:
	UnitTest();

	~UnitTest();

	UnitTest( const UnitTest & );

	UnitTest & operator= ( const UnitTest & );

public:
	//! \return Instance of the singleton of this class.
	static UnitTest & instance();

	//! \return Test case for the given \arg name.
	TestCase * createTestCaseIfNotExists( const std::string & name );

	//! Run all tests.
	void runAllTests();

private:
	//! Test cases.
	std::map< std::string, std::shared_ptr< TestCase > > m_testCases;
}; // class UnitTest


//
// TEST
//

#define TEST( TestCaseName, TestName ) \
class TestName##Class final \
	:	public Test \
{ \
public: \
	TestName##Class() \
		:	Test( #TestName ) \
	{ \
	} \
 \
	~TestName##Class() \
	{ \
	} \
 \
	void testBody(); \
 \
	static TestInfo testInfo; \
}; \
 \
TestInfo TestName##Class::testInfo = \
	TestInfo( #TestCaseName, std::shared_ptr< Test > ( \
		new TestName##Class() ) ); \
void TestName##Class::testBody()


//
// RUN_ALL_TESTS
//

#define RUN_ALL_TESTS() \
	UnitTest::instance().runAllTests();


//
// CHECK_CONDITION
//

#define CHECK_CONDITION( Condition ) \
	if( !(Condition) ) \
	{ \
		std::ostringstream stream; \
		stream << "     file: \"" << __FILE__ \
			<< "\" line: " << __LINE__ << "\n" \
			<< "     ( " << #Condition << " ) [FAILED]"; \
 \
		throw std::runtime_error( stream.str() ); \
	}

#endif // UNITTEST__UNIT_TEST_HPP__INCLUDED

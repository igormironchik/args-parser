
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

// UnitTest include.
#include <UnitTest/unit_test.hpp>

// C++ include.
#include <exception>
#include <iostream>


//
// Test
//

Test::Test( const std::string & name )
	:	m_name( name )
{
}

Test::~Test()
{
}

const std::string &
Test::name() const
{
	return m_name;
}

void
Test::runTest()
{
	try {
		testBody();
	}
	catch( const std::exception & x )
	{
		std::cout << "[FAILED]" << std::endl;
		std::cout << x.what();

		exit( 1 );
	}
	catch( ... )
	{
		std::cout << "[FAILED]" << std::endl;
		std::cout << "  Unexpected exception.";

		exit( 1 );
	}
}


//
// TestInfo
//

TestInfo::TestInfo( const std::string & testCaseName,
	const std::shared_ptr< Test > & test )
{
	UnitTest::instance().createTestCaseIfNotExists( testCaseName )->
		addTest( test );
}


//
// TestCase
//

TestCase::TestCase( const std::string & name )
	:	m_name( name )
{
}

TestCase::~TestCase()
{
}

const std::string &
TestCase::name() const
{
	return m_name;
}

void
TestCase::addTest( const std::shared_ptr< Test > & test )
{
	m_tests.push_back( test );
}

void
TestCase::runAllTests()
{
	std::cout << name() << " started..." << std::endl;

	size_t i = 1;

	for( auto & t : m_tests )
	{
		std::cout.width( 2 );
		std::cout.fill( '0' );

		std::cout << " " << i << ". "
			<< t->name() << "... ";

		t->runTest();

		std::cout << "[OK]" << std::endl;
	}

	std::cout << name() << " finished..." << std::endl;
}


//
// UnitTest
//

UnitTest::UnitTest()
{
}

UnitTest::~UnitTest()
{
}

UnitTest &
UnitTest::instance()
{
	static UnitTest unit;

	return unit;
}

TestCase *
UnitTest::createTestCaseIfNotExists( const std::string & name )
{
	auto it = m_testCases.find( name );

	if( it == m_testCases.end() )
	{
		auto p = m_testCases.insert(
			std::pair< std::string, std::shared_ptr< TestCase > >( name,
				std::shared_ptr< TestCase > ( new TestCase( name ) ) ) );

		it = p.first;
	}

	return it->second.get();
}

void
UnitTest::runAllTests()
{
	for( auto & p : m_testCases )
		p.second->runAllTests();
}

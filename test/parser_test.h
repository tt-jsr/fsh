#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class ParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ParserTest);
        CPPUNIT_TEST(testParseIdentifier);
        CPPUNIT_TEST(testParseNumber);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testParseIdentifier();
    void testParseNumber();
};


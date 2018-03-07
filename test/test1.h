#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../machine.h"

class Test1 : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(Test1);
        CPPUNIT_TEST(testBasic);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
    bool StartParser(const char *);

    void testBasic();
};


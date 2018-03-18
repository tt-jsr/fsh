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
        CPPUNIT_TEST(testControl);
        CPPUNIT_TEST(testFunctions);
        CPPUNIT_TEST(testList);
        CPPUNIT_TEST(testString);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
    void ValidateBool(const char *varname, bool expected);
    bool StartParser(const char *);
    void UnitTestException(const char *);
    void UnitTestControl(int);
    void UnitTestFunctions(int);
    void UnitTestList(int);
    void UnitTestString(int);

    void testBasic();
    void testControl();
    void testFunctions();
    void testList();
    void testString();
};


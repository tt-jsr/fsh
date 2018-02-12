#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include "parser_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ParserTest);

int main (int argc, char* argv[]) 
{
  
    CppUnit::TestResult testResult;
    CppUnit::TextTestRunner runner;
    runner.addTest(ParserTest::suite());
    runner.run();

    return 0;
}

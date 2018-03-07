#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include <iostream>
#include "test1.h"
#include "../common.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Test1);

int main (int argc, char* argv[]) 
{
  
    CppUnit::TestResult testResult;
    CppUnit::TextTestRunner runner;
    runner.addTest(Test1::suite());
    runner.run();

    return 0;
}


extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern uint64_t lineno;
extern uint64_t column;

fsh::Machine machine;

void yyerror(const char *s) {
    std::cout << "EEK, parse error!  Message: " << s << " line: " << lineno << ":" << column << std::endl;
	// might as well halt now:
	exit(-1);
}


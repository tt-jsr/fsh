#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include <iostream>
#include "../instrusive_ptr.h"
#include "../common.h"
#include "../element.h"
#include "../bytecode.h"
#include "../builtins.h"
#include "../machine.h"
#include "../ast.h"
#include "test1.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Test1);

int main (int argc, char* argv[]) 
{
  
    CppUnit::TestResult testResult;
    CppUnit::TextTestRunner runner;
    runner.addTest(Test1::suite());
    runner.run();

    return 0;
}


extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern uint64_t lineno;
extern uint64_t column;

fsh::Machine machine;

void yyerror(const char *s) {
    std::cout << "EEK, parse error!  Message: " << s << " line: " << lineno << ":" << column << std::endl;
	// might as well halt now:
	exit(-1);
}


#include <iostream>
#include "test1.h"
#include "../common.h"
#include "../builtins.h"
#include "../element.h"
#include "../machine.h"

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern uint64_t lineno;
extern uint64_t column;
extern fsh::Machine machine;

bool Test1::StartParser(const char *fname)
{
	// open a file handle to a particular file:
	FILE *myfile = fopen(fname, "r");
	// make sure it's valid:
	if (!myfile) {
        std::cout << "Cannot open " << fname << std::endl;
		return false;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;

    RegisterBuiltIns(machine);

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
    return true;
}

void Test1::setUp()
{
}

void Test1::tearDown()
{
}

void Test1::testBasic()
{
    StartParser("test1.fsh");

    std::string svalue;
    bool bvalue;
    double dvalue;
    int64_t ivalue;
    fsh::ElementPtr e;
    
    CPPUNIT_ASSERT(machine.get_variable("str", svalue));
    CPPUNIT_ASSERT(svalue == "hello");
    
    CPPUNIT_ASSERT(machine.get_variable("int", ivalue));
    CPPUNIT_ASSERT(ivalue == 1234);
    
    CPPUNIT_ASSERT(machine.get_variable("flt", dvalue));
    CPPUNIT_ASSERT(dvalue == 12.27);
    
    CPPUNIT_ASSERT(machine.get_variable("bt", bvalue));
    CPPUNIT_ASSERT(bvalue == true);
    
    CPPUNIT_ASSERT(machine.get_variable("bf", bvalue));
    CPPUNIT_ASSERT(bvalue == false);
    
    CPPUNIT_ASSERT(machine.get_variable("none", e));
    CPPUNIT_ASSERT(e->IsNone());
    
    CPPUNIT_ASSERT(machine.get_variable("__str2_3", svalue));
    CPPUNIT_ASSERT(svalue == "__str2_3");
}

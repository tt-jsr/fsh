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

    lineno = 0;
    column = 0;
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

void Test1::ValidateBool(const char *varname, bool expected)
{
    bool b;
    CPPUNIT_ASSERT(machine.get_variable(varname,  b));
    CPPUNIT_ASSERT(b == expected);
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

    CPPUNIT_ASSERT(machine.get_variable("__str2_3", svalue));
    CPPUNIT_ASSERT(svalue == "__str2_3");

    CPPUNIT_ASSERT(machine.get_variable("s2", svalue));
    CPPUNIT_ASSERT(svalue == "hello world");

    //CPPUNIT_ASSERT(machine.get_variable("n", ivalue));
    //CPPUNIT_ASSERT(ivalue == -1234);
    
    bool b;
    CPPUNIT_ASSERT(machine.get_variable("v1",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("v2",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("v3",  b));
    CPPUNIT_ASSERT(b == false);

    CPPUNIT_ASSERT(machine.get_variable("v4",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("v5",  b));
    CPPUNIT_ASSERT(b == false);

    CPPUNIT_ASSERT(machine.get_variable("v6",  b));
    CPPUNIT_ASSERT(b == false);

    CPPUNIT_ASSERT(machine.get_variable("v7",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("v8",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("v9",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("v10",  b));
    CPPUNIT_ASSERT(b == false);

    CPPUNIT_ASSERT(machine.get_variable("v11",  b));
    CPPUNIT_ASSERT(b == false);

    CPPUNIT_ASSERT(machine.get_variable("v12",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("v13",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("op1",  b));
    CPPUNIT_ASSERT(b == false);

    CPPUNIT_ASSERT(machine.get_variable("op2",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("op3",  b));
    CPPUNIT_ASSERT(b == true);

    CPPUNIT_ASSERT(machine.get_variable("op4",  b));
    CPPUNIT_ASSERT(b == false);

    CPPUNIT_ASSERT(machine.get_variable("m1",  ivalue));
    CPPUNIT_ASSERT(ivalue == 51);

    CPPUNIT_ASSERT(machine.get_variable("m2",  ivalue));
    CPPUNIT_ASSERT(ivalue == 84);

    CPPUNIT_ASSERT(machine.get_variable("m3",  ivalue));
    CPPUNIT_ASSERT(ivalue == -18);
}

void Test1::UnitTestControl(int n)
{
    bool bvalue;
    int64_t nvalue;
    switch (n)
    {
    case 1:
        CPPUNIT_ASSERT(machine.get_variable("result",  bvalue));
        CPPUNIT_ASSERT(bvalue == true);
        break;
    case 2:
        CPPUNIT_ASSERT(machine.get_variable("result",  bvalue));
        CPPUNIT_ASSERT(bvalue == false);
        break;
    case 3:
        CPPUNIT_ASSERT(machine.get_variable("result",  nvalue));
        CPPUNIT_ASSERT(nvalue == 0);
        break;
    }
}

void Test1::testControl()
{
    machine.reset();
    std::function<void (int)> f = std::bind(&Test1::UnitTestControl, this, std::placeholders::_1);
    machine.register_unittest(f);
    StartParser("control.fsh");
}


void Test1::UnitTestFunctions(int n)
{
    bool bvalue;
    int64_t nvalue;
    std::string svalue;
    fsh::ElementPtr e;
    switch (n)
    {
    case 1:
        CPPUNIT_ASSERT(machine.get_variable("result",  bvalue));
        CPPUNIT_ASSERT(bvalue == true);
        break;
    case 2:
        CPPUNIT_ASSERT(machine.get_variable("arg1",  svalue));
        CPPUNIT_ASSERT(svalue == "hello");
        break;
    case 3:
        CPPUNIT_ASSERT(machine.get_variable("arg1",  e) == false);
        break;
    case 4:
        CPPUNIT_ASSERT(machine.get_variable("_exception",  svalue));
        CPPUNIT_ASSERT(svalue == "test throw");
        CPPUNIT_ASSERT(machine.get_variable("var",  e) == false);
        break;
    }
}

void Test1::testFunctions()
{
    machine.reset();
    std::function<void (int)> f = std::bind(&Test1::UnitTestFunctions, this, std::placeholders::_1);
    machine.register_unittest(f);
    StartParser("functions.fsh");
}

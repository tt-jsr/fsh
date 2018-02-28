#include <iostream>
#include "common.h"
#include "list.h"
#include "machine.h"
#include "instructions.h"
#include "builtins.h"

using namespace fsh;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

extern Machine machine;

int main(int argc, char *argv[])
{
	// open a file handle to a particular file:
	FILE *myfile = fopen("fsh.input", "r");
	// make sure it's valid:
	if (!myfile) {
        std::cout << "Cannot open fsh.input" << std::endl;
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;

    machine.register_builtin("Print", fsh::Print);
    //machine.register_builtin("If", fsh::If);
    //machine.register_builtin("While", fsh::While);
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
}

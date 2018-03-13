#include <iostream>
#include "common.h"
#include "machine.h"
#include "instructions.h"
#include "builtins.h"

using namespace fsh;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char * yytext;
extern uint64_t lineno;
extern uint64_t column;

Machine machine;

void yyerror(const char *s) {
    std::cout << "Syntax error: " << s << " line: " << lineno << ":" << column << " " << yytext << std::endl;
	// might as well halt now:
	exit(-1);
}

int main(int argc, char *argv[])
{
	// open a file handle to a particular file:
	FILE *myfile = fopen("junk.fsh", "r");
	// make sure it's valid:
	if (!myfile) {
        std::cout << "Cannot open io.fsh" << std::endl;
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;

    RegisterBuiltIns(machine);

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
}

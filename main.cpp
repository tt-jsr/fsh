#include <iostream>
#include "common.h"
#include "machine.h"
#include "instructions.h"
#include "builtins.h"

using namespace fsh;

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern char * yytext;
extern uint64_t lineno;
extern uint64_t column;
extern void command_line();

Machine machine;

void yyerror(const char *s) {
    std::cout << "Syntax error: " << s << " line: " << lineno << ":" << column << " \"" << yytext << "\"" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "usage: fsh script" << std::endl;
        return 1;
    }
	// open a file handle to a particular file:
	FILE *myfile = fopen(argv[1], "r");
	// make sure it's valid:
	if (!myfile) {
        std::cout << "Cannot open " << argv[1] << std::endl;
		return 1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;

    RegisterBuiltIns(machine);

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
    fclose(myfile);
    command_line();
}

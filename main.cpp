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
    const char *fname = "junk.fsh";
	// open a file handle to a particular file:
	FILE *myfile = fopen(fname, "r");
	// make sure it's valid:
	if (!myfile) {
        std::cout << "Cannot open " << fname << std::endl;
		return -1;
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

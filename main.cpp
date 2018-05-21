#include <iostream>
#include <signal.h>
#include "instrusive_ptr.h"
#include "common.h"
#include "element.h"
#include "bytecode.h"
#include "builtins.h"
#include "machine.h"

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

static struct sigaction entry_int, entry_quit, entry_pipe;

static bool ignore_sig(void)
{
	static bool first = true;
	struct sigaction act_ignore;

	memset(&act_ignore, 0, sizeof(act_ignore));
	act_ignore.sa_handler = SIG_IGN; /* may generate warning on Solaris */
	if (first) {
		first = false;
		sigaction(SIGINT, &act_ignore, &entry_int);
		sigaction(SIGQUIT, &act_ignore, &entry_quit);
		sigaction(SIGPIPE, &act_ignore, &entry_pipe);
	}
	else {
		sigaction(SIGINT, &act_ignore, NULL);
		sigaction(SIGQUIT, &act_ignore, NULL);
		sigaction(SIGPIPE, &act_ignore, NULL);
	}
	return true;
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

    ignore_sig();

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
    fclose(myfile);
    command_line();
}

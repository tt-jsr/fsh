%{
#include <cstdio>
#include <iostream>
#include "common.h"
#include "instructions.h"
#include "machine.h"
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
fsh::instruction::Instruction *result;
 
void yyerror(const char *s);
%}

%define api.value.type {void *} // Actually is instruction::Instruction

// define the constant-string tokens:
%token FSH_IDENTIFIER
%token FSH_INT
%token FSH_FLOAT
%left '='
%left '+' '-'
%left '*' '/'

%% /* The grammar follows. */
input:
    %empty
    | input exp
    ;
exp:
    FSH_INT { 
        $$ = $1; 
        result = (fsh::instruction::Instruction *)$1;
    }
    | FSH_IDENTIFIER { 
        $$ = $1; 
        result = (fsh::instruction::Instruction *)$1;
    }
    | FSH_IDENTIFIER '=' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_ASSIGNMENT;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
        result = bop;
    }
    //| FNCT '(' exp ')' { $$ = (*($1->value.fnctptr))($3); }
    | exp '+' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_PLUS;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
        result = bop;
    }
        
    | exp '-' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_MINUS;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
        result = bop;
    }
    | exp '*' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_MULTIPLY;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
        result = bop;
    }
    | exp '/' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_DIVIDE;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
        result = bop;
    }
    //| '-' exp %prec NEG { $$ = -$2; }
    //| exp '^' exp { $$ = pow ($1, $3); }
    //| '(' exp ')' { $$ = $2; }
    ;
/* End of grammar. */
%%


int main(int, char**) {
	// open a file handle to a particular file:
	FILE *myfile = fopen("fsh.input", "r");
	// make sure it's valid:
	if (!myfile) {
		cout << "Cannot open fsh.input" << endl;
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;

    fsh::Machine machine;
	// parse through the input until there is no more:
	do {
		yyparse();
        fsh::instruction::InstructionPtr inst(result);
        fsh::ElementPtr e = machine.Execute(inst);
        switch (e->type())
        {
        case fsh::ELEMENT_TYPE_INTEGER:
            {
                fsh::IntegerPtr ip = e.cast<fsh::Integer>();
                std::cout << ip->value;
            }
        default:
            break;
        }
	} while (!feof(yyin));
	
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}


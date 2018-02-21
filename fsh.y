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
void Execute(fsh::instruction::Instruction *pInst);
 
void yyerror(const char *s);
%}

%define api.value.type {void *} // Actually is instruction::Instruction

// define the constant-string tokens:
%token IDENTIFIER
%token INT
%token FLOAT
%left ','
%left '='
%left '+' '-'
%left '*' '/'
%precedence NEG

%% /* The grammar follows. */
input:
    %empty
    | input toplev
    ;

toplev: exp {
        Execute((fsh::instruction::Instruction *)$1);
      }
      ;
exp:
    INT { 
        $$ = $1; 
    }
    | FLOAT { 
        $$ = $1; 
    }
    | IDENTIFIER { 
        $$ = $1; 
    }
    | IDENTIFIER '=' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_ASSIGNMENT;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | IDENTIFIER '[' exp_list ']' { 
        fsh::instruction::FunctionCall *pCall = new fsh::instruction::FunctionCall();
        fsh::instruction::Identifier *id = (fsh::instruction::Identifier *)$1;
        pCall->name = id->name;
        delete id;
        pCall->args = (fsh::instruction::ExpressionList *)$3;
        $$ = pCall;
    }
    | IDENTIFIER '[' ']' { 
        fsh::instruction::FunctionCall *pCall = new fsh::instruction::FunctionCall();
        fsh::instruction::Identifier *id = (fsh::instruction::Identifier *)$1;
        pCall->name = id->name;
        delete id;
        $$ = pCall;
    }
    | exp '+' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_PLUS;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp '-' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_MINUS;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp '*' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_MULTIPLY;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp '/' exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_DIVIDE;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | '-' exp %prec NEG { 
        fsh::instruction::Instruction *p = (fsh::instruction::Instruction *)$2;
        if (p->type() == fsh::instruction::INSTRUCTION_INTEGER)
        {
            fsh::instruction::Integer *pInt = (fsh::instruction::Integer *)p;
            pInt->value = -pInt->value;
        }
        else if (p->type() == fsh::instruction::INSTRUCTION_FLOAT)   {
            fsh::instruction::Float *pFloat = (fsh::instruction::Float *)p;
            pFloat->value = -pFloat->value;
        }
        $$ = p; 
    }
    | '(' exp ')' { $$ = $2; }
    ;
exp_list:
    exp     {
        fsh::instruction::ExpressionList *el = new fsh::instruction::ExpressionList();
        el->expressions.push_back((fsh::instruction::Instruction *)$1);
        $$ = el;
    }
    | exp ',' exp   {
        fsh::instruction::ExpressionList *el = new fsh::instruction::ExpressionList();
        el->expressions.push_back((fsh::instruction::Instruction *)$1);
        el->expressions.push_back((fsh::instruction::Instruction *)$3);
        $$ = el;
    }
    | exp_list ',' exp {
        fsh::instruction::ExpressionList *el = (fsh::instruction::ExpressionList *)$1;
        el->expressions.push_back((fsh::instruction::Instruction *)$3);
        $$ = el;
    }
    ;
/* End of grammar. */
%%

fsh::Machine machine;

void Execute(fsh::instruction::Instruction *pInst)
{
    fsh::instruction::InstructionPtr inst(pInst);
    fsh::ElementPtr e = machine.Execute(inst);
    switch (e->type())
    {
    case fsh::ELEMENT_TYPE_INTEGER:
        {
            fsh::IntegerPtr ip = e.cast<fsh::Integer>();
            std::cout << ip->value << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_FLOAT:
        {
            fsh::FloatPtr ip = e.cast<fsh::Float>();
            std::cout << ip->value << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_ERROR:
        {
            fsh::ErrorPtr ep = e.cast<fsh::Error>();
            std::cout << ep->msg << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_IDENTIFIER:
        {
            fsh::IdentifierPtr id = e.cast<fsh::Identifier>();
            std::cout << id->name << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_NONE:
        {
            std::cout << "None" << std::endl;
        }
        break;
    default:
        std::cout << "Unhandled return from Execute!" << std::endl;
        std::cout << "type: " << e->type() << std::endl;
        break;
    }
}

fsh::ElementPtr TestFuncBody(fsh::Machine& machine, std::vector<fsh::ElementPtr>& args)
{
    std::cout << "Testfunc: ";
    for (auto& e : args)
    {
        switch(e->type())
        {
        case fsh::ELEMENT_TYPE_INTEGER:
            {
                fsh::IntegerPtr n = e.cast<fsh::Integer>();
                std::cout << " " << n->value;
            }
            break;
        case fsh::ELEMENT_TYPE_FLOAT:
            {
                fsh::FloatPtr f = e.cast<fsh::Float>();
                std::cout << " " << f->value;
            }
            break;
        default:
            std::cout << " TestFunc no handler ";
            break;
        }
    }
    std::cout << std::endl;
    fsh::ElementPtr e = fsh::MakeInteger(43);
    return e;
}

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

    machine.register_builtin("TestFunc", TestFuncBody);
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
	
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}


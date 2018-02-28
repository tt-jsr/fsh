%{
#include <cstdio>
#include <iostream>
#include "common.h"
#include "instructions.h"
#include "machine.h"
#include "builtins.h"
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern uint64_t lineno;
void Execute(fsh::instruction::Instruction *pInst);
 
void yyerror(const char *s);
%}

%define api.value.type {void *} // Actually is instruction::Instruction

// define the constant-string tokens:
%token IDENTIFIER
%token VARIABLE
%token NONE
%token STRING
%token INT
%token FLOAT
%token TRUE
%token FALSE
%token IF
%token WHILE
%token THEN
%token ELSE
%token ';'
%left ','
%left '='
%left GT LT GTE LTE
%left EQ NEQ
%left '+' '-'
%left '*' '/'
%precedence NEG

%% /* The grammar follows. */
input:
    %empty
    | input toplev
    ;

toplev: exp  {
        //fsh::instruction::DumpContext ctx(std::cout);
        fsh::instruction::Instruction *pInst = (fsh::instruction::Instruction *)$1;
        //pInst->dump(ctx);
        //std::cout << "***********************************" << std::endl;
        Execute(pInst);
      }
      ;
term:
    INT { 
        $$ = $1; 
    }
    | FLOAT { 
        $$ = $1; 
    }
    | NONE {
        $$ = new fsh::instruction::None();
    }
    | TRUE {
        $$ = new fsh::instruction::Boolean(true);
    }
    | FALSE {
        $$ = new fsh::instruction::Boolean(false);
    }
    | STRING {
        $$ = $1;
    }
    | VARIABLE {
        $$ = $1;
    }
    ;
exp:
    term {$$ = $1;}
    |
    IDENTIFIER '=' exp { 
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
        pCall->functionArguments = (fsh::instruction::Instruction *)$3;
        //std::cout << "adding " << id->name << std::endl;
        $$ = pCall;
    }
    | IDENTIFIER '[' ']' { 
        fsh::instruction::FunctionCall *pCall = new fsh::instruction::FunctionCall();
        fsh::instruction::Identifier *id = (fsh::instruction::Identifier *)$1;
        //std::cout << "adding " << id->name << std::endl;
        pCall->name = id->name;
        delete id;
        $$ = pCall;
    }
    |'&' '[' iden_list ':' exp_list ']' {
        fsh::instruction::FunctionDef *pDef = new fsh::instruction::FunctionDef();
        fsh::instruction::IdentifierList *il = (fsh::instruction::IdentifierList *)$3;
        for (auto& inst : il->identifiers)
        {
            if (inst->type() == fsh::instruction::INSTRUCTION_IDENTIFIER)
            {
                fsh::instruction::IdentifierPtr id = inst.cast<fsh::instruction::Identifier>();
                pDef->arg_names.push_back(id->name);
            }
        }
        pDef->functionBody = (fsh::instruction::Instruction *)$5;
        delete il;
        $$ = pDef;
    }
    |'&' '[' exp_list ']' {
        fsh::instruction::FunctionDef *pDef = new fsh::instruction::FunctionDef();
        pDef->functionBody = (fsh::instruction::Instruction *)$3;
        $$ = pDef;
    }
    | IF '[' exp_list  THEN exp_list  ELSE exp_list  ']'  {
        fsh::instruction::WhileIf *pWhileIf = new fsh::instruction::WhileIf();
        pWhileIf->isWhile = false;
        pWhileIf->condition = (fsh::instruction::Instruction *)$3;
        pWhileIf->if_true = (fsh::instruction::Instruction *)$5;
        pWhileIf->if_false = (fsh::instruction::Instruction *)$7;
        $$ = pWhileIf;
    }
    | WHILE '[' exp_list  THEN exp_list  ELSE exp_list  ']'  {
        fsh::instruction::WhileIf *pWhileIf = new fsh::instruction::WhileIf();
        pWhileIf->isWhile = true;
        pWhileIf->condition = (fsh::instruction::Instruction *)$3;
        pWhileIf->if_true = (fsh::instruction::Instruction *)$5;
        pWhileIf->if_false = (fsh::instruction::Instruction *)$7;
        $$ = pWhileIf;
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
    | exp GT exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_GT;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp GTE exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_GTE;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp LT exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_LT;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp LTE exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_LTE;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp EQ exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_EQ;
        bop->lhs = (fsh::instruction::Instruction *)$1;
        bop->rhs = (fsh::instruction::Instruction *)$3;
        $$ = bop;
    }
    | exp NEQ exp { 
        fsh::instruction::BinaryOperator *bop = new fsh::instruction::BinaryOperator();
        bop->op = fsh::TOKEN_NEQ;
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
    | '(' exp_list ')' { $$ = $2; }
    | '{' exp_list '}' { 
        fsh::instruction::ExpressionList *el = (fsh::instruction::ExpressionList *)$2;
        el->isList = true;
        $$ = $2; 
    }
    ;
iden_list:
    IDENTIFIER {
        fsh::instruction::IdentifierList *il = new fsh::instruction::IdentifierList();
        il->identifiers.push_back((fsh::instruction::Instruction *)$1);
        $$ = il;
    }
    | IDENTIFIER ',' IDENTIFIER {
        fsh::instruction::IdentifierList *il = new fsh::instruction::IdentifierList();
        il->identifiers.push_back((fsh::instruction::Instruction *)$1);
        il->identifiers.push_back((fsh::instruction::Instruction *)$3);
        $$ = il;
    }
    | iden_list ',' IDENTIFIER {
        fsh::instruction::IdentifierList *il = (fsh::instruction::IdentifierList *)$1;
        il->identifiers.push_back((fsh::instruction::Instruction *)$3);
        $$ = il;
    }
    ;
exp_list:
    exp {
        fsh::instruction::ExpressionList *el = new fsh::instruction::ExpressionList();
        el->expressions.push_back((fsh::instruction::Instruction *)$1);
        $$ = el;
    }
    | exp ',' exp {
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
    case fsh::ELEMENT_TYPE_ERROR:
        {
            fsh::ErrorPtr ep = e.cast<fsh::Error>();
            std::cout << ep->msg << std::endl;
        }
        break;
    /*
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
    case fsh::ELEMENT_TYPE_STRING:
        {
            std::cout << "String" << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_LIST:
        {
            std::cout << "List" << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_HEAD:
        {
            std::cout << "Head" << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_FUNCTION_BUILTIN:
        {
            std::cout << "FunctionBuiltIn" << std::endl;
        }
        break;
    case fsh::ELEMENT_TYPE_FUNCTION_SHELL:
        {
            std::cout << "FunctionShell" << std::endl;
        }
        break;
    default:
        std::cout << "Unhandled return from Execute!" << std::endl;
        std::cout << "type: " << e->type() << std::endl;
        break;
    */
    }
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << " line: " << lineno << endl;
	// might as well halt now:
	exit(-1);
}


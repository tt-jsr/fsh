
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
extern uint64_t column;
void Execute(fsh::instruction::Instruction *pInst);
 
extern void yyerror(const char *s);

typedef fsh::instruction::Instruction inst_t;
typedef fsh::instruction::BinaryOperator bop_t;
typedef fsh::instruction::FunctionCall call_t;
typedef fsh::instruction::FunctionDef fdef_t;
typedef fsh::instruction::WhileIf wif_t;
typedef fsh::instruction::ExpressionList el_t;
typedef fsh::instruction::IdentifierList il_t;
typedef fsh::instruction::Identifier iden_t;
typedef fsh::instruction::String str_t;
typedef fsh::instruction::Boolean bool_t;
typedef fsh::instruction::Integer int_t;
typedef fsh::instruction::Float float_t;
typedef fsh::instruction::None none_t;
typedef fsh::instruction::TryCatch trycatch_t;
typedef fsh::instruction::DotOperator dot_t;
typedef fsh::instruction::For for_t;

void dump(const char *p)
{
    std::cout << p << std::endl;
}

void dump(void *p)
{
    inst_t *ip = (inst_t *)p;
    switch(ip->type())
    {
    case fsh::instruction::INSTRUCTION_IDENTIFIER:
        std::cout << "IDENTIFIER" << std::endl;
        break;
    case fsh::instruction::INSTRUCTION_INTEGER:
        std::cout << "INTEGER" << std::endl;
        break;
    default:
        std::cout << "enum: " << ip->type() << std::endl;
        break;
    }
}
%}

%define api.value.type {void *} // Actually is instruction::Instruction

// define the constant-string tokens:
%token IDENTIFIER INTEGER FLOAT STRING_LITERAL
%token NONE TRUE FALSE
%token IF WHILE THEN ELSE
%token TRY CATCH
%token PART
%token FOR IN
%token ';'
%left ','
%left '='
%left AND OR
%left GT LT GTE LTE
%left EQ NEQ
%left '+' '-'
%left '*' '/'
%left '.'
%precedence NEG

%start input

%% /* The grammar follows. */
input:
    %empty
    | input toplev
    ;

toplev
    :statement {
        //fsh::instruction::DumpContext ctx(std::cout);
        inst_t *pInst = (inst_t *)$1;
        //pInst->dump(ctx);
        //std::cout << "***********************************" << std::endl;
        Execute(pInst);
    }
    ;

primary_expression
    : IDENTIFIER            { $$ = $1;}
    | INTEGER               {$$ = $1;}
    | FLOAT                 {$$ = $1;}
    | STRING_LITERAL        {$$ = $1;}
    | NONE                  {$$ = new none_t(lineno);}
    | TRUE                  {$$ = new bool_t(lineno, true);}
    | FALSE                 {$$ = new bool_t(lineno, false);}
    | '(' expression_list ')'    {$$ = $2;}
    | '{' expression_list '}' { 
        el_t *el = (el_t *)$2;
        el->isList = true;
        $$ = $2; 
    }
    ;

assignment_expression
    : IDENTIFIER '=' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_ASSIGNMENT;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    ;

expression
    : primary_expression        {$$ = $1;}
    | assignment_expression     {$$ = $1;}
    | math_expression           {$$ = $1;}
    | relational_expression     {$$ = $1;}
    | function_call             {$$ = $1;}
    | function_definition       {$$ = $1;}
    | selection_expression      {$$ = $1;}
    | iteration_expression      {$$ = $1;}
    | exception_expression      {$$ = $1;}
    | part_expression           {$$ = $1;}
    | dot_expression            {$$ = $1;}
    | for_expression            {$$ = $1;}
    ;

relational_expression
    :expression LT expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_LT;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    |expression LTE expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_LTE;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    |expression GT expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_GT;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    |expression GTE expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_GTE;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    |expression EQ expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_EQ;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    |expression NEQ expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_NEQ;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    |expression AND expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_AND;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    |expression OR expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_OR;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    ;

math_expression
    :expression '+' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_PLUS;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    | expression '-' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_MINUS;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    | expression '*' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_MULTIPLY;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    | expression '/' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::TOKEN_DIVIDE;
        bop->lhs = (inst_t *)$1;
        bop->rhs = (inst_t *)$3;
        $$ = bop;
    }
    | '-' expression %prec NEG {
        inst_t *p = (inst_t *)$2;
        if (p->type() == fsh::instruction::INSTRUCTION_INTEGER)
        {
            int_t *pInt = (int_t *)p;
            pInt->value = -pInt->value;
        }
        else if (p->type() == fsh::instruction::INSTRUCTION_FLOAT)   {
            float_t *pFloat = (float_t *)p;
            pFloat->value = -pFloat->value;
        }
        $$ = p; 
    }
    ;

part_expression
    : PART '[' expression ',' expression ':' expression ']' {
        call_t *pCall = new call_t(lineno);
        pCall->name = "Part";
        el_t *el = new fsh::instruction::ExpressionList(lineno);
        el->expressions.push_back((inst_t *)$3);
        el->expressions.push_back((inst_t *)$5);
        el->expressions.push_back(new str_t(lineno, ":"));
        el->expressions.push_back((inst_t *)$7);
        pCall->functionArguments = el;
        //std::cout << "func call " << id->name << std::endl;
        $$ = pCall;
    }
    | PART '[' expression ',' expression ':' ']' {
        call_t *pCall = new call_t(lineno);
        pCall->name = "Part";
        el_t *el = new fsh::instruction::ExpressionList(lineno);
        el->expressions.push_back((inst_t *)$3);
        el->expressions.push_back((inst_t *)$5);
        el->expressions.push_back(new str_t(lineno, ":"));
        pCall->functionArguments = el;
        //std::cout << "func call " << id->name << std::endl;
        $$ = pCall;
    }
    | PART '[' expression ',' expression ']' {
        call_t *pCall = new call_t(lineno);
        pCall->name = "Part";
        el_t *el = new fsh::instruction::ExpressionList(lineno);
        el->expressions.push_back((inst_t *)$3);
        el->expressions.push_back((inst_t *)$5);
        pCall->functionArguments = el;
        //std::cout << "func call " << id->name << std::endl;
        $$ = pCall;
    }
    | PART '[' expression ',' ':' expression ']' {
        call_t *pCall = new call_t(lineno);
        pCall->name = "Part";
        el_t *el = new fsh::instruction::ExpressionList(lineno);
        el->expressions.push_back((inst_t *)$3);
        el->expressions.push_back(new str_t(lineno, ":"));
        el->expressions.push_back((inst_t *)$6);
        pCall->functionArguments = el;
        //std::cout << "func call " << id->name << std::endl;
        $$ = pCall;
    }
    ;

dot_expression
    : IDENTIFIER '.' expression {
        dot_t *dop = new dot_t(lineno);
        dop->lhs = (inst_t *)$1;
        dop->rhs = (inst_t *)$3;
        $$ = dop;
    }
    ;

exception_expression
    : TRY '[' statement_list  CATCH statement_list  ']'  {
        trycatch_t *pException = new trycatch_t(lineno);
        pException->try_ = (inst_t *)$3;
        pException->catch_ = (inst_t *)$5;
        $$ = pException;
    }
    ;

selection_expression
    : IF '[' expression_list  THEN statement_list  ELSE statement_list  ']'  {
        wif_t *pWhileIf = new wif_t(lineno);
        pWhileIf->isWhile = false;
        pWhileIf->condition = (inst_t *)$3;
        pWhileIf->if_true = (inst_t *)$5;
        pWhileIf->if_false = (inst_t *)$7;
        $$ = pWhileIf;
    }
    ;

for_expression
    : FOR '[' IDENTIFIER IN expression THEN statement_list ']'  {
        for_t *pFor = new for_t(lineno);
        pFor->identifier = (inst_t *)$3;
        pFor->list = (inst_t *)$5;
        pFor->body = (inst_t *)$7;
        $$ = pFor;
    }
    ;

iteration_expression
    : WHILE '[' expression_list  THEN statement_list  ELSE statement_list  ']'  {
        wif_t *pWhileIf = new wif_t(lineno);
        pWhileIf->isWhile = true;
        pWhileIf->condition = (inst_t *)$3;
        pWhileIf->if_true = (inst_t *)$5;
        pWhileIf->if_false = (inst_t *)$7;
        $$ = pWhileIf;
    }
    ;

function_definition
    : '&' '[' identifier_list ':' statement_list ']' {
        fdef_t *pDef = new fdef_t(lineno);
        inst_t *arg3 = (inst_t *)$3;
        assert(arg3->type() == fsh::instruction::INSTRUCTION_IDENTIFIER
            || arg3->type() == fsh::instruction::INSTRUCTION_IDENTIFIER_LIST);
        if (arg3->type() == fsh::instruction::INSTRUCTION_IDENTIFIER)
        {
            iden_t * id = (iden_t *)$3;
            pDef->arg_names.push_back(id->name);
            delete id;
        }
        else
        {
            il_t *il = (il_t *)$3;
            for (auto& inst : il->identifiers)
            {
                if (inst->type() == fsh::instruction::INSTRUCTION_IDENTIFIER)
                {
                    fsh::instruction::IdentifierPtr id = inst.cast<fsh::instruction::Identifier>();
                    pDef->arg_names.push_back(id->name);
                }
            }
            delete il;
        }
        pDef->functionBody = (fsh::instruction::Instruction *)$5;
        $$ = pDef;
    }
    |'&' '[' statement_list ']' {
        fsh::instruction::FunctionDef *pDef = new fsh::instruction::FunctionDef(lineno);
        pDef->functionBody = (fsh::instruction::Instruction *)$3;
        $$ = pDef;
    }
    ;

function_call
    : IDENTIFIER '[' expression_list ']' {
        call_t *pCall = new call_t(lineno);
        iden_t *id = (iden_t *)$1;
        pCall->name = id->name;
        delete id;
        pCall->functionArguments = (inst_t *)$3;
        //std::cout << "func call " << id->name << std::endl;
        $$ = pCall;
    }
    | IDENTIFIER '[' ']' {
        call_t *pCall = new call_t(lineno);
        iden_t *id = (iden_t *)$1;
        pCall->name = id->name;
        delete id;
        //std::cout << "func call  " << id->name << std::endl;
        $$ = pCall;
    }
    ;

identifier_list
    :IDENTIFIER {
        $$ = $1;
    }
    |identifier_list ',' IDENTIFIER  {
        assert(((inst_t *)$3)->type() == fsh::instruction::INSTRUCTION_IDENTIFIER);
        iden_t *arg3 = (iden_t *)$3;
        inst_t *ip = (inst_t *)$1;
        if (ip->type() == fsh::instruction::INSTRUCTION_IDENTIFIER_LIST)
        {
            il_t *il = (il_t *)$1;
            il->identifiers.push_back(arg3);
            $$ = il;
        }
        else
        {
            assert(((inst_t *)$1)->type() == fsh::instruction::INSTRUCTION_IDENTIFIER);
            il_t *il = new il_t(lineno);
            il->identifiers.push_back((iden_t *)$1);
            il->identifiers.push_back(arg3);
            $$ = il;
        }
    }
    ;

expression_list
    :expression {
        //std::cout << "expression_list exp  " << std::endl;
        $$ = $1;
    }
    |expression_list ',' expression  {
        //std::cout << "expression_list list  "  << std::endl;
        inst_t *ip = (inst_t *)$1;
        if (ip->type() == fsh::instruction::INSTRUCTION_EXPRESSION_LIST)
        {
            el_t *el = (el_t *)$1;
            el->expressions.push_back((inst_t *)$3);
            $$ = el;
        }
        else
        {
            el_t *el = new el_t(lineno);
            el->expressions.push_back((inst_t *)$1);
            el->expressions.push_back((inst_t *)$3);
            $$ = el;
        }
    }
    ;

expression_statement
    :';' 
    | expression ';' { $$ = $1;}
    ;

statement
    :expression_statement {$$ = $1;}
    ;

statement_list
    :statement  {$$ = $1;}
    |statement_list statement   {
        inst_t *arg1 = (inst_t *)$1;
        el_t *el = nullptr;
        if (arg1->type() != fsh::instruction::INSTRUCTION_EXPRESSION_LIST)
        {
            el = new el_t(lineno);
            el->expressions.push_back(arg1);
        }
        else
        {
            el = (el_t *)arg1;
        }
        el->expressions.push_back((inst_t *)$2);
        $$ = el;
    }
    ;

/* End of grammar. */
%%

extern fsh::Machine machine;

void Execute(fsh::instruction::Instruction *pInst)
{
    fsh::instruction::InstructionPtr inst(pInst);
    fsh::ElementPtr e = machine.Execute(inst);
    switch (e->type())
    {
    case fsh::ELEMENT_TYPE_ERROR:
        {
            fsh::ErrorPtr ep = e.cast<fsh::Error>();
            if (ep->bOk == false)
                std::cout << "Execution returned: " << ep->msg << std::endl;
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



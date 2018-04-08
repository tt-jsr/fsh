
%{
#include <cstdio>
#include <iostream>
#include "common.h"
#include "ast.h"
#include "machine.h"
#include "builtins.h"
#include "shell.h"

using namespace std;

// stuff from flex that bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern uint64_t lineno;
extern uint64_t column;
extern bool commandMode;
extern bool interactive;
void AstExecute(fsh::Ast *pAst);
 
extern void yyerror(const char *s);

typedef fsh::Ast ast_t;
typedef fsh::ASTBinaryOperator bop_t;
//typedef fsh::FunctionCall call_t;
//typedef fsh::FunctionDef fdef_t;
//typedef fsh::WhileIf wif_t;
typedef fsh::ASTExpressionList el_t;
//typedef fsh::IdentifierList il_t;
//typedef fsh::Identifier iden_t;
//typedef fsh::String str_t;
//typedef fsh::Boolean bool_t;
//typedef fsh::Integer int_t;
//typedef fsh::Float float_t;
//typedef fsh::None none_t;
//typedef fsh::TryCatch trycatch_t;
//typedef fsh::DotOperator dot_t;
//typedef fsh::For for_t;
//typedef fsh::Attribute attr_t;
//typedef fsh::System system_t;
%}

%define api.value.type {void *} // Actually is Ast

// define the constant-string tokens:
%token IDENTIFIER INTEGER FLOAT STRING_LITERAL
%token NONE TRUE FALSE
%token IF WHILE THEN ELSE
%token TRY CATCH
%token FOR IN
%token DOUBLE_BRACKET_OPEN
%token DOUBLE_BRACKET_CLOSE
%token RIGHT_ARROW
%token SYSTEM
%token CMD_WORD
%token CMD_BAR
%token CMD_LT
%token CMD_GT
%token CMD_AMP
%token CMD_GTGT
%token CMD_SEMI
%token CMD_NL
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
        ast_t *pAst = (ast_t *)$1;
        AstExecute(pAst);
    }
    |CMD_WORD {fsh::PushWord((char *)$1);}
    |CMD_BAR {fsh::PushBar();}
    |CMD_LT {fsh::PushLT();}
    |CMD_GT {fsh::PushGT();}
    |CMD_GTGT {fsh::PushGTGT();}
    |CMD_AMP {fsh::PushAmp();}
    |CMD_SEMI {fsh::PushSemi();}
    |CMD_NL {fsh::PushNL();
            std::cout << "\r> ";
            }
    ;

primary_expression
    : IDENTIFIER            { $$ = $1;}
    | INTEGER               {$$ = $1;}
    | FLOAT                 {$$ = $1;}
    | STRING_LITERAL        {$$ = $1;}
    | NONE                  {
                                fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
                                bc->ctype = fsh::ASTConstant::CTYPE_NONE;
                                $$ = bc;
                            }
    | TRUE                  {
                                fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
                                bc->ctype = fsh::ASTConstant::CTYPE_TRUE;
                                $$ = bc;
                            }
    | FALSE                 {
                                fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
                                bc->ctype = fsh::ASTConstant::CTYPE_FALSE;
                                $$ = bc;
                            }
    //| SYSTEM                {$$ = $1;}
    | '(' expression_list ')'    {$$ = $2;}
    | '{' expression_list '}' { 
        //el_t *el = (el_t *)$2;
        //el->isList = true;
        $$ = $2; 
    }
    | '{' '}' { 
        //el_t *el = new el_t(lineno);
        //el->isList = true;
        //$$ = el;
    }
    ;

assignment_expression
    : IDENTIFIER '=' expression {
        fsh::ASTAssignment *ass = new fsh::ASTAssignment(lineno);
        ass->rhs.reset((ast_t *)$3);
        ass->lhs.reset((ast_t *)$1);
        $$ = ass;
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
    | dot_expression            {$$ = $1;}
    | for_expression            {$$ = $1;}
    | subscript_expression      {$$ = $1;}
    ;

relational_expression
    :expression LT expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_LT;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    |expression LTE expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_LTE;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    |expression GT expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_GT;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    |expression GTE expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_GTE;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    |expression EQ expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_EQ;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    |expression NEQ expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_NEQ;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    |expression AND expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_LOGICAL_AND;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    |expression OR expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_LOGICAL_OR;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    ;

math_expression
    :expression '+' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_ADD;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    | expression '-' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_SUBTRACT;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    | expression '*' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_MULTIPLY;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    | expression '/' expression {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_DIVIDE;
        bop->lhs.reset((ast_t *)$1);
        bop->rhs.reset((ast_t *)$3);
        $$ = bop;
    }
    | '-' expression %prec NEG {
        fsh::ASTUnaryOperator *uo= new fsh::ASTUnaryOperator(lineno);
        uo->op = fsh::BC_UNARY_NEGATE;
        uo->operand.reset((ast_t *)$2);
        $$ = uo;
    }
    ;

attribute_expression
    : IDENTIFIER RIGHT_ARROW expression {
        //attr_t *pAttr = new attr_t(lineno);
        //pAttr->name = (inst_t *)$1);
        //pAttr->value = (inst_t *)$3);
        //$$ = pAttr;
    }
    ;

subscript_expression
    : primary_expression DOUBLE_BRACKET_OPEN  expression ':' expression DOUBLE_BRACKET_CLOSE {
        //call_t *pCall = new call_t(lineno);
        //pCall->call = new iden_t(lineno, "Subscript");
        //el_t *el = new fsh::instruction::ExpressionList(lineno);
        //el->expressions.push_back((inst_t *)$1);
        //el->expressions.push_back((inst_t *)$3);
        //el->expressions.push_back(new str_t(lineno, ":"));
        //el->expressions.push_back((inst_t *)$5);
        //pCall->functionArguments = el;
        ////std::cout << "func call " << id->name << std::endl;
        //$$ = pCall;
    }
    | primary_expression DOUBLE_BRACKET_OPEN  expression ':' DOUBLE_BRACKET_CLOSE {
        //call_t *pCall = new call_t(lineno);
        //pCall->call = new iden_t(lineno, "Subscript");
        //el_t *el = new fsh::instruction::ExpressionList(lineno);
        //el->expressions.push_back((inst_t *)$1);
        //el->expressions.push_back((inst_t *)$3);
        //el->expressions.push_back(new str_t(lineno, ":"));
        //pCall->functionArguments = el;
        //std::cout << "func call " << id->name << std::endl;
        //$$ = pCall;
    }
    | primary_expression DOUBLE_BRACKET_OPEN  expression DOUBLE_BRACKET_CLOSE {
        //call_t *pCall = new call_t(lineno);
        //pCall->call = new iden_t(lineno, "Subscript");
        //el_t *el = new fsh::instruction::ExpressionList(lineno);
        //el->expressions.push_back((inst_t *)$1);
        //el->expressions.push_back((inst_t *)$3);
        //pCall->functionArguments = el;
        ////std::cout << "func call " << id->name << std::endl;
        //$$ = pCall;
    }
    | primary_expression DOUBLE_BRACKET_OPEN  ':' expression DOUBLE_BRACKET_CLOSE {
        //call_t *pCall = new call_t(lineno);
        //pCall->call = new iden_t(lineno, "Subscript");
        //el_t *el = new fsh::instruction::ExpressionList(lineno);
        //el->expressions.push_back((inst_t *)$1);
        //el->expressions.push_back(new str_t(lineno, ":"));
        //el->expressions.push_back((inst_t *)$4);
        //pCall->functionArguments = el;
        ////std::cout << "func call " << id->name << std::endl;
        //$$ = pCall;
    }
    ;

dot_expression
    : IDENTIFIER '.' expression {
        //dot_t *dop = new dot_t(lineno);
        //dop->lhs = (inst_t *)$1;
        //dop->rhs = (inst_t *)$3;
        //$$ = dop;
    }
    ;

exception_expression
    : TRY '[' statement_list  CATCH statement_list  ']'  {
        //trycatch_t *pException = new trycatch_t(lineno);
        //pException->try_ = (inst_t *)$3;
        //pException->catch_ = (inst_t *)$5;
        //$$ = pException;
    }
    ;

selection_expression
    : IF '[' expression_list  THEN statement_list  ELSE statement_list  ']'  {
        fsh::ASTIfStatement *pIf = new fsh::ASTIfStatement(lineno);
        pIf->condition.reset((ast_t *)$3);
        pIf->if_true.reset((ast_t *)$5);
        pIf->if_false.reset((ast_t *)$7);
        $$ = pIf;
    }
    ;

for_expression
    : FOR '[' IDENTIFIER IN expression THEN statement_list ']'  {
        //for_t *pFor = new for_t(lineno);
        //pFor->identifier = (inst_t *)$3;
        //pFor->list = (inst_t *)$5;
        //pFor->body = (inst_t *)$7;
        //$$ = pFor;
    }
    ;

iteration_expression
    : WHILE '[' expression_list  THEN statement_list ']'  {
        fsh::ASTWhile *pWhileIf = new fsh::ASTWhile(lineno);
        pWhileIf->condition.reset((ast_t *)$3);
        pWhileIf->if_true.reset((ast_t *)$5);
        $$ = pWhileIf;
    }
    ;

function_definition
    : '&' '[' identifier_list ':' statement_list ']' {
        fsh::ASTFunctionDef *pDef = new fsh::ASTFunctionDef(lineno);
        ast_t *arg3 = (ast_t *)$3;
        assert(arg3->type() == fsh::AST_CONSTANT
            || arg3->type() == fsh::AST_IDENTIFIER_LIST);
        if (arg3->type() == fsh::AST_CONSTANT)
        {
            fsh::ASTConstant * id = (fsh::ASTConstant *)$3;
            assert(id->ctype == fsh::ASTConstant::CTYPE_IDENTIFIER);
            pDef->arg_names.push_back(id->svalue);
            delete id;
        }
        else
        {
            fsh::ASTIdentifierList *il = (fsh::ASTIdentifierList *)$3;
            for (auto& inst : il->identifiers)
            {
                if (inst->type() == fsh::AST_CONSTANT)
                {
                    fsh::ASTConstant *id = (fsh::ASTConstant *)inst.get();
                    assert(id->ctype == fsh::ASTConstant::CTYPE_IDENTIFIER);
                    pDef->arg_names.push_back(id->svalue);
                }
            }
            delete il;
        }
        pDef->functionBody.reset((fsh::Ast *)$5);
        $$ = pDef;
    }
    |'&' '[' statement_list ']' {
        fsh::ASTFunctionDef *pDef = new fsh::ASTFunctionDef(lineno);
        pDef->functionBody.reset((fsh::Ast *)$3);
        $$ = pDef;
    }
    ;

function_call
    : primary_expression '[' call_expression_list ']' {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset((ast_t *)$1);
        pCall->arguments.reset((ast_t *)$3);
        ////std::cout << "func call " << id->name << std::endl;
        $$ = pCall;
    }
    | primary_expression '[' ']' {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset((ast_t *)$1);
        ////std::cout << "func call  " << id->name << std::endl;
        $$ = pCall;
    }
    ;

identifier_list
    :IDENTIFIER {
        $$ = $1;
    }
    |identifier_list ',' IDENTIFIER  {
        assert(((ast_t *)$3)->type() == fsh::AST_CONSTANT);
        fsh::ASTConstant *arg3 = (fsh::ASTConstant *)$3;
        ast_t *ast = (ast_t *)$1;
        if (ast->type() == fsh::AST_IDENTIFIER_LIST)
        {
            fsh::ASTIdentifierList *il = (fsh::ASTIdentifierList *)$1;
            il->identifiers.emplace_back(arg3);
            $$ = il;
        }
        else
        {
            assert(((ast_t *)$1)->type() == fsh::AST_CONSTANT);
            fsh::ASTIdentifierList *il = new fsh::ASTIdentifierList(lineno);
            il->identifiers.emplace_back((fsh::ASTConstant *)$1);
            il->identifiers.emplace_back(arg3);
            $$ = il;
        }
    }
    ;

expression_list
    :expression {
        el_t *el = new el_t(lineno);
        el->expressions.emplace_back((ast_t *)$1);
        $$ = el;
    }
    |expression_list ',' expression  {
        ast_t *ast = (ast_t *)$1;
        assert (ast->type() == fsh::AST_EXPRESSION_LIST);
        el_t *el = (el_t *)$1;
        el->expressions.emplace_back((ast_t *)$3);
        $$ = el;
    }
    ;

attribute_list
    :attribute_expression {
        //el_t *el = new el_t(lineno);
        //el->expressions.push_back((inst_t *)$1);
        //$$ = el;
    }
    | attribute_list ',' attribute_expression  {
        //inst_t *ip = (inst_t *)$1;
        //assert (ip->type() == fsh::instruction::INSTRUCTION_EXPRESSION_LIST);
        //el_t *el = (el_t *)$1;
        //el->expressions.push_back((inst_t *)$3);
        //$$ = el;
    }
    ;

call_expression_list
    :expression {
        fsh::ASTExpressionList *el = new fsh::ASTExpressionList(lineno);
        el->expressions.emplace_back((ast_t *)$1);
        $$ = el;
    }
    | attribute_list {
        //el_t *el = new el_t(lineno);
        //el->expressions.push_back((inst_t *)$1);
        //$$ = el;
    }
    | expression_list ',' expression  {
        ast_t *ip = (ast_t *)$1;
        assert (ip->type() == fsh::AST_EXPRESSION_LIST);
        fsh::ASTExpressionList *el = (el_t *)$1;
        el->expressions.emplace_back((ast_t *)$3);
        $$ = el;
    }
    | expression_list ',' attribute_list {
        //inst_t *ip = (inst_t *)$1;
        //assert (ip->type() == fsh::instruction::INSTRUCTION_EXPRESSION_LIST);
        //el_t *el = (el_t *)$1;
        //el_t *el_attr = (el_t *)$3;
        //for (auto e : el_attr->expressions)
        //{
            //el->expressions.push_back((inst_t *)$3);
        //}
        ////delete el_attr;
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
        ast_t *arg1 = (ast_t *)$1;
        fsh::ASTExpressionList *el = nullptr;
        if (arg1->type() != fsh::AST_EXPRESSION_LIST)
        {
            el = new fsh::ASTExpressionList(lineno);
            el->expressions.emplace_back(arg1);
        }
        else
        {
            el = (fsh::ASTExpressionList *)arg1;
        }
        el->expressions.emplace_back((ast_t *)$2);
        $$ = el;
    }
    ;

/* End of grammar. */
%%

extern fsh::Machine machine;

void AstExecute(fsh::Ast *pAst)
{
    pAst->GenerateCode(machine, machine.get_byte_code());
    fsh::ElementPtr e = machine.Execute();
    if(e->type() == fsh::ELEMENT_TYPE_ERROR)
    {
        fsh::ErrorPtr ep = e.cast<fsh::Error>();
        if (ep->bOk == false)
            std::cout << ep->msg << std::endl;
    }
    if (interactive)
        std::cout << fsh::toString(machine, e) << std::endl;
}



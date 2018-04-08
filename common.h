#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include <stdexcept>
#include "instrusive_ptr.h"
#include "element.h"
#include "bytecode.h"

namespace fsh
{
    enum ByteCodeType
    {
        BC_LOAD_INTEGER
        ,BC_LOAD_NONE
        ,BC_LOAD_FLOAT
        ,BC_LOAD_STRING
        ,BC_LOAD_IDENTIFIER
        ,BC_LOAD_TRUE
        ,BC_LOAD_FALSE
        ,BC_STORE_VAR
        ,BC_BINARY_ADD
        ,BC_BINARY_SUBTRACT
        ,BC_BINARY_MULTIPLY
        ,BC_BINARY_DIVIDE
        ,BC_LOGICAL_AND
        ,BC_LOGICAL_OR
        ,BC_RELATIONAL_GT
        ,BC_RELATIONAL_LT
        ,BC_RELATIONAL_GTE
        ,BC_RELATIONAL_LTE
        ,BC_RELATIONAL_EQ
        ,BC_RELATIONAL_NEQ
        ,BC_UNARY_NEGATE
        ,BC_JUMP_IF_FALSE
        ,BC_JUMP
        ,BC_CALL
        ,BC_LOAD_FUNCTION_DEF
    };

    enum AstType
    {
        AST_NONE
        ,AST_CONSTANT
        ,AST_ASSIGNMENT
        ,AST_BINARY_OPERATOR
        ,AST_UNARY_OPERATOR
        ,AST_EXPRESSION_LIST
        ,AST_IDENTIFIER_LIST
        ,AST_IF
        ,AST_WHILE
        ,AST_FUNCTION_CALL
        ,AST_FUNCTION_DEF
    };

    struct FunctionDefinition
    {
        FunctionDefinition()
        :isBuiltIn(false)
        {}
        std::vector<std::string> arg_names;
        ByteCode shellFunction;
        std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)> builtIn;
        bool isBuiltIn;
    };
}
#include "ast.h"

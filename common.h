#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include "instrusive_ptr.h"
#include "element.h"

namespace fsh
{
    enum TokenType
    {
        TOKEN_NONE
        ,TOKEN_UNKNOWN
        ,TOKEN_GT
        ,TOKEN_GTE
        ,TOKEN_LT
        ,TOKEN_LTE
        ,TOKEN_EQ
        ,TOKEN_NEQ
        ,TOKEN_NOT
        // For the OPEN and CLOSE pairs, CLOSE must occur after the OPEN
        ,TOKEN_PAREN
        ,TOKEN_BRACKET
        ,TOKEN_SUBSCRIPT
        ,TOKEN_BRACE
        ,TOKEN_DBL_QUOTE
        ,TOKEN_VERTICLE_BAR
        ,TOKEN_IDENTIFIER
        ,TOKEN_INTEGER
        ,TOKEN_ASSIGNMENT
        ,TOKEN_COMMA
        ,TOKEN_COLON
        ,TOKEN_PLUS
        ,TOKEN_MINUS
        ,TOKEN_MULTIPLY
        ,TOKEN_DIVIDE
        ,TOKEN_MOD
        ,TOKEN_DEFINE
        ,TOKEN_AMP
        ,TOKEN_AND
        ,TOKEN_OR
        ,TOKEN_NL
        ,TOKEN_SEQUENCE
    };

    struct Token
    {
        Token()
        :token(TOKEN_NONE)
        {}
        TokenType token;
        std::string text;
    };

    namespace instruction
    {
        enum InstructionType
        {
            INSTRUCTION_NONE
            ,INSTRUCTION_IDENTIFIER
            ,INSTRUCTION_INTEGER
            ,INSTRUCTION_FLOAT
            ,INSTRUCTION_BINARY_OPERATOR
            ,INSTRUCTION_FUNCTION_CALL
            ,INSTRUCTION_FUNCTION_DEF
            ,INSTRUCTION_EXPRESSION_LIST
            ,INSTRUCTION_IDENTIFIER_LIST
            ,INSTRUCTION_STRING
        };
    }

}
#include "instructions.h"

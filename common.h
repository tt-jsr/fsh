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
        ,TOKEN_GT
        ,TOKEN_GTE
        ,TOKEN_LT
        ,TOKEN_LTE
        ,TOKEN_EQ
        ,TOKEN_NEQ
        // For the OPEN and CLOSE pairs, CLOSE must occur after the OPEN
        ,TOKEN_OPEN_PAREN
        ,TOKEN_CLOSE_PAREN
        ,TOKEN_OPEN_BRACKET
        ,TOKEN_CLOSE_BRACKET
        ,TOKEN_OPEN_SUBSCRIPT
        ,TOKEN_CLOSE_SUBSCRIPT
        ,TOKEN_OPEN_BRACE
        ,TOKEN_CLOSE_BRACE
        ,TOKEN_VERTICLE_BAR
        ,TOKEN_DOLLAR_GT
        ,TOKEN_AND
        ,TOKEN_OR
        ,TOKEN_IN
        ,TOKEN_IDENTIFIER
        ,TOKEN_INT
        ,TOKEN_ASSIGN
        ,TOKEN_COMMA
        ,TOKEN_PLUS
        ,TOKEN_MINUS
        ,TOKEN_MULTIPLY
        ,TOKEN_DIVIDE
        ,TOKEN_MOD
    };

    namespace instruction
    {
        enum InstructionType
        {
            INSTRUCTION_NONE
            ,INSTRUCTION_IDENTIFIER
            ,INSTRUCTION_INTEGER
            ,INSTRUCTION_BINARY_OPERATOR
        };
    }
}

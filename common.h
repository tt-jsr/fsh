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
        // Load integer immediate. 
        // The integer is pushed onto the stack
        // The next location contains the int
        BC_LOAD_INTEGER

        // Push None onto the stack
        ,BC_LOAD_NONE

        // Load float immediate.
        // The float ispushed onto the stack
        // The next location contains the float
        ,BC_LOAD_FLOAT

        // Load string.
        // The string is pushed onto the stack
        // The next location contains an int. The value
        // is the key into the machine's string table.
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
        // Pops TOS. If the value is false (as defined by Machine::ConvertToBool())
        // jump to the location specified by the next location
        ,BC_JUMP_IF_FALSE

        // Checks Machine.get_gp_register(), and if false, jumps to the location 
        // specified in the next location
        ,BC_JUMP_GP_FALSE

        // Jump to the location specified in the next location
        ,BC_JUMP
        ,BC_CALL
        ,BC_LOAD_FUNCTION_DEF
        ,BC_SYSTEM

        // An index is popped off the stack
        // A list is popped off the stack
        // The index is used to get an item from the
        // list, witch is pushed onto the stack
        // If index is out of bounds, the machine register
        // gp_register is set to false, and nothing is pushed
        ,BC_LOAD_LIST_ITEM

        // pops the TOS, performs any variable lookup, and pushes the result
        ,BC_RESOLVE

        // Increment an integer
        // The next location contains the location
        // of the integer to be incremented.
        // No value is pushed onto the stack
        ,BC_INCREMENT_LOCATION

        // Load an integer from a location
        // The  next location contains the location
        // of the integer to be pushed onto the stack
        ,BC_LOAD_INTEGER_LOCATION

        // Does not contain any code. The next location is reserved
        // to store a value for use by the bytecode
        ,BC_DATA

        ,BC_PUSH_CONTEXT

        ,BC_POP
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
        ,AST_FOR
        ,AST_FUNCTION_CALL
        ,AST_FUNCTION_DEF
        ,AST_SYSTEM
        ,AST_SUBSCRIPT
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

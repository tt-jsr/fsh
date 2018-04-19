#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include <stdexcept>
#include <functional>
#include "instrusive_ptr.h"
#include "element.h"
#include "bytecode.h"

namespace fsh
{
    enum ByteCodeType
    {
        // Load integer immediate. 
        // The integer is pushed onto the stack
        // The ip+1 contains the int
        BC_LOAD_INTEGER

        // Push None onto the stack
        ,BC_LOAD_NONE

        // Push boolean constants
        ,BC_LOAD_TRUE
        ,BC_LOAD_FALSE

        // Load float immediate.
        // The float is pushed onto the stack
        // ip+1 contains the float
        ,BC_LOAD_FLOAT

        // Load string.
        // The string is pushed onto the stack
        // ip+1 contains an int. The value
        // is the key into the machine's string table.
        ,BC_LOAD_STRING

        // Load identifier.
        // The identifier is pushed onto the stack
        // ip+1 contains an int. The value
        // is the key into the machine's string table.
        ,BC_LOAD_IDENTIFIER

        // pops the identifer
        // pops the value to be stored
        // pushes the value stored
        ,BC_STORE_VAR

        // BINARY, LOGICAL and RELATIONAL operators all
        // pop lhs
        // pop rhs
        // perform operation 
        // pushes result
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

        // pops TOS, and pushes the negation of the value
        ,BC_UNARY_NEGATE

        // Pops TOS. If the value is false (as defined by Machine::ConvertToBool())
        // jump to the location specified by ip+1
        ,BC_JUMP_IF_FALSE

        // Checks Machine.get_gp_register(), and if false, jumps to the location 
        // specified in ip+1
        // See BC_LOAD_LIST_ITEM
        ,BC_JUMP_GP_FALSE

        // Jump to the location specified in ip+1
        ,BC_JUMP

        // Call a function
        // 1. The function definition is popped from the stack
        // 2. An Integer is then popped from the stack. This is the number of
        // arguments to the function.
        // 3. All arguments are popped from the stack
        // 4. The function is called
        // 5. The return from the function is pushed
        ,BC_CALL

        // Load function definition.
        // The function def is pushed onto the stack
        // ip+1 contains an int. The value
        // is the key into the machine's function table.
        ,BC_LOAD_FUNCTION_DEF

        // Execution system command line
        // ip+1 contains a string id. The value
        // is the key into the machine's string table.
        // The command is executed using the system(3) call
        // None is pushed
        ,BC_SYSTEM

        // An index is popped off the stack
        // A list is popped off the stack
        // The index is used to get an item from the
        // list, which is pushed onto the stack
        // If index is out of bounds, the machine register
        // gp_register is set to false, and nothing is pushed
        ,BC_LOAD_LIST_ITEM

        // pops the TOS, performs any variable lookup, and pushes the result
        ,BC_RESOLVE

        // Increment an integer
        // ip+1 contains the location
        // of the integer to be incremented.
        // No value is pushed or popped onto the stack
        ,BC_INCREMENT_LOCATION

        // Load an integer from a location
        // ip+1 contains the location
        // of the integer to be pushed onto the stack
        ,BC_LOAD_INTEGER_LOCATION

        // Does not contain any code. ip+1 is reserved
        // to store a value for use by the bytecode
        ,BC_DATA

        // calls machine.push_context(). No data is pushed or popped fromn the stack
        ,BC_PUSH_CONTEXT

        // pops and discards the TOS
        ,BC_POP

        ,BC_TRY

        ,BC_BIND
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
        ,AST_TRY_CATCH
        ,AST_BIND
    };

    struct FunctionDefinition
    {
        ElementPtr Call(Machine& machine, int64_t nArgsOnStack);
        virtual ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args) = 0;
    };
    
    struct BuiltInFunction : public FunctionDefinition
    {
        ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args);

        std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)> builtIn;
    };

    struct ShellFunction : public FunctionDefinition
    {
        ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args);

        std::vector<std::string> arg_names;
        ByteCode shellFunction;
    };

    struct BoundFunction : public FunctionDefinition
    {
        ElementPtr CallImpl(Machine&, std::vector<ElementPtr>& args);

        FunctionDefinition *target;
        std::vector<ElementPtr> bound_args;
        ByteCode attributes;
    };
}

#include "ast.h"

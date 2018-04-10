#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "common.h"
#include "common.h"
#include "ast.h"
#include "machine.h"
#include "builtins.h"

namespace fsh
{
    /*************************************************/
    ASTBinaryOperator::~ASTBinaryOperator()
    {
    }

    void ASTBinaryOperator::GenerateCode(Machine& machine, ByteCode& bc)
    {
        rhs->GenerateCode(machine, bc);
        lhs->GenerateCode(machine, bc);
        bc.code(op);
    }

    /*****************************************************/
    void ASTConstant::GenerateCode(Machine& machine, ByteCode& bc)
    {
        switch(ctype)
        {
        case CTYPE_INTEGER:
            bc.code(BC_LOAD_INTEGER, ivalue);
            break;
        case CTYPE_FLOAT:
            bc.code(BC_LOAD_FLOAT, *(int64_t *)&dvalue);
            break;
        case CTYPE_STRING:
            {
                int64_t id = machine.string_table_add(svalue);
                bc.code(BC_LOAD_STRING, id);
            }
            break;
        case CTYPE_IDENTIFIER:
            {
                int64_t id = machine.string_table_add(svalue);
                bc.code(BC_LOAD_IDENTIFIER, id);
            }
            break;
        case CTYPE_NONE:
            bc.code(BC_LOAD_NONE);
            break;
        case CTYPE_TRUE:
            bc.code(BC_LOAD_TRUE);
            break;
        case CTYPE_FALSE:
            bc.code(BC_LOAD_FALSE);
            break;
        default:
            assert(false);
            break;;
        }
    }

    /*****************************************************/
    void ASTUnaryOperator::GenerateCode(Machine& machine, ByteCode& bc)
    {
        operand->GenerateCode(machine, bc);
        bc.code(op);
    }

    /*****************************************************/
    void ASTAssignment::GenerateCode(Machine& machine, ByteCode& bc)
    {
        rhs->GenerateCode(machine, bc);
        lhs->GenerateCode(machine, bc);
        bc.code(BC_STORE_VAR);
    }

    /*****************************************************/
    void ASTExpressionList::GenerateCode(Machine& machine, ByteCode& bc)
    {
        for (auto& e : expressions)
        {
            e->GenerateCode(machine, bc);
        }
    }

    /*****************************************************/
    void ASTIdentifierList::GenerateCode(Machine& machine, ByteCode& bc)
    {
        for (auto& e : identifiers)
        {
            e->GenerateCode(machine, bc);
        }
    }

    /*****************************************************/
    void ASTIfStatement::GenerateCode(Machine& machine, ByteCode& bc)
    {
        condition->GenerateCode(machine, bc);
        size_t jump_else = bc.jump_forward(BC_JUMP_IF_FALSE);
        if_true->GenerateCode(machine, bc);
        size_t jump_end = bc.jump_forward(BC_JUMP);
        bc.set_jump_location(jump_else); // Actually point to the byte code before the one we want to execute
        if_false->GenerateCode(machine, bc);
        bc.set_jump_location(jump_end);
    }

    /*****************************************************/
    void ASTWhile::GenerateCode(Machine& machine, ByteCode& bc)
    {
        size_t begin = bc.current_location();
        condition->GenerateCode(machine, bc);
        size_t jump_end = bc.jump_forward(BC_JUMP_IF_FALSE);
        if_true->GenerateCode(machine, bc);
        bc.jump_to(BC_JUMP, begin);
        bc.set_jump_location(jump_end);
    }


    /*****************************************************/
    void ASTFor::GenerateCode(Machine& machine, ByteCode& bc)
    {
        // Allocate a spot for the  list iterator
        size_t iterator = bc.code(BC_DATA, 0);

        size_t begin = bc.current_location();

        // The list
        list->GenerateCode(machine, bc);
        // Resolve the TOS so the list gets put on the stack
        bc.code(BC_RESOLVE);
        bc.code(BC_LOAD_INTEGER_LOCATION, iterator);
        bc.code(BC_LOAD_LIST_ITEM);
        size_t jump_end = bc.jump_forward(BC_JUMP_GP_FALSE);

        // The variable name to store the current list item
        identifier->GenerateCode(machine, bc);
        bc.code(BC_STORE_VAR);

        body->GenerateCode(machine, bc);
        bc.code(BC_INCREMENT_LOCATION, iterator);
        bc.jump_to(BC_JUMP, begin);
        bc.set_jump_location(jump_end);
    }

    /***************************************************/
    void ASTFunctionCall::GenerateCode(Machine& machine, ByteCode& bc)
    {
        ASTExpressionList *el = (ASTExpressionList *)arguments.get();
        if (arguments)
        {
            arguments->GenerateCode(machine, bc);
            bc.code(BC_LOAD_INTEGER, el->expressions.size());
        }
        else
        {
            bc.code(BC_LOAD_INTEGER, 0);
        }
        call->GenerateCode(machine, bc);
        bc.code(BC_CALL);
    }

    /***************************************************/
    void ASTFunctionDef::GenerateCode(Machine& machine, ByteCode& bc)
    {
        FunctionDefinition fd;
        functionBody->GenerateCode(machine, fd.shellFunction);
        fd.arg_names = arg_names;
        fd.isBuiltIn = false;
        int64_t id = machine.registerFunction(fd);

        bc.code(BC_LOAD_FUNCTION_DEF, id);
    }

    /***************************************************/
    void ASTSystem::GenerateCode(Machine& machine, ByteCode& bc)
    {
        int64_t id = machine.string_table_add(cmd);
        bc.code(BC_SYSTEM, id);
    }
 }


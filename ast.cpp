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
    ASTBinaryOperator::~ASTBinaryOperator()
    {
    }

    void ASTBinaryOperator::GenerateCode(Machine& machine, ByteCode& bc)
    {
        rhs->GenerateCode(machine, bc);
        lhs->GenerateCode(machine, bc);
        bc.push_back(op);
    }

    /*****************************************************/
    void ASTConstant::GenerateCode(Machine& machine, ByteCode& bc)
    {
        switch(ctype)
        {
        case CTYPE_INTEGER:
            bc.push_back(BC_LOAD_INTEGER);
            bc.push_back(ivalue);
            break;
        case CTYPE_FLOAT:
            bc.push_back(BC_LOAD_FLOAT);
            bc.push_back(*(int64_t *)&dvalue);
            break;
        case CTYPE_STRING:
            {
                int64_t id = machine.string_table_add(svalue);
                bc.push_back(BC_LOAD_STRING);
                bc.push_back(id);
            }
            break;
        case CTYPE_IDENTIFIER:
            {
                int64_t id = machine.string_table_add(svalue);
                bc.push_back(BC_LOAD_IDENTIFIER);
                bc.push_back(id);
            }
            break;
        case CTYPE_NONE:
            bc.push_back(BC_LOAD_NONE);
            break;
        case CTYPE_TRUE:
            bc.push_back(BC_LOAD_TRUE);
            break;
        case CTYPE_FALSE:
            bc.push_back(BC_LOAD_FALSE);
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
        bc.push_back(op);
    }

    /*****************************************************/
    void ASTAssignment::GenerateCode(Machine& machine, ByteCode& bc)
    {
        rhs->GenerateCode(machine, bc);
        lhs->GenerateCode(machine, bc);
        bc.push_back(BC_STORE_VAR);
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
        size_t jump_else = bc.push_jump(BC_JUMP_IF_FALSE);
        if_true->GenerateCode(machine, bc);
        size_t jump_end = bc.push_jump(BC_JUMP);
        bc.setToCurrent(jump_else); // Actually point to the byte code before the one we want to execute
        if_false->GenerateCode(machine, bc);
        bc.setToCurrent(jump_end);
    }

    /*****************************************************/
    void ASTWhile::GenerateCode(Machine& machine, ByteCode& bc)
    {
        size_t begin = bc.size()-1;
        condition->GenerateCode(machine, bc);
        size_t jump_end = bc.push_jump(BC_JUMP_IF_FALSE);
        if_true->GenerateCode(machine, bc);
        bc.push_jump(BC_JUMP, begin);
        bc.setToCurrent(jump_end);
    }

    /***************************************************/
    void ASTFunctionCall::GenerateCode(Machine& machine, ByteCode& bc)
    {
        ASTExpressionList *el = (ASTExpressionList *)arguments.get();
        if (arguments)
        {
            arguments->GenerateCode(machine, bc);
            bc.push_back(BC_LOAD_INTEGER);
            bc.push_back(el->expressions.size());
        }
        else
        {
            bc.push_back(BC_LOAD_INTEGER);
            bc.push_back(0);
        }
        call->GenerateCode(machine, bc);
        bc.push_back(BC_CALL);
    }

    /***************************************************/
    void ASTFunctionDef::GenerateCode(Machine& machine, ByteCode& bc)
    {
        FunctionDefinition fd;
        functionBody->GenerateCode(machine, fd.shellFunction);
        fd.arg_names = arg_names;
        fd.isBuiltIn = false;
        int64_t id = machine.registerFunction(fd);

        bc.push_back(BC_LOAD_FUNCTION_DEF);
        bc.push_back(id);
    }
 }


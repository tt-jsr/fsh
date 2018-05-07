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
            bc.code(BC_LOAD_FLOAT, *(uintptr_t *)&dvalue);
            break;
        case CTYPE_STRING:
            {
                std::string *ps = new std::string();
                *ps = svalue;
                bc.code(BC_LOAD_STRING, (uintptr_t)ps);
            }
            break;
        case CTYPE_IDENTIFIER:
            {
                std::string *ps = new std::string();
                *ps = svalue;
                bc.code(BC_LOAD_IDENTIFIER, (uintptr_t)ps);
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

    ASTConstant *ASTMakeIntegerConstant(size_t lineno, uintptr_t n)
    {
        ASTConstant *p = new ASTConstant(lineno);
        p->ctype = ASTConstant::CTYPE_INTEGER;
        p->ivalue = n;
        return p;
    }

    ASTConstant *ASTMakeBooleanConstant(size_t lineno, bool b)
    {
        ASTConstant *p = new ASTConstant(lineno);
        if (b)
            p->ctype = ASTConstant::CTYPE_TRUE;
        else
            p->ctype = ASTConstant::CTYPE_FALSE;
        return p;
    }

    ASTConstant *ASTMakeNoneConstant(size_t lineno)
    {
        ASTConstant *p = new ASTConstant(lineno);
        p->ctype = ASTConstant::CTYPE_NONE;
        return p;
    }

    ASTConstant *ASTMakeIdentifierConstant(size_t lineno, const std::string& s)
    {
        ASTConstant *p = new ASTConstant(lineno);
        p->ctype = ASTConstant::CTYPE_IDENTIFIER;
        p->svalue = s;
        return p;
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
        bc.code(BC_RESOLVE);
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
        // Allocate a spot for the iterator key
        size_t iterator = bc.code(BC_DATA, 0);

        bc.code(BC_DELETE_ITERATOR, iterator);
        size_t begin = bc.current_location();

        // The container
        list->GenerateCode(machine, bc);

        // Resolve the TOS so the list gets put on the stack
        bc.code(BC_RESOLVE);
        bc.code(BC_INCREMENT_ITERATOR, iterator);
        bc.code(BC_LOAD_CONTAINER_ITEM);
        size_t jump_end = bc.jump_forward(BC_JUMP_GP_FALSE);

        // The variable name to store the current container item
        identifier->GenerateCode(machine, bc);
        bc.code(BC_STORE_VAR);

        body->GenerateCode(machine, bc);
        bc.jump_to(BC_JUMP, begin);
        bc.set_jump_location(jump_end);
    }

    /***************************************************/
    void ASTTryCatch::GenerateCode(Machine& machine, ByteCode& bc)
    {
        ByteCode bctry;
        ByteCode bccatch;
        try_block->GenerateCode(machine, bctry);
        uintptr_t try_id = machine.storeBlock(bctry);
        catch_block->GenerateCode(machine, bccatch);
        uintptr_t catch_id = machine.storeBlock(bccatch);
        bc.code(BC_TRY);
        bc.code(try_id);
        bc.code(catch_id);
    }

    /***************************************************/
    void ASTFunctionCall::GenerateCode(Machine& machine, ByteCode& bc)
    {
        ASTExpressionList *el = (ASTExpressionList *)arguments.get();
        if (arguments)
        {
            for (auto& a : arguments->expressions)
            {
                a->GenerateCode(machine, bc);
                bc.code(BC_RESOLVE);
            }
            bc.code(BC_LOAD_INTEGER, el->expressions.size());
        }
        else
        {
            bc.code(BC_LOAD_INTEGER, 0);
        }
        call->GenerateCode(machine, bc);
        bc.code(BC_RESOLVE);
        bc.code(BC_PUSH_CONTEXT);
        if (attributes)
        {
            for (auto& a : attributes->expressions)
            {
                a->GenerateCode(machine, bc);
                bc.code(BC_POP);
            }
        }
        bc.code(BC_CALL);
    }

    /***************************************************/
    void ASTBind::GenerateCode(Machine& machine, ByteCode& bc)
    {
        ASTExpressionList *el = (ASTExpressionList *)arguments.get();
        if (arguments)
        {
            for (auto& a : arguments->expressions)
            {
                a->GenerateCode(machine, bc);
                bc.code(BC_RESOLVE);
            }
            bc.code(BC_LOAD_INTEGER, el->expressions.size());
        }
        else
        {
            bc.code(BC_LOAD_INTEGER, 0);
        }
        function->GenerateCode(machine, bc);
        bc.code(BC_RESOLVE);
        ByteCode *pAttr = nullptr;
        if (attributes)
        {
            pAttr = new ByteCode();
            for (auto& a : attributes->expressions)
            {
                a->GenerateCode(machine, *pAttr);
                pAttr->code(BC_POP);
            }
        }
        bc.code(BC_BIND);
        bc.code((uintptr_t)pAttr);
    }
    /***************************************************/
    void ASTFunctionDef::GenerateCode(Machine& machine, ByteCode& bc)
    {
        ShellFunction *fd = new ShellFunction();
        functionBody->GenerateCode(machine, fd->shellFunction);
        fd->arg_names = arg_names;
        uintptr_t id = machine.registerFunction(fd);

        bc.code(BC_LOAD_FUNCTION_DEF, id);
    }
    /***************************************************/
    void ASTReturn::GenerateCode(Machine& machine, ByteCode& bc)
    {
        if (expression)
        {
            expression->GenerateCode(machine, bc);
            bc.code(BC_RESOLVE);
        }
        else
        {
            bc.code(BC_LOAD_NONE);
        }
        bc.code(BC_RETURN);
    }

    /***************************************************/
    void ASTGlobal::GenerateCode(Machine& machine, ByteCode& bc)
    {
        assert(identifier);
        identifier->GenerateCode(machine, bc);
        bc.code(BC_GLOBAL);
    }

    /***************************************************/
    void ASTSystem::GenerateCode(Machine& machine, ByteCode& bc)
    {
        uintptr_t id = machine.string_table_add(cmd);
        bc.code(BC_SYSTEM, id);
    }
 }


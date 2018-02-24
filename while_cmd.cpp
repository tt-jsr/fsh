#include <vector>
#include <iostream>
#include "common.h"
#include "element.h"
#include "machine.h"

namespace fsh
{
    ElementPtr While(Machine& machine, std::vector<instruction::InstructionPtr>& args)
    {
        if (args.size() != 2)
        {
            throw std::runtime_error("While requires two arguments");
        }
        instruction::InstructionPtr cond = args[0];

        std::vector<instruction::InstructionPtr> *body = nullptr;
        if (args[1]->type() == instruction::INSTRUCTION_IDENTIFIER)
        {
            args[1]->Execute(machine);
            ElementPtr e = machine.pop_data();
            if (!e->IsFunctionDefinition())
                throw std::runtime_error("While requires second argument to be a function");
            if (e->IsFunctionDefinition())
            {
                FunctionDefinitionPtr fdef = e.cast<FunctionDefinition>();
                body = &fdef->shellBody;
            }
        }
        else
        {
            if (args[1]->type() != instruction::INSTRUCTION_FUNCTION_DEF)
                throw std::runtime_error("While requires second argument to be a function");
            instruction::FunctionDefPtr f = args[1].cast<instruction::FunctionDef>();
            body = &f->statements;
        }

        ElementPtr rtn = MakeNone();
        while (true)
        {
            cond->Execute(machine);
            ElementPtr e = machine.pop_data();
            assert(e->IsBool());
            BoolPtr b = e.cast<Bool>();
            if (b->value)
            {
                for (auto& stmt : *body)
                {
                    stmt->Execute(machine);
                    rtn = machine.pop_data();
                }
            }
            else
            {
                return rtn;
            }
        }
    }
}


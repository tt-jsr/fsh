#include <vector>
#include <iostream>
#include "common.h"
#include "element.h"
#include "machine.h"

namespace fsh
{
    ElementPtr If(Machine& machine, std::vector<instruction::InstructionPtr>& args)
    {
        if (args.size() != 3)
        {
            throw std::runtime_error("If requires three arguments");
        }
        instruction::InstructionPtr cond = args[0];

        std::vector<instruction::InstructionPtr> *if_true = nullptr;
        std::vector<instruction::InstructionPtr> *if_false = nullptr;
        if (args[1]->type() == instruction::INSTRUCTION_IDENTIFIER)
        {
            args[1]->Execute(machine);
            ElementPtr e = machine.pop_data();
            if (!e->IsFunctionDefinition())
                throw std::runtime_error("If requires second argument to be a function");
            if (e->IsFunctionDefinition())
            {
                FunctionDefinitionPtr fdef = e.cast<FunctionDefinition>();
                if_true = &fdef->shellBody;
            }
        }
        else
        {
            if (args[1]->type() != instruction::INSTRUCTION_FUNCTION_DEF)
                throw std::runtime_error("If requires second argument to be a function");
            instruction::FunctionDefPtr f = args[1].cast<instruction::FunctionDef>();
            if_true = &f->statements;
        }
        if (args[2]->type() == instruction::INSTRUCTION_IDENTIFIER)
        {
            args[2]->Execute(machine);
            ElementPtr e = machine.pop_data();
            if (!e->IsFunctionDefinition())
                throw std::runtime_error("If requires third argument to be a function");
            if (e->IsFunctionDefinition())
            {
                FunctionDefinitionPtr fdef = e.cast<FunctionDefinition>();
                if_false = &fdef->shellBody;
            }
        }
        else
        {
            if (args[2]->type() != instruction::INSTRUCTION_FUNCTION_DEF)
                throw std::runtime_error("If requires third argument to be a function");
            instruction::FunctionDefPtr f = args[2].cast<instruction::FunctionDef>();
            if_false = &f->statements;
        }


        cond->Execute(machine);
        ElementPtr e = machine.pop_data();
        assert(e->IsBool());
        BoolPtr b = e.cast<Bool>();
        ElementPtr rtn = MakeNone();
        if (b->value)
        {
            for (auto& stmt : *if_true)
            {
                stmt->Execute(machine);
                rtn = machine.pop_data();
            }
        }
        else
        {
            for (auto& stmt : *if_false)
            {
                stmt->Execute(machine);
                rtn = machine.pop_data();
            }
        }

        return rtn;
    }
}


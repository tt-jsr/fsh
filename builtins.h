#pragma once
#include <vector>

namespace fsh
{
    namespace instruction
    {
        class Instruction;
        typedef instrusive_ptr<Instruction> InstructionPtr;
    }

    ElementPtr Print(Machine&, std::vector<instruction::InstructionPtr>&);
    ElementPtr If(Machine&, std::vector<instruction::InstructionPtr>&);
    ElementPtr While(Machine&, std::vector<instruction::InstructionPtr>&);
}

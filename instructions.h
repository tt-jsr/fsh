#pragma once
#include "data.h"

namespace fsh
{
    class Machine;

    class Instruction
    {
    public:
        virtual void Execute(Machine&) = 0;
    };

    class Assignment : Instruction
    {
    public:
        void Execute(Machine&);
        Instruction * lhs;
        Instruction * rhs;
    };

    class FunctionCall
    {
    public:
        void Execute(Machine&);
        std::vector<Instruction *> args;
        std::string name;
    };

    class In
    {
    public:
        void Execute(Machine&);
        std::string var;
        Instruction * list;
    };

    class Identifier
    {
    public:
        Identifier(const std::string& s)
            :name(s)
        { }
        void Execute(Machine&);
        std::string name;
    };

    class Integer
    {
    public:
        Integer(int64_t n)
            :value(n)
        { }
        void Execute(Machine&);
        int64_t value;
    };
}


#pragma once

namespace fsh
{
    class Machine;

    class Instruction
    {
    public:
        virtual void Execute(Machine&) = 0;
    };

    class BinaryOperator : public Instruction
    {
    public:
        BinaryOperator()
        :lhs(nullptr)
        ,rhs(nullptr)
        {}
        void Execute(Machine&);
        int op;
        int prec;
        Instruction *lhs;
        Instruction *rhs;
    };

    class Function : public Instruction
    {
    public:
        Function()
        : body(nullptr)
        { }
        void Execute(Machine&);
        std::vector<std::string> args;
        Instruction *body;
    };

    class Identifier : public Instruction
    {
    public:
        Identifier()
        { }
        void Execute(Machine&);
        std::string name;
    };

    class Integer : public Instruction
    {
    public:
        Integer()
        { }
        void Execute(Machine&);
        int64_t value;
    };
}


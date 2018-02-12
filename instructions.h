#pragma once

namespace fsh
{
    class Machine;

    namespace instruction
    {
        class Instruction
        {
        public:
            virtual void Execute(Machine&) = 0;
            virtual bool lowerPrec(int) {return false;}
            virtual void dump(std::ostream&) = 0;
        };

        class BinaryOperator : public Instruction
        {
        public:
            BinaryOperator()
            :lhs(nullptr)
            ,rhs(nullptr)
            {}
            void Execute(Machine&);
            bool lowerPrec(int n) {return prec < n;}
            void dump(std::ostream&);
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
            void dump(std::ostream&);
            std::vector<std::string> args;
            Instruction *body;
        };

        class Identifier : public Instruction
        {
        public:
            Identifier()
            { }
            void Execute(Machine&);
            void dump(std::ostream&);
            std::string name;
        };

        class Integer : public Instruction
        {
        public:
            Integer()
            { }
            void Execute(Machine&);
            void dump(std::ostream&);
            int64_t value;
        };
    }
}


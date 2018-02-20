#pragma once

namespace fsh
{
    class Machine;

    namespace instruction
    {
        class Instruction : public fsh::instrusive_base
        {
        public:
            virtual ~Instruction(){}
            virtual void Execute(Machine&) = 0;
            virtual instruction::InstructionType type() = 0;
            virtual void dump(std::ostream&) = 0;
        };

        typedef fsh::instrusive_ptr<Instruction> InstructionPtr;

        class BinaryOperator : public Instruction
        {
        public:
            BinaryOperator()
            :lhs(nullptr)
            ,rhs(nullptr)
            {}
            ~BinaryOperator();
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_BINARY_OPERATOR;}
            void dump(std::ostream&);
            int op;
            int prec;
            InstructionPtr lhs;
            InstructionPtr rhs;
        };

        typedef fsh::instrusive_ptr<BinaryOperator> BinaryOperatorPtr;

        /*
        class Function : public Instruction
        {
        public:
            Function()
            : body(nullptr)
            { }
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_BINARY_OPERATOR;}
            void dump(std::ostream&);
            std::vector<std::string> args;
            Instruction *body;
        };
        */

        class Identifier : public Instruction
        {
        public:
            Identifier()
            { }

            Identifier(const char *p)
            :name(p)
            { }

            ~Identifier() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_IDENTIFIER;}
            void dump(std::ostream&);
            std::string name;
        };

        typedef fsh::instrusive_ptr<Identifier> IdentifierPtr;

        class Integer : public Instruction
        {
        public:
            Integer()
            { }

            Integer(int64_t n)
            :value(n)
            {}

            ~Integer() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_INTEGER;}
            void dump(std::ostream&);
            int64_t value;
        };

        typedef fsh::instrusive_ptr<Integer> IntegerPtr;

        class Float : public Instruction
        {
        public:
            Float()
            { }

            Float(double n)
            :value(n)
            {}

            ~Float() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_FLOAT;}
            void dump(std::ostream&);
            double value;
        };

        typedef fsh::instrusive_ptr<Float> FloatPtr;
    }
}


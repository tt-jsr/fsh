#pragma once

namespace fsh
{
    class Machine;

    namespace instruction
    {
        class DumpContext
        {
        public:
            DumpContext(std::ostream& os);
            void inc();
            void dec();
            void indent();
            std::ostream& strm();

            int indent_;
            std::ostream& strm_;
        };

        class Instruction : public fsh::instrusive_base
        {
        public:
            virtual ~Instruction(){}
            virtual void Execute(Machine&) = 0;
            virtual instruction::InstructionType type() = 0;
            virtual std::string type_str() = 0;
            virtual void dump(DumpContext&) = 0;
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
            std::string type_str();
            void dump(DumpContext&);
            int op;
            int prec;
            InstructionPtr lhs;
            InstructionPtr rhs;
        };

        typedef fsh::instrusive_ptr<BinaryOperator> BinaryOperatorPtr;

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
            std::string type_str();
            void dump(DumpContext&);
            std::string name;
        };

        typedef fsh::instrusive_ptr<Identifier> IdentifierPtr;

        class Integer : public Instruction
        {
        public:
            Integer()
            :value(0)
            { }

            Integer(int64_t n)
            :value(n)
            {}

            ~Integer() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_INTEGER;}
            std::string type_str();
            void dump(DumpContext&);
            int64_t value;
        };

        typedef fsh::instrusive_ptr<Integer> IntegerPtr;

        class String : public Instruction
        {
        public:
            String()
            { }

            String(const std::string& s)
            :value(s)
            {}

            ~String() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_STRING;}
            std::string type_str();
            void dump(DumpContext&);
            std::string value;
        };

        typedef fsh::instrusive_ptr<String> StringPtr;

        class None : public Instruction
        {
        public:
            None()
            { }

            ~None() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_NONE;}
            std::string type_str();
            void dump(DumpContext&);
        };

        typedef fsh::instrusive_ptr<None> NonePtr;

        class Boolean : public Instruction
        {
        public:
            Boolean()
            :value(false)
            { }
            Boolean(bool b)
            : value(b)
            { }

            ~Boolean() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_BOOL;}
            std::string type_str();
            void dump(DumpContext&);
            bool value;
        };

        typedef fsh::instrusive_ptr<Boolean> BooleanPtr;

        class Float : public Instruction
        {
        public:
            Float()
            :value(0.0)
            { }

            Float(double n)
            :value(n)
            {}

            ~Float() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_FLOAT;}
            std::string type_str();
            void dump(DumpContext&);
            double value;
        };

        typedef fsh::instrusive_ptr<Float> FloatPtr;

        /*
        class ElementWrapper : public Instruction
        {
        public:
            ElementWrapper()
            { }

            ~ElementWrapper() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_ELEMENT_WRAPPER;}
            std::string type_str();
            void dump(DumpContext&);
            fsh::ElementPtr element;
        };

        typedef fsh::instrusive_ptr<ElementWrapper> ElementWrapperPtr;
        */

        class ExpressionList : public Instruction
        {
        public:
            ExpressionList()
            :isList(false)
            { }

            ~ExpressionList() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_EXPRESSION_LIST;}
            std::string type_str();
            void dump(DumpContext&);
            std::vector<InstructionPtr> expressions;
            bool isList;
        };

        typedef fsh::instrusive_ptr<ExpressionList> ExpressionListPtr;

        class WhileIf : public Instruction
        {
        public:
            WhileIf()
            { }

            ~WhileIf() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_WHILE_IF;}
            std::string type_str();
            void dump(DumpContext&);
            bool isWhile;
            InstructionPtr condition;
            InstructionPtr if_true;
            InstructionPtr if_false;
        };

        typedef fsh::instrusive_ptr<WhileIf> WhileIfPtr;

        class TryCatch : public Instruction
        {
        public:
            TryCatch()
            { }

            ~TryCatch() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_TRYCATCH;}
            std::string type_str();
            void dump(DumpContext&);
            InstructionPtr try_;
            InstructionPtr catch_;
        };

        typedef fsh::instrusive_ptr<TryCatch> TryCatchPtr;

        class IdentifierList : public Instruction
        {
        public:
            IdentifierList()
            { }

            ~IdentifierList() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_IDENTIFIER_LIST;}
            std::string type_str();
            void dump(DumpContext&);
            std::vector<IdentifierPtr> identifiers;
        };

        typedef fsh::instrusive_ptr<IdentifierList> IdentifierListPtr;

        class FunctionCall : public Instruction
        {
        public:
            FunctionCall()
            { }
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_FUNCTION_CALL;}
            std::string type_str();
            void dump(DumpContext&);
            InstructionPtr functionArguments;
            std::string name;
        };

        typedef fsh::instrusive_ptr<FunctionCall> FunctionCallPtr;

        class FunctionDef : public Instruction
        {
        public:
            FunctionDef()
            { }
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_FUNCTION_DEF;}
            std::string type_str();
            void dump(DumpContext&);
            std::vector<std::string> arg_names;
            InstructionPtr functionBody;
        };

        typedef fsh::instrusive_ptr<FunctionDef> FunctionDefPtr;

    }
}


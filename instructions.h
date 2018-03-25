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

            std::ostream& strm_;
            int indent_;
        };

        class Instruction : public fsh::instrusive_base
        {
        public:
            Instruction(size_t l):lineno(l){}
            virtual ~Instruction(){}
            virtual void Execute(Machine&) = 0;
            virtual instruction::InstructionType type() = 0;
            virtual std::string type_str() = 0;
            virtual void dump(DumpContext&) = 0;
            size_t lineno;
        };

        typedef fsh::instrusive_ptr<Instruction> InstructionPtr;

        class BinaryOperator : public Instruction
        {
        public:
            BinaryOperator(size_t l)
            :Instruction(l)
            ,lhs(nullptr)
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

        class DotOperator : public Instruction
        {
        public:
            DotOperator(size_t l)
            :Instruction(l)
            ,lhs(nullptr)
            ,rhs(nullptr)
            {}
            ~DotOperator();
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_DOT_OPERATOR;}
            std::string type_str();
            void dump(DumpContext&);
            InstructionPtr lhs;
            InstructionPtr rhs;
        };

        typedef fsh::instrusive_ptr<DotOperator> DotOperatorPtr;

        class Identifier : public Instruction
        {
        public:
            Identifier(size_t l)
            :Instruction(l)
            { }

            Identifier(size_t l, const char *p)
            :Instruction(l)
            ,name(p)
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
            Integer(size_t l)
            :Instruction(l)
            ,value(0)
            { }

            Integer(size_t l, int64_t n)
            :Instruction(l)
            ,value(n)
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
            String(size_t l)
            :Instruction(l)
            { }

            String(size_t l, const std::string& s)
            :Instruction(l)
            ,value(s)
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
            None(size_t l)
            :Instruction(l)
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
            Boolean(size_t l)
            :Instruction(l)
            ,value(false)
            { }

            Boolean(size_t l, bool b)
            :Instruction(l)
            ,value(b)
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
            Float(size_t l)
            :Instruction(l)
            ,value(0.0)
            { }

            Float(size_t l, double n)
            :Instruction(l)
            ,value(n)
            {}

            ~Float() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_FLOAT;}
            std::string type_str();
            void dump(DumpContext&);
            double value;
        };

        typedef fsh::instrusive_ptr<Float> FloatPtr;

        class Attribute : public Instruction
        {
        public:
            Attribute(size_t l)
            :Instruction(l)
            { }

            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_ATTRIBUTE;}
            std::string type_str();
            void dump(DumpContext&);
            InstructionPtr name;
            InstructionPtr value;
        };

        typedef fsh::instrusive_ptr<Attribute> AttributePtr;

        class System : public Instruction
        {
        public:
            System(size_t l)
            :Instruction(l)
            { }

            System(size_t l, const char *p)
            :Instruction(l)
            ,value(p)
            { }

            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_SYSTEM;}
            std::string type_str();
            void dump(DumpContext&);
            std::string value;
        };

        typedef fsh::instrusive_ptr<System> SystemPtr;

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
            ExpressionList(size_t l)
            :Instruction(l)
            ,isList(false)
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
            WhileIf(size_t l)
            :Instruction(l)
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

        class For : public Instruction
        {
        public:
            For(size_t l)
            :Instruction(l)
            { }

            ~For() {}
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_FOR;}
            std::string type_str();
            void dump(DumpContext&);
            InstructionPtr identifier;
            InstructionPtr list;
            InstructionPtr body;
        };

        typedef fsh::instrusive_ptr<For> ForPtr;

        class TryCatch : public Instruction
        {
        public:
            TryCatch(size_t l)
            :Instruction(l)
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
            IdentifierList(size_t l)
            :Instruction(l)
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
            FunctionCall(size_t l)
            :Instruction(l)
            { }
            void Execute(Machine&);
            InstructionType type() {return INSTRUCTION_FUNCTION_CALL;}
            std::string type_str();
            void dump(DumpContext&);
            InstructionPtr functionArguments;
            InstructionPtr call;
        };

        typedef fsh::instrusive_ptr<FunctionCall> FunctionCallPtr;

        class FunctionDef : public Instruction
        {
        public:
            FunctionDef(size_t l)
            :Instruction(l)
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


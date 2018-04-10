#pragma once
#include <memory>

namespace fsh
{
    class Machine;

    class Ast
    {
    public:
        Ast(size_t l):lineno(l){}
        virtual ~Ast(){}
        virtual void GenerateCode(Machine&, ByteCode&) = 0;
        virtual AstType type() = 0;
        size_t lineno;
    };

    class ASTBinaryOperator : public Ast
    {
    public:
        ASTBinaryOperator(size_t l)
        :Ast(l)
        {}
        ~ASTBinaryOperator();
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_BINARY_OPERATOR;}
        int op;
        std::unique_ptr<Ast> lhs;
        std::unique_ptr<Ast> rhs;
    };

    class ASTUnaryOperator : public Ast
    {
    public:
        ASTUnaryOperator(size_t l)
        :Ast(l)
        {}
        ~ASTUnaryOperator() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_UNARY_OPERATOR;}
        int op;
        std::unique_ptr<Ast> operand;
    };

    class ASTConstant : public Ast
    {
    public:
        enum{CTYPE_INTEGER, CTYPE_STRING, CTYPE_FLOAT, CTYPE_IDENTIFIER, CTYPE_NONE,CTYPE_TRUE, CTYPE_FALSE};
        ASTConstant(size_t l)
        :Ast(l)
        {}
        ~ASTConstant() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_CONSTANT;}
        int ctype;
        double dvalue;
        int64_t ivalue;
        std::string svalue;
    };

    class ASTAssignment : public Ast
    {
    public:
        ASTAssignment(size_t l)
        :Ast(l)
        {}
        ~ASTAssignment() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_ASSIGNMENT;}
        std::unique_ptr<Ast> lhs;            
        std::unique_ptr<Ast> rhs;            
    };

    class ASTExpressionList : public Ast
    {
    public:
        ASTExpressionList(size_t l)
        :Ast(l)
        {}
        ~ASTExpressionList() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_EXPRESSION_LIST;}
        std::vector<std::unique_ptr<Ast> > expressions;
    };

    class ASTIdentifierList : public Ast
    {
    public:
        ASTIdentifierList(size_t l)
        :Ast(l)
        {}
        ~ASTIdentifierList() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_IDENTIFIER_LIST;}
        std::vector<std::unique_ptr<ASTConstant> > identifiers;
    };

    class ASTIfStatement : public Ast
    {
    public:
        ASTIfStatement(size_t l)
        :Ast(l)
        {}
        ~ASTIfStatement() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_IF;}
        std::unique_ptr<Ast> condition;
        std::unique_ptr<Ast> if_true;
        std::unique_ptr<Ast> if_false;
    };

    class ASTWhile : public Ast
    {
    public:
        ASTWhile(size_t l)
        :Ast(l)
        {}
        ~ASTWhile() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_WHILE;}
        std::unique_ptr<Ast> condition;
        std::unique_ptr<Ast> if_true;
        std::unique_ptr<Ast> if_false;
    };

    class ASTFor : public Ast
    {
    public:
        ASTFor(size_t l)
        :Ast(l)
        {}
        ~ASTFor() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_FOR;}
        std::unique_ptr<Ast> identifier;
        std::unique_ptr<Ast> list;
        std::unique_ptr<Ast> body;
    };

    class ASTFunctionCall : public Ast
    {
    public:
        ASTFunctionCall(size_t l)
        :Ast(l)
        {}
        ~ASTFunctionCall() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_FUNCTION_CALL;}
        std::unique_ptr<Ast> call;
        std::unique_ptr<Ast> arguments;
    };

    class ASTFunctionDef : public Ast
    {
    public:
        ASTFunctionDef(size_t l)
        :Ast(l)
        {}
        ~ASTFunctionDef() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_FUNCTION_DEF;}
        std::vector<std::string> arg_names;
        std::unique_ptr<Ast> functionBody;
    };

    class ASTSystem : public Ast
    {
    public:
        ASTSystem(size_t l, const std::string& s)
        :Ast(l)
        , cmd(s)
        {}
        ~ASTSystem() {}
        void GenerateCode(Machine&, ByteCode&);
        AstType type() {return AST_SYSTEM;}
        std::string cmd;
    };

}


#pragma once 

#include <string>
#include <vector>
#include <functional>
#include "instrusive_base.h"
#include "instrusive_ptr.h"

namespace fsh
{
    enum ElementType
    {
        ELEMENT_TYPE_STRING
        ,ELEMENT_TYPE_INTEGER
        ,ELEMENT_TYPE_FLOAT
        ,ELEMENT_TYPE_LIST
        ,ELEMENT_TYPE_HEAD
        ,ELEMENT_TYPE_EXPRESSION
        ,ELEMENT_TYPE_ERROR
        ,ELEMENT_TYPE_IDENTIFIER
    };

    struct Element : public instrusive_base
    {
        virtual ElementType type() = 0;
        bool IsInteger() {return type() == ELEMENT_TYPE_INTEGER;}
        bool IsFloat() {return type() == ELEMENT_TYPE_FLOAT;}
        bool IsString() {return type() == ELEMENT_TYPE_STRING;}
        bool IsList() {return type() == ELEMENT_TYPE_LIST;}
        bool IsHead() {return type() == ELEMENT_TYPE_HEAD;}
        bool IsExpression() {return type() == ELEMENT_TYPE_EXPRESSION;}
        bool IsError() {return type() == ELEMENT_TYPE_ERROR;}
        bool IsIdentifier() {return type() == ELEMENT_TYPE_IDENTIFIER;}
    };

    typedef instrusive_ptr<Element> ElementPtr;

    enum HeadType
    {
        HEAD_TYPE_COMMAND
        , HEAD_TYPE_ERROR
        , HEAD_TYPE_LIST
    };

    struct Head : public Element
    {
        Head(HeadType ht)
        :value(ht)
        {}

        virtual ElementType type() {return ELEMENT_TYPE_HEAD;}
        HeadType value;
    };
    typedef instrusive_ptr<Head> HeadPtr;

    struct String : public Element
    {
        String() {}
        String(const std::string& s)
        :value(s)
        {}

        virtual ElementType type() {return ELEMENT_TYPE_STRING;}
        std::string value;
    };
    typedef instrusive_ptr<String> StringPtr;

    struct Identifier : public Element
    {
        Identifier() {}
        Identifier(const std::string& s)
        :name(s)
        {}

        virtual ElementType type() {return ELEMENT_TYPE_IDENTIFIER;}
        std::string name;
    };
    typedef instrusive_ptr<Identifier> IdentifierPtr;

    struct Error : public Element
    {
        Error() {}
        Error(const std::string& s)
        :msg(s)
        {}

        virtual ElementType type() {return ELEMENT_TYPE_ERROR;}
        std::string msg;
    };
    typedef instrusive_ptr<Error> ErrorPtr;

    struct Integer : public Element
    {
        Integer(int64_t n)
        : value(n)
        {}

        virtual ElementType type() {return ELEMENT_TYPE_INTEGER;}
        int64_t value;
    };
    typedef instrusive_ptr<Integer> IntegerPtr;

    struct Float : public Element
    {
        Float(double f)
        : value(f)
        {}

        virtual ElementType type() {return ELEMENT_TYPE_FLOAT;}
        double value;
    };
    typedef instrusive_ptr<Float> FloatPtr;

    struct List : public Element
    {
        void Add(const std::string&);
        void Add(int64_t n);
        size_t size() {return elements.size();}
        virtual ElementType type() {return ELEMENT_TYPE_LIST;}
        std::vector<ElementPtr> elements;
    };

    typedef instrusive_ptr<List> ListPtr;

    struct ExecutionContext;
    class Machine;
    typedef instrusive_ptr<ExecutionContext> ExecutionContextPtr;

    struct Expression : public Element
    {
        virtual ElementType type() {return ELEMENT_TYPE_EXPRESSION;}
        void Execute(ExecutionContextPtr) {}
        std::vector<std::function<void(Machine&)>> instructions;
    };

    typedef instrusive_ptr<Expression> ExpressionPtr;

    StringPtr MakeString(const std::string& s);
    ErrorPtr MakeError(const std::string& s);
    IntegerPtr MakeInteger(int64_t);
    FloatPtr MakeFloat(double);
    ListPtr MakeList(HeadType);
    ExpressionPtr MakeExpression();
    IdentifierPtr MakeIdentifier(const std::string&);
}


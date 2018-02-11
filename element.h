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
        ,ELEMENT_TYPE_LIST
        ,ELEMENT_TYPE_HEAD
        ,ELEMENT_TYPE_EXPRESSION
    };

    class Integer;
    typedef instrusive_ptr<Integer> IntegerPtr;

    struct Element : public instrusive_base
    {
        virtual ElementType type() = 0;
        bool IsInteger() {return type() == ELEMENT_TYPE_INTEGER;}
        bool IsString() {return type() == ELEMENT_TYPE_STRING;}
        bool IsList() {return type() == ELEMENT_TYPE_LIST;}
        bool IsHead() {return type() == ELEMENT_TYPE_HEAD;}
        bool IsExpression() {return type() == ELEMENT_TYPE_EXPRESSION;}

        IntegerPtr ToInteger();
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

    struct Integer : public Element
    {
        Integer(int64_t n)
        : value(n)
        {}

        virtual ElementType type() {return ELEMENT_TYPE_INTEGER;}
        int64_t value;
    };
    typedef instrusive_ptr<Integer> IntPtr;

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
    IntegerPtr MakeInteger(int64_t);
    ListPtr MakeList(HeadType);
    ExpressionPtr MakeExpression();
}


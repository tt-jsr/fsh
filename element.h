#pragma once 

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "instrusive_base.h"
#include "instrusive_ptr.h"


namespace fsh
{
    class Machine;

    enum ElementType
    {
        ELEMENT_TYPE_STRING
        ,ELEMENT_TYPE_INTEGER
        ,ELEMENT_TYPE_FLOAT
        ,ELEMENT_TYPE_LIST
        ,ELEMENT_TYPE_MAP
        ,ELEMENT_TYPE_IDENTIFIER
        ,ELEMENT_TYPE_NONE
        ,ELEMENT_TYPE_BOOLEAN
        ,ELEMENT_TYPE_FILE_HANDLE
        ,ELEMENT_TYPE_ERROR
        ,ELEMENT_TYPE_ATTRIBUTE
        ,ELEMENT_TYPE_FUNCTION_DEF
    };

    struct Element : public instrusive_base
    {
        virtual ElementType type() const = 0;
        bool IsInteger() const {return type() == ELEMENT_TYPE_INTEGER;}
        bool IsFloat() const {return type() == ELEMENT_TYPE_FLOAT;}
        bool IsString() const {return type() == ELEMENT_TYPE_STRING;}
        bool IsList() const {return type() == ELEMENT_TYPE_LIST;}
        bool IsMap() const {return type() == ELEMENT_TYPE_MAP;}
        bool IsIdentifier() const {return type() == ELEMENT_TYPE_IDENTIFIER;}
        bool IsNone() const {return type() == ELEMENT_TYPE_NONE;}
        bool IsBoolean() const {return type() == ELEMENT_TYPE_BOOLEAN;}
        bool IsFileHandle() const {return type() == ELEMENT_TYPE_FILE_HANDLE;}
        bool IsError() const {return type() == ELEMENT_TYPE_ERROR;}
        bool IsAttribute() const {return type() == ELEMENT_TYPE_ATTRIBUTE;}
        bool IsFunctionDef() const {return type() == ELEMENT_TYPE_FUNCTION_DEF;}
    };

    typedef instrusive_ptr<Element> ElementPtr;

    struct Error : public Element
    {
        Error()
        :bOk(false)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_ERROR;}
        std::string msg;
        bool bOk;
    };

    typedef instrusive_ptr<Error> ErrorPtr;

    struct None : public Element
    {
        None() {}

        virtual ElementType type() const {return ELEMENT_TYPE_NONE;}
    };
    typedef instrusive_ptr<None> NonePtr;

    struct String : public Element
    {
        String() {}
        String(const std::string& s)
        :value(s)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_STRING;}
        std::string value;
    };
    typedef instrusive_ptr<String> StringPtr;

    struct Identifier : public Element
    {
        Identifier()
        {}

        Identifier(const std::string& s)
        :value(s)
        {}
        
        virtual ElementType type() const {return ELEMENT_TYPE_IDENTIFIER;}
        std::string value;
    };
    typedef instrusive_ptr<Identifier> IdentifierPtr;

    struct Integer : public Element
    {
        Integer(int64_t n)
        : value(n)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_INTEGER;}
        int64_t value;
    };
    typedef instrusive_ptr<Integer> IntegerPtr;

    struct Boolean : public Element
    {
        Boolean(bool b)
        : value(b)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_BOOLEAN;}
        bool value;
    };
    typedef instrusive_ptr<Boolean> BooleanPtr;

    struct Float : public Element
    {
        Float(double f)
        : value(f)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_FLOAT;}
        double value;
    };
    typedef instrusive_ptr<Float> FloatPtr;

    struct Attribute : public Element
    {
        Attribute()
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_ATTRIBUTE;}
        IdentifierPtr name;
        ElementPtr value;
    };
    typedef instrusive_ptr<Attribute> AttributePtr;

    struct List : public Element
    {
        size_t size() {return items.size();}
        virtual ElementType type() const {return ELEMENT_TYPE_LIST;}
        std::vector<ElementPtr> items;
        std::string listtype;
    };

    typedef instrusive_ptr<List> ListPtr;

    struct FunctionDef : public Element
    {
        virtual ElementType type() const {return ELEMENT_TYPE_FUNCTION_DEF;}
        int64_t funcid;
    };

    typedef instrusive_ptr<FunctionDef> FunctionDefPtr;

    struct FileHandle : public Element
    {
        FileHandle()
        :fp(nullptr)
        ,bRead(false)
        ,isPipe(false)
        ,stripnl(false)
        ,addnl(false)
        {}
        ~FileHandle()
        {
            if (fp && !isPipe)
                fclose(fp);
            if (fp && isPipe)
                pclose(fp);
            fp = nullptr;
        }
        virtual ElementType type() const {return ELEMENT_TYPE_FILE_HANDLE;}
        FILE *fp;
        bool bRead;
        bool isPipe;
        bool stripnl;
        bool addnl;
    };

    typedef instrusive_ptr<FileHandle> FileHandlePtr;

    struct MyHash
    {
    public:
        size_t operator()(const ElementPtr& e) const
        {
            if (e->IsInteger())
                return std::hash<int64_t>{}(e.cast<fsh::Integer>()->value);
            if (e->IsString())
                return std::hash<std::string>{}(e.cast<fsh::String>()->value);
            throw std::runtime_error("map: unsupported key type");
        }
    };

    struct ElementEqual
    {
    public:
        size_t operator()(const ElementPtr& lhs, const ElementPtr& rhs) const
        {
            if (lhs->type() != rhs->type())
                return false;
            if (lhs->IsInteger())
                return lhs.cast<Integer>()->value == rhs.cast<fsh::Integer>()->value;
            if (lhs->IsString())
                return lhs.cast<String>()->value == rhs.cast<fsh::String>()->value;
            throw std::runtime_error("map: unsupported key type");
        }
    };

    struct Map : public Element
    {
        size_t size() {return map.size();}
        virtual ElementType type() const {return ELEMENT_TYPE_MAP;}
        std::unordered_map<ElementPtr, ElementPtr, MyHash, ElementEqual> map;
    };

    typedef instrusive_ptr<Map> MapPtr;

    struct ExecutionContext;
    typedef instrusive_ptr<ExecutionContext> ExecutionContextPtr;

    StringPtr MakeString(const std::string& s);
    IntegerPtr MakeInteger(int64_t);
    FloatPtr MakeFloat(double);
    ListPtr MakeList(const char *);
    MapPtr MakeMap();
    IdentifierPtr MakeIdentifier(const std::string&);
    NonePtr MakeNone();
    BooleanPtr MakeBoolean(bool);
    FileHandlePtr MakeFileHandle();
    ErrorPtr MakeError(const std::string&, bool);
    FunctionDefPtr MakeFunctionDef(int64_t id);
}


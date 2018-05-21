#pragma once 

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>


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
        ,ELEMENT_TYPE_FUNCTION_DEF_ID
        ,ELEMENT_TYPE_PAIR
        ,ELEMENT_TYPE_PIPELINE_ACTION
        ,ELEMENT_TYPE_LIST_ITEM
    };

    struct Element : public instrusive_base
    {
        virtual ElementType type() const = 0;
        virtual std::string stype() const = 0;
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
        bool IsFunctionDefId() const {return type() == ELEMENT_TYPE_FUNCTION_DEF_ID;}
        bool IsPair() const {return type() == ELEMENT_TYPE_PAIR;}
        bool IsPipeLineAction() const {return type() == ELEMENT_TYPE_PIPELINE_ACTION;}
        bool IsListItem() const {return type() == ELEMENT_TYPE_LIST_ITEM;}
    };

    typedef instrusive_ptr<Element> ElementPtr;

    struct PipeLineAction : public Element
    {
        PipeLineAction()
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_PIPELINE_ACTION;}
        virtual std::string stype() const {return "PipeLineAction";}
        enum Action {
            RESTART
            ,EXIT
        };
        Action action;
    };

    typedef instrusive_ptr<PipeLineAction> PipeLineActionPtr;

    struct Error : public Element
    {
        Error()
        :bOk(false)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_ERROR;}
        virtual std::string stype() const {return "Error";}
        std::string msg;
        bool bOk;
    };

    typedef instrusive_ptr<Error> ErrorPtr;

    struct None : public Element
    {
        None() {}

        virtual ElementType type() const {return ELEMENT_TYPE_NONE;}
        virtual std::string stype() const {return "None";}
    };
    typedef instrusive_ptr<None> NonePtr;

    struct String : public Element
    {
        String() {}
        String(const std::string& s)
        :value(s)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_STRING;}
        virtual std::string stype() const {return "String";}
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
        virtual std::string stype() const {return "Identifier";}
        std::string value;
    };
    typedef instrusive_ptr<Identifier> IdentifierPtr;

    struct Integer : public Element
    {
        Integer(int64_t n)
        : value(n)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_INTEGER;}
        virtual std::string stype() const {return "Integer";}
        int64_t value;
    };
    typedef instrusive_ptr<Integer> IntegerPtr;

    struct Boolean : public Element
    {
        Boolean(bool b)
        : value(b)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_BOOLEAN;}
        virtual std::string stype() const {return "Boolean";}
        bool value;
    };
    typedef instrusive_ptr<Boolean> BooleanPtr;

    struct Float : public Element
    {
        Float(double f)
        : value(f)
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_FLOAT;}
        virtual std::string stype() const {return "Float";}

        double value;
    };
    typedef instrusive_ptr<Float> FloatPtr;

    struct Attribute : public Element
    {
        Attribute()
        {}

        virtual ElementType type() const {return ELEMENT_TYPE_ATTRIBUTE;}
        virtual std::string stype() const {return "Attribute";}
        IdentifierPtr name;
        ElementPtr value;
    };
    typedef instrusive_ptr<Attribute> AttributePtr;

    struct List : public Element
    {
        size_t size() {return items.size();}
        virtual ElementType type() const {return ELEMENT_TYPE_LIST;}
        virtual std::string stype() const {return "List";}
        std::vector<ElementPtr> items;
        std::string listtype;
    };

    typedef instrusive_ptr<List> ListPtr;

    struct FunctionDefId : public Element
    {
        virtual ElementType type() const {return ELEMENT_TYPE_FUNCTION_DEF_ID;}
        virtual std::string stype() const {return "FunctionDefId";}
        int64_t funcid;
    };

    typedef instrusive_ptr<FunctionDefId> FunctionDefIdPtr;

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
        virtual std::string stype() const {return "FileHandle";}
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
        virtual std::string stype() const {return "Map";}
        std::unordered_map<ElementPtr, ElementPtr, MyHash, ElementEqual> map;
    };

    typedef instrusive_ptr<Map> MapPtr;

    struct Pair : public Element
    {
        virtual ElementType type() const {return ELEMENT_TYPE_PAIR;}
        virtual std::string stype() const {return "Pair";}
        ElementPtr first;
        ElementPtr second;
    };

    typedef instrusive_ptr<Pair> PairPtr;

    struct ListItem : public Element
    {
        virtual ElementType type() const {return ELEMENT_TYPE_LIST_ITEM;}
        virtual std::string stype() const {return "ListItem";}
        ListPtr list;
        size_t idx;
    };

    typedef instrusive_ptr<ListItem> ListItemPtr;

    struct ExecutionContext;
    typedef instrusive_ptr<ExecutionContext> ExecutionContextPtr;

    StringPtr MakeString(const std::string& s);
    IntegerPtr MakeInteger(int64_t);
    FloatPtr MakeFloat(double);
    ListPtr MakeList(const char *t = "__list__");
    MapPtr MakeMap();
    IdentifierPtr MakeIdentifier(const std::string&);
    NonePtr MakeNone();
    BooleanPtr MakeBoolean(bool);
    FileHandlePtr MakeFileHandle();
    ErrorPtr MakeError(const std::string&, bool);
    FunctionDefIdPtr MakeFunctionDefId(int64_t id);
    PairPtr MakePair(ElementPtr f, ElementPtr s);
    PipeLineActionPtr MakePipeLineAction();
    ListItemPtr MakeListItem();
}


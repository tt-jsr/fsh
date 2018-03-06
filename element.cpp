#include <cstdint>
#include <cassert>
#include <iostream>
#include "common.h"
#include "machine.h"
#include "instructions.h"

namespace fsh
{
    ListPtr MakeList(HeadType ht)
    {
        ListPtr r(new List());
        HeadPtr hp(new Head(ht));
        r->items.push_back(hp);
        return r;
    }

    StringPtr MakeString(const std::string& s)
    {
        return StringPtr(new String(s));
    }

    ErrorPtr MakeError(const std::string& s, bool b)
    {
        return ErrorPtr(new Error(s,b));
    }

    IdentifierPtr MakeIdentifier(const std::string& s)
    {
        return IdentifierPtr(new Identifier(s));
    }

    IntegerPtr MakeInteger(int64_t n)
    {
        return IntegerPtr(new Integer(n));
    }

    FloatPtr MakeFloat(double f)
    {
        return FloatPtr(new Float(f));
    }

    FunctionDefinitionPtr MakeFunctionDefinition()
    {
        return FunctionDefinitionPtr(new FunctionDefinition());
    }

    NonePtr MakeNone()
    {
        return NonePtr(new None());
    }

    FileHandlePtr MakeFileHandle()
    {
        return FileHandlePtr(new FileHandle());
    }

    BooleanPtr MakeBoolean(bool b)
    {
        return BooleanPtr(new Boolean(b));
    }

    ObjectPtr MakeObject(ObjectBase *p, uint64_t m)
    {
        ObjectPtr op = ObjectPtr(new Object());
        op->magic = m;
        op->pObject = p;
        return op;
    }
}

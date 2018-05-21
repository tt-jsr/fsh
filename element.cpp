#include <cstdint>
#include <cassert>
#include <iostream>
#include "instrusive_ptr.h"
#include "common.h"
#include "element.h"
#include "bytecode.h"
#include "builtins.h"
#include "machine.h"
#include "ast.h"

namespace fsh
{
    ListPtr MakeList(const char *lt)
    {
        ListPtr r(new List());
        r->listtype = lt;
        return r;
    }

    MapPtr MakeMap()
    {
        MapPtr r(new Map());
        return r;
    }

    StringPtr MakeString(const std::string& s)
    {
        return StringPtr(new String(s));
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

    PairPtr MakePair(ElementPtr f, ElementPtr s)
    {
        PairPtr pr(new Pair());
        pr->first = f;
        pr->second = s;
        return pr;
    }

    ErrorPtr MakeError(const std::string& m, bool b)
    {
        ErrorPtr e = new Error();
        e->msg = m;
        e->bOk = b;
        return e;
    }

    FunctionDefIdPtr MakeFunctionDefId(int64_t id)
    {
        FunctionDefIdPtr p(new FunctionDefId());
        p->funcid = id;
        return p;
    }

    PipeLineActionPtr MakePipeLineAction()
    {
        PipeLineActionPtr pl(new PipeLineAction());
        return pl;
    }

    ListItemPtr MakeListItem()
    {
        ListItemPtr li(new ListItem());
        return li;
    }
}

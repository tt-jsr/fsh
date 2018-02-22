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
        r->elements.push_back(new Head(ht));
        return r;
    }

    StringPtr MakeString(const std::string& s)
    {
        return StringPtr(new String(s));
    }

    ErrorPtr MakeError(const std::string& s)
    {
        return ErrorPtr(new Error(s));
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

    FunctionBuiltInPtr MakeFunctionBuiltIn()
    {
        return FunctionBuiltInPtr(new FunctionBuiltIn());
    }

    NonePtr MakeNone()
    {
        return NonePtr(new None());
    }

    FunctionShellPtr MakeFunctionShell()
    {
        return FunctionShellPtr(new FunctionShell());
    }

    /****************************************************/
    void FunctionBuiltIn::Execute(Machine& machine)
    {
        returnVal = body(machine, args);
        if (!returnVal)
        {
            returnVal = MakeNone();
        }
    }

    /***************************************************/
    void FunctionShell::Execute(Machine& machine)
    {
        body->Execute(machine);
    }
}

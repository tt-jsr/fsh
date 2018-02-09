#include <cstdint>
#include <cassert>
#include <iostream>
#include "common.h"

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

    IntPtr MakeInt(int64_t n)
    {
        return IntPtr(new Int(n));
    }

    ExpressionPtr MakeExpression()
    {
        return ExpressionPtr(new Expression());
    }
}

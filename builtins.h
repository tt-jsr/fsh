#pragma once
#include <vector>

namespace fsh
{
    class Element;
    typedef instrusive_ptr<Element> ElementPtr;

    ElementPtr Print(Machine&, std::vector<ElementPtr>&);
    ElementPtr If(Machine&, std::vector<ElementPtr>&);
}

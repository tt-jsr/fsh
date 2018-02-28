#pragma once
#include <vector>

namespace fsh
{
    ElementPtr Print(Machine&, std::vector<ElementPtr>&);
    ElementPtr Part(Machine&, std::vector<ElementPtr>&);
}

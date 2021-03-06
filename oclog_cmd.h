#pragma once
#include <vector>

namespace fsh
{
    ElementPtr ParseProtobuf(Machine& machine, std::vector<ElementPtr>& args);
    ElementPtr OCLogFilter(Machine& machine, std::vector<ElementPtr>& args);
}

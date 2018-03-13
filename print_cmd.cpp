#include <vector>
#include <iostream>
#include "common.h"
#include "element.h"
#include "machine.h"

namespace fsh
{

    ElementPtr Print(Machine& machine, std::vector<ElementPtr>& args)
    {
        for (auto& e : args)
        {
            std::cout << ToString(machine, e);
        }
        std::cout << std::endl;
        return MakeNone();
    }

}


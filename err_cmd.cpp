#include <vector>
#include <iostream>
#include "common.h"
#include "element.h"
#include "machine.h"

namespace fsh
{
    ElementPtr IsError(Machine& machine, std::vector<ElementPtr>& args)
    {
        if (args.size() < 1 || args[0]->type() != ELEMENT_TYPE_ERROR)
        {
            return MakeBoolean(false);
        }
        ErrorPtr ep = args[0].cast<Error>();
        return MakeBoolean(!ep->bOk);
    }

    ElementPtr ErrorString(Machine& machine, std::vector<ElementPtr>& args)
    {
        if (args.size() < 1 || args[0]->type() != ELEMENT_TYPE_ERROR)
        {
            return MakeString("");
        }
        ErrorPtr ep = args[0].cast<Error>();
        return MakeString(ep->msg);
    }

}


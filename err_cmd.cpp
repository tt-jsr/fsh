#include <vector>
#include <iostream>
#include <stdexcept>
#include "common.h"
#include "element.h"
#include "machine.h"
#include "builtins.h"

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

    ElementPtr Throw(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            throw std::runtime_error(sp->value);
        }
        throw std::runtime_error("Unknown exception");
    }
}


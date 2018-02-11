#include "common.h"
#include "machine.h"
#include "for_cmd.h"

namespace fsh
{
    void ForCmd(Machine& machine)
    {
        ElementPtr nargs = machine.data_pop();
        ElementPtr body = machine.data_pop();
        ElementPtr list = machine.data_pop();
        ElementPtr var = machine.data_pop();
        ExecutionContextPtr ctx = MakeExecutionContext();
        ctx->parent = ctxParent;

        int64_t count = 0;
        for (auto e : list->elements)
        {
            ctx->AddVariable("For.interation", MakeInt(count));
            ctx->AddVariable(variable->value, e);
            body->Execute(machine);
        }
    }
}


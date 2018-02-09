#pragma once

#include "execution_context.h"

namespace fsh
{
    void ForCmd(ExecutionContextPtr ctx, StringPtr variable, ListPtr list, ExpressionPtr body);
}


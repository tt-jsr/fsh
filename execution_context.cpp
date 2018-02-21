#include "common.h"
#include "execution_context.h"

namespace fsh
{
    ExecutionContextPtr MakeExecutionContext()
    {
        return ExecutionContextPtr(new ExecutionContext());
    }

    ElementPtr GetVariableImpl(ExecutionContextPtr ctx, const std::string& s)
    {
        if (ctx == false)
            return ElementPtr();

        auto it = ctx->variables_.find(s);
        if (it == ctx->variables_.end())
            return GetVariableImpl(ctx->parent, s);
        return it->second;
    }

    bool AddVariableImpl(ExecutionContextPtr ctx, const std::string& s, ElementPtr value)
    {
        if (ctx == false)
            return false;

        auto it = ctx->variables_.find(s);
        if (it == ctx->variables_.end())
            return AddVariableImpl(ctx->parent, s, value);
        it->second = value;
        return true;
    }

    ElementPtr ExecutionContext::GetVariable(const std::string& s)
    {
        return GetVariableImpl(ExecutionContextPtr(this), s);
    }

    void ExecutionContext::AddVariable(const std::string& name, ElementPtr value)
    {
        if (AddVariableImpl(ExecutionContextPtr(this), name, value) == false)
        {
            variables_[name] = value;
        }
    }
}

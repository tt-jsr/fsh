#include "common.h"
#include "execution_context.h"

namespace fsh
{
    ExecutionContextPtr MakeExecutionContext()
    {
        return ExecutionContextPtr(new ExecutionContext());
    }

    bool ExecutionContext::GetVariable(const std::string& s, ElementPtr& out)
    {
        auto it = variables_.find(s);
        if (it == variables_.end())
            return false;
        out = it->second;
        if (out.get() == nullptr)
            return false;
        return true;
    }

    bool ExecutionContext::AddVariable(const std::string& name, ElementPtr value)
    {
        // 1. If the variable does not exist, create it.
        // 2. If the variable exists as a null, return false. Varriable
        //    has been marked as global
        // 3. If variable exists, set it.
        auto it = variables_.find(name);
        if (it == variables_.end())
        {
            variables_[name] = value;
            return true;
        }
        if (!it->second)
            return false;
        it->second = value;
        return true;
    }
}

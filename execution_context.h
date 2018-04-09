#pragma once
#include <unordered_map>

namespace fsh
{
    struct ExecutionContext;
    typedef instrusive_ptr<ExecutionContext> ExecutionContextPtr;

    struct ExecutionContext : instrusive_base
    {
        ExecutionContextPtr parent;
        std::unordered_map<std::string, ElementPtr> variables_;

        ElementPtr GetVariable(const std::string& s);
        void AddVariable(const std::string& name, ElementPtr value);
    };

    ExecutionContextPtr MakeExecutionContext();
}


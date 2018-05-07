#pragma once
#include <unordered_map>

namespace fsh
{
    struct ExecutionContext;
    typedef instrusive_ptr<ExecutionContext> ExecutionContextPtr;

    struct ExecutionContext : instrusive_base
    {
        std::unordered_map<std::string, ElementPtr> variables_;

        bool GetVariable(const std::string& s, ElementPtr&);
        bool AddVariable(const std::string& name, ElementPtr value);
    };

    ExecutionContextPtr MakeExecutionContext();
}


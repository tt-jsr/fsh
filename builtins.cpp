#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include "common.h"
#include "element.h"
#include "machine.h"
#include "builtins.h"

namespace fsh
{
    ElementPtr GetElement(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        if (args.size() <= index)
        {
            return ElementPtr();
        }
        ElementPtr ep = args[index];
        ep = machine.resolve(ep);
        return ep;
    }

    ObjectPtr GetObject(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return ObjectPtr();

        if (ep->type() != ELEMENT_TYPE_OBJECT)
        {
            return ObjectPtr();
        }
        return ep.cast<Object>();
    }

    StringPtr GetString(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return StringPtr();

        if (ep->type() != ELEMENT_TYPE_STRING)
        {
            return StringPtr();
        }
        return ep.cast<String>();
    }

    IntegerPtr GetInteger(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return IntegerPtr();

        if (ep->type() != ELEMENT_TYPE_INTEGER)
        {
            return IntegerPtr();
        }
        return ep.cast<Integer>();
    }

    FunctionDefinitionPtr GetFunctionDefinition(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
        {
            return FunctionDefinitionPtr();
        }

        if (ep->type() != ELEMENT_TYPE_FUNCTION_DEFINITION)
        {
            return FunctionDefinitionPtr();
        }
        return ep.cast<FunctionDefinition>();
    }

    // You must push_context() before calling this function, and pop_context
    // after it returns!
    ElementPtr CallFunctionImpl(Machine& machine, FunctionDefinitionPtr funcDef, size_t nItemsOnStack)
    {
        if (funcDef->isBuiltIn)
        {
            // now execute the function
            std::vector<ElementPtr> arguments;
            while(nItemsOnStack)
            {
                arguments.push_back(machine.pop_data());
                --nItemsOnStack;
            }
            std::reverse(arguments.begin(), arguments.end());
            ElementPtr rtn = funcDef->builtInBody(machine, arguments);
            rtn = machine.resolve(rtn);
            return rtn;
        }
        else
        {
            std::vector<ElementPtr> dataArgs;
            // Execute each argument and put the result into the dataArgs vector
            while (nItemsOnStack)
            {
                dataArgs.push_back(machine.pop_data());
                --nItemsOnStack;
            }
            std::reverse(dataArgs.begin(), dataArgs.end());
            size_t dataArgIdx = 0;
            for (; dataArgIdx < dataArgs.size() && dataArgIdx < funcDef->arg_names.size(); ++dataArgIdx)
            {
                machine.store_variable(funcDef->arg_names[dataArgIdx], dataArgs[dataArgIdx]);
            }
            // Any named arguments that the caller did not provide, we set to None
            for (;dataArgIdx < funcDef->arg_names.size(); ++dataArgIdx)
            {
                ElementPtr none = MakeNone();
                machine.store_variable(funcDef->arg_names[dataArgIdx], none);
            }
            size_t top = machine.size_data();
            funcDef->functionBody->Execute(machine);
            ElementPtr rtn = machine.peek_data();
            rtn = machine.resolve(rtn);
            while(machine.size_data() > top)
            {
                machine.pop_data();
            }
            return rtn;
        }
    }

    ElementPtr UnitTest(Machine& machine, std::vector<ElementPtr>& args)
    {
        IntegerPtr n = GetInteger(machine, args, 0);
        if (n)
        {
            machine.unittest_callback(n->value);
        }
    }
}


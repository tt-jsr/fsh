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

    ElementPtr GetElement(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode)
    {
        if (args.size() <= index)
        {
            errorCode = GET_NOT_ENOUGH_ARGUMENTS;
            return ElementPtr();
        }
        ElementPtr ep = args[index];
        if (ep->type() == ELEMENT_TYPE_IDENTIFIER)
        {
            IdentifierPtr ip = ep.cast<Identifier>();
            if (ip->IsVariable())
            {
                if (machine.get_variable(ip->value, ep) == false)
                {
                    errorCode = GET_VARIABLE_NOT_FOUND;
                    return ElementPtr();
                }
            }
        }
        return ep;
    }

    ObjectPtr GetObject(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode)
    {
        ElementPtr ep = GetElement(machine, args, index, errorCode);
        if(ep.get() == nullptr)
            return ObjectPtr();

        if (ep->type() != ELEMENT_TYPE_OBJECT)
        {
            errorCode = GET_NOT_EXPECTED_TYPE;
            return ObjectPtr();
        }
        return ep.cast<Object>();
    }

    StringPtr GetString(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode)
    {
        ElementPtr ep = GetElement(machine, args, index, errorCode);
        if(ep.get() == nullptr)
            return StringPtr();

        if (ep->type() != ELEMENT_TYPE_STRING)
        {
            errorCode = GET_NOT_EXPECTED_TYPE;
            return StringPtr();
        }
        return ep.cast<String>();
    }

    IOObject * GetIOObject(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode)
    {
        ObjectPtr op = GetObject(machine, args, index, errorCode);
        if(op.get() ==nullptr)
            return nullptr;

        if(op->magic != ioobject_magic)
        {
            errorCode = GET_NOT_EXPECTED_TYPE;
            return nullptr;
        }

        return (IOObject *)op->pObject;
    }

    FunctionDefinitionPtr GetFunctionDefinition(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode)
    {
        ElementPtr ep = GetElement(machine, args, index, errorCode);
        if(ep.get() == nullptr)
        {
            return FunctionDefinitionPtr();
        }

        if (ep->type() != ELEMENT_TYPE_FUNCTION_DEFINITION)
        {
            errorCode = GET_NOT_EXPECTED_TYPE;
            return FunctionDefinitionPtr();
        }
        return ep.cast<FunctionDefinition>();
    }

    ElementPtr CallFunction(Machine& machine, FunctionDefinitionPtr funcDef, size_t nItemsOnStack)
    {
        if (funcDef->isBuiltIn)
        {
            machine.push_context();
            // now execute the function
            std::vector<ElementPtr> arguments;
            while(nItemsOnStack)
            {
                arguments.push_back(machine.pop_data());
                --nItemsOnStack;
            }
            std::reverse(arguments.begin(), arguments.end());
            ElementPtr rtn = funcDef->builtInBody(machine, arguments);
            machine.pop_context();
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
            // Create a new context, create a variable for each named argument
            machine.push_context();
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
            while(machine.size_data() > top)
            {
                machine.pop_data();
            }
            machine.pop_context();
            return rtn;
        }
    }
}


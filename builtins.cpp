#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <sstream>
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

    ListPtr GetList(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return ListPtr();

        if (ep->type() != ELEMENT_TYPE_LIST)
        {
            return ListPtr();
        }
        return ep.cast<List>();
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

    IdentifierPtr GetIdentifier(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return IdentifierPtr();

        if (ep->type() != ELEMENT_TYPE_IDENTIFIER)
        {
            return IdentifierPtr();
        }
        return ep.cast<Identifier>();
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
            assert(!rtn->IsIdentifier());
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
        return MakeNone();
    }

    ElementPtr ToInt(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("Int[] requires an argument");
        switch(e->type())
        {
        case ELEMENT_TYPE_FLOAT:
            return MakeInteger((int64_t)(e.cast<Float>()->value));
        case ELEMENT_TYPE_STRING:
            return MakeInteger(std::stol(e.cast<String>()->value));
        case ELEMENT_TYPE_INTEGER:
            return e;
        default:
            throw std::runtime_error("Int[] unconvertable type");
        }
    }

    ElementPtr ToFloat(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("Float[] requires an argument");
        switch(e->type())
        {
        case ELEMENT_TYPE_FLOAT:
            return e;
        case ELEMENT_TYPE_STRING:
            return MakeFloat(std::stof(e.cast<String>()->value));
        case ELEMENT_TYPE_INTEGER:
            return MakeFloat((double)(e.cast<Integer>()->value));
        default:
            throw std::runtime_error("Float[] unconvertable type");
        }
    }

    std::string toString(Machine& machine, ElementPtr e)
    {
        switch(e->type())
        {
        case ELEMENT_TYPE_INTEGER:
            {
                IntegerPtr n = e.cast<Integer>();
                return std::to_string(n->value);
            }
            break;
        case ELEMENT_TYPE_BOOLEAN:
            {
                BooleanPtr n = e.cast<Boolean>();
                if (n->value)
                    return "True";
                else
                    return "False";
            }
            break;
        case ELEMENT_TYPE_FLOAT:
            {
                FloatPtr f = e.cast<Float>();
                return std::to_string(f->value);
            }
            break;
        case ELEMENT_TYPE_ERROR:
            {
                ErrorPtr ep = e.cast<Error>();
                return ep->msg;
            }
            break;
        case ELEMENT_TYPE_IDENTIFIER:
            {
                IdentifierPtr id = e.cast<Identifier>();
                ElementPtr e;
                if (machine.get_variable(id->value, e))
                {
                    if (e->IsIdentifier())
                        return e.cast<Identifier>()->value;
                    else
                        return toString(machine, e);
                }
                else
                    return id->value;
            }
            break;
        case ELEMENT_TYPE_NONE:
            {
                return "None";
            }
            break;
        case ELEMENT_TYPE_STRING:
            {
                ElementPtr r = machine.resolve(e);
                assert(r->IsString());
                return r.cast<String>()->value;
            }
            break;
        case ELEMENT_TYPE_LIST:
            {
                ListPtr lp = e.cast<List>();
                std::string rtn;
                for (size_t idx = 0; idx < lp->items.size(); ++idx)
                {
                    ElementPtr item = lp->items[idx];
                    rtn.push_back('\'');
                    std::string s = toString(machine, item);
                    rtn += s;
                    rtn.push_back('\'');
                    if (idx < lp->items.size()-1)
                        rtn.push_back(',');
                }
                return rtn;
            }
            break;
        case ELEMENT_TYPE_FUNCTION_DEFINITION:
            {
                return "FunctionDefinition";
            }
            break;
        default:
            {
                std::stringstream strm;
                strm << "(" << e->type() << ")";
                return strm.str();
            }
            break;
        }
    }

    ElementPtr ToString(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("ToString[] requires an argument");
        return MakeString(toString(machine, e));
    }

    ElementPtr Print(Machine& machine, std::vector<ElementPtr>& args)
    {
        for (auto& e : args)
        {
            std::cout << toString(machine, e);
        }
        std::cout << std::endl;
        return MakeNone();
    }

    ElementPtr Eval(Machine& machine, std::vector<ElementPtr>& args)
    {
        throw std::runtime_error("Eval is not implemented");
    }

    void RegisterBuiltIns(Machine& machine)
    {
        // IO
        machine.register_builtin("Print", fsh::Print);
        machine.register_builtin("ReadFile", fsh::ReadFile);
        machine.register_builtin("OpenFile", fsh::OpenFile);
        machine.register_builtin("PipeLine", fsh::PipeLine);
        machine.register_builtin("OpenProcess", fsh::OpenProcess);
        machine.register_builtin("ReadProcess", fsh::ReadProcess);

        // String
        machine.register_builtin("Trim", fsh::Trim);
        machine.register_builtin("TrimLeft", fsh::TrimLeft);
        machine.register_builtin("TrimRight", fsh::TrimRight);
        machine.register_builtin("Split", fsh::Split);
        machine.register_builtin("Strcmp", fsh::Strcmp);
        machine.register_builtin("RegMatch", fsh::RegMatch);
        machine.register_builtin("RegSearch", fsh::RegSearch);
        machine.register_builtin("Find", fsh::Find);

        // List
        machine.register_builtin("Part", fsh::Part);
        machine.register_builtin("DefineRecord", fsh::DefineRecord);
        machine.register_builtin("MakeRecord", fsh::MakeRecord);
        machine.register_builtin("Len", fsh::Len);

        // Misc
        machine.register_builtin("IsError", fsh::IsError);
        machine.register_builtin("ErrorString", fsh::ErrorString);
        machine.register_builtin("Throw", fsh::Throw);
        machine.register_builtin("UnitTest", fsh::UnitTest);
        machine.register_builtin("Int", fsh::ToInt);
        machine.register_builtin("Float", fsh::ToFloat);
        machine.register_builtin("ToString", fsh::ToString);
        machine.register_builtin("Eval", fsh::Eval);
    }
}


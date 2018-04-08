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

    MapPtr GetMap(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return MapPtr();

        if (ep->type() != ELEMENT_TYPE_MAP)
        {
            return MapPtr();
        }
        return ep.cast<Map>();
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

    /*
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
    */

    /*
    void RearraingeArgs(Machine& machine, std::vector<ElementPtr>& boundArgs, std::vector<ElementPtr>& args, std::vector<ElementPtr>& out)
    {
        // Process _n arguments first
        for (auto e : boundArgs)
        {
            if (e->IsIdentifier())
            {
                std::string n = e.cast<Identifier>()->value;
                if (n[0] == '_')
                {
                    size_t dst = strtol(&n[1], nullptr, 10);
                    if (dst-1 >= args.size())
                        throw std::runtime_error("Bind: Positional argument out of range");
                    out.push_back(args[dst-1]);
                }
                else
                    throw std::runtime_error("Bind invalid position argument");
            }
            else
            {
                if (e->IsAttribute())
                {
                    AttributePtr attr = e.cast<Attribute>();
                    machine.store_variable(attr->name->value, attr->value);
                }
                else
                    out.push_back(e);
            }
        }
    }

    // You must push_context() before calling this function, and pop_context
    // after it returns!
    ElementPtr CallFunctionImpl(Machine& machine, bool isBind, FunctionDefinitionPtr funcDef, size_t nItemsOnStack)
    {
        if (funcDef->isBuiltIn)
        {
            // now execute the function
            std::vector<ElementPtr> arguments;
            while(nItemsOnStack)
            {
                ElementPtr e = machine.pop_data();
                if (e->IsAttribute() && isBind == false)
                {
                    AttributePtr attr = e.cast<Attribute>();
                    machine.store_variable(attr->name->value, attr->value);
                }
                else
                {
                    e = machine.resolve(e);
                    arguments.push_back(e);
                }
                --nItemsOnStack;
            }
            std::reverse(arguments.begin(), arguments.end());
            if (funcDef->boundArgs.size())
            {
                std::vector<ElementPtr> tmp;
                RearraingeArgs(machine, funcDef->boundArgs, arguments, tmp);
                arguments = std::move(tmp);
            }
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
                ElementPtr e = machine.pop_data();
                if (e->IsAttribute() && isBind == false)
                {
                    AttributePtr attr = e.cast<Attribute>();
                    machine.store_variable(attr->name->value, attr->value);
                }
                else
                {
                    dataArgs.push_back(e);
                }
                --nItemsOnStack;
            }
            std::reverse(dataArgs.begin(), dataArgs.end());
            if (funcDef->boundArgs.size())
            {
                std::vector<ElementPtr> tmp;
                RearraingeArgs(machine, funcDef->boundArgs, dataArgs, tmp);
                dataArgs = std::move(tmp);
            }
            size_t dataArgIdx = 0;
            for (; dataArgIdx < dataArgs.size() && dataArgIdx < funcDef->arg_names.size(); ++dataArgIdx)
            {
                ElementPtr e = machine.resolve(dataArgs[dataArgIdx]);
                machine.store_variable(funcDef->arg_names[dataArgIdx], e);
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
*/
    ElementPtr UnitTest(Machine& machine, std::vector<ElementPtr>& args)
    {
        IntegerPtr n = GetInteger(machine, args, 0);
        if (n)
        {
            machine.unittest_callback(n->value);
        }
        return MakeNone();
    }

    IntegerPtr ToInt(Machine& machine, std::vector<ElementPtr>& args)
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
            return e.cast<Integer>();
        default:
            throw std::runtime_error("Int[] unconvertable type");
        }
    }

    FloatPtr ToFloat(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("Float[] requires an argument");
        switch(e->type())
        {
        case ELEMENT_TYPE_FLOAT:
            return e.cast<Float>();
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
        case ELEMENT_TYPE_MAP:
            {
                MapPtr mp = e.cast<Map>();
                std::string rtn;
                int count = 0;
                for (auto& pr : mp->map)
                {
                    ElementPtr key = pr.first;
                    ElementPtr value = pr.second;
                    rtn.push_back('\'');
                    std::string s = toString(machine, key);
                    rtn += s;
                    rtn.push_back('\'');
                    rtn.push_back(':');
                    rtn.push_back('\'');
                    s = toString(machine, value);
                    rtn += s;
                    rtn.push_back('\'');
                    if (count < mp->map.size()-1)
                        rtn.push_back(',');
                }
                return rtn;
            }
            break;
            /*
        case ELEMENT_TYPE_FUNCTION_DEFINITION:
            {
                return "FunctionDefinition";
            }
            break;
            */
        default:
            {
                std::stringstream strm;
                strm << "(" << e->type() << ")";
                return strm.str();
            }
            break;
        }
    }

    StringPtr ToString(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("ToString[] requires an argument");
        return MakeString(toString(machine, e));
    }

    ElementPtr Print(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool addnl(true);
        machine.get_variable("__addnl", addnl);
        for (auto& e : args)
        {
            std::cout << toString(machine, e);
        }
        if (addnl)
            std::cout << std::endl;
        return MakeNone();
    }

    /*
    FunctionDefinitionPtr Bind(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e->IsFunctionDefinition())
            throw std::runtime_error("Bind requires function");

        FunctionDefinitionPtr funcSrc = e.cast<FunctionDefinition>();
        if (funcSrc->boundArgs.size())
            throw std::runtime_error("Bind: Cannot bind to a bound function");
        FunctionDefinitionPtr funcDst = MakeFunctionDefinition();
        funcDst->builtInBody = funcSrc->builtInBody;
        funcDst->functionBody = funcSrc->functionBody;
        funcDst->isBuiltIn= funcSrc->isBuiltIn;
        funcDst->arg_names = funcSrc->arg_names;
        for (size_t idx = 1; idx < args.size(); ++idx)
        {
            funcDst->boundArgs.push_back(args[idx]);
        }
        return funcDst;
    }
*/
    void RegisterBuiltInImpl(Machine& machine, const std::string& name, std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)> f)
    {
        FunctionDefinition fd;
        fd.builtIn = f;
        fd.isBuiltIn = true;
        int64_t id = machine.registerFunction(fd);
        ElementPtr e = MakeInteger(id);
        machine.store_variable(name, e);
    }

    void RegisterBuiltIns(Machine& machine)
    {
        // IO
        RegisterBuiltInImpl(machine, "Print", fsh::Print);
        RegisterBuiltInImpl(machine, "ReadFile", fsh::ReadFile);
        RegisterBuiltInImpl(machine, "OpenFile", fsh::OpenFile);
        RegisterBuiltInImpl(machine, "PipeLine", fsh::PipeLine);
        RegisterBuiltInImpl(machine, "OpenProcess", fsh::OpenProcess);
        RegisterBuiltInImpl(machine, "ReadProcess", fsh::ReadProcess);

        // String
        RegisterBuiltInImpl(machine, "Trim", fsh::Trim);
        RegisterBuiltInImpl(machine, "TrimLeft", fsh::TrimLeft);
        RegisterBuiltInImpl(machine, "TrimRight", fsh::TrimRight);
        RegisterBuiltInImpl(machine, "Split", fsh::Split);
        RegisterBuiltInImpl(machine, "Strcmp", fsh::Strcmp);
        RegisterBuiltInImpl(machine, "RegMatch", fsh::RegMatch);
        RegisterBuiltInImpl(machine, "RegSearch", fsh::RegSearch);
        RegisterBuiltInImpl(machine, "Find", fsh::Find);
        RegisterBuiltInImpl(machine, "Format", fsh::Format);
        RegisterBuiltInImpl(machine, "SubString", fsh::SubString);

        // List
        RegisterBuiltInImpl(machine, "Part", fsh::Part);
        RegisterBuiltInImpl(machine, "Subscript", fsh::Subscript);
        RegisterBuiltInImpl(machine, "DefineRecord", fsh::DefineRecord);
        RegisterBuiltInImpl(machine, "MakeRecord", fsh::MakeRecord);
        RegisterBuiltInImpl(machine, "Len", fsh::Len);
        RegisterBuiltInImpl(machine, "Append", fsh::Append);
        RegisterBuiltInImpl(machine, "SetRecordType", fsh::SetRecordType);

        // Map
        RegisterBuiltInImpl(machine, "CreateMap", fsh::CreateMap);
        RegisterBuiltInImpl(machine, "Insert", fsh::Insert);
        RegisterBuiltInImpl(machine, "Delete", fsh::Delete);
        RegisterBuiltInImpl(machine, "Lookup", fsh::Lookup);

        // Misc
        RegisterBuiltInImpl(machine, "IsError", fsh::IsError);
        RegisterBuiltInImpl(machine, "ErrorString", fsh::ErrorString);
        RegisterBuiltInImpl(machine, "Throw", fsh::Throw);
        RegisterBuiltInImpl(machine, "UnitTest", fsh::UnitTest);
        RegisterBuiltInImpl(machine, "Int", fsh::ToInt);
        RegisterBuiltInImpl(machine, "Float", fsh::ToFloat);
        RegisterBuiltInImpl(machine, "ToString", fsh::ToString);
        //machine.register_builtin("Bind", fsh::Bind);
    }
}


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

    BooleanPtr GetBoolean(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return BooleanPtr();

        if (ep->type() != ELEMENT_TYPE_BOOLEAN)
        {
            return BooleanPtr();
        }
        return ep.cast<Boolean>();
    }

    FunctionDefIdPtr GetFunctionDefId(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return FunctionDefIdPtr();

        if (ep->type() != ELEMENT_TYPE_FUNCTION_DEF_ID)
        {
            return FunctionDefIdPtr();
        }
        return ep.cast<FunctionDefId>();
    }

    /*******************************************************************/
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
        if (e.get() == nullptr)
            return "null";
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
                rtn.push_back('[');
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
                rtn.push_back(']');
                return rtn;
            }
            break;
        case ELEMENT_TYPE_MAP:
            {
                MapPtr mp = e.cast<Map>();
                std::string rtn;
                int count = 0;
                rtn.push_back('[');
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
                rtn.push_back(']');
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
        machine.get_variable("addnl", addnl);
        for (auto& e : args)
        {
            std::cout << toString(machine, e);
        }
        if (addnl)
            std::cout << std::endl;
        return MakeNone();
    }

    ElementPtr FunctionCallHelper(Machine& machine, FunctionDefinition *fd, int64_t nArgsOnStack)
    {
        std::vector<ElementPtr> args;
        while(nArgsOnStack)
        {
            ElementPtr e = machine.pop_data();
            args.push_back(e);
            --nArgsOnStack;
        }
        std::reverse(args.begin(), args.end());
        ElementPtr rtn;
        try {
            if (fd->isBuiltIn)
            {
                rtn = fd->builtIn(machine, args);
                rtn = machine.resolve(rtn);
            }
            else
            {
                size_t end = std::min(fd->arg_names.size(), args.size());
                size_t idx = 0;
                for (idx; idx < end; ++idx)
                {
                    machine.store_variable(fd->arg_names[idx], args[idx]);
                }
                for (; idx < fd->arg_names.size();++idx)
                {
                    machine.store_variable(fd->arg_names[idx], MakeNone());
                }
                fd->shellFunction.ip = 0;
                while(fd->shellFunction.ip < fd->shellFunction.size())
                {
                    fsh::Execute(machine, fd->shellFunction);
                    ++fd->shellFunction.ip;
                }
                rtn = machine.pop_data();
                rtn = machine.resolve(rtn);
            }
            machine.pop_context();
            return rtn;
        }
        catch (std::exception& e)
        {
            machine.pop_context();
            throw;
        }
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
        ElementPtr e = MakeFunctionDefId(id);
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
        RegisterBuiltInImpl(machine, "CreateRecord", fsh::CreateRecord);
        RegisterBuiltInImpl(machine, "CreateList", fsh::CreateList);
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


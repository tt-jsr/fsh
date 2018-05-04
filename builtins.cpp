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

    PairPtr GetPair(Machine& machine, std::vector<ElementPtr>& args, size_t index)
    {
        ElementPtr ep = GetElement(machine, args, index);
        if(ep.get() == nullptr)
            return PairPtr();

        if (ep->type() != ELEMENT_TYPE_PAIR)
        {
            return PairPtr();
        }
        return ep.cast<Pair>();
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
                return e.cast<String>()->value;
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
        case ELEMENT_TYPE_PAIR:
            {
                std::string rtn;
                rtn.push_back('(');
                PairPtr pr = e.cast<Pair>();
                rtn += toString(machine, pr->first);
                rtn += ", ";
                rtn += toString(machine, pr->second);
                rtn.push_back(')');
                return rtn;
            }
            break;
        case ELEMENT_TYPE_FUNCTION_DEF_ID:
            {
                FunctionDefIdPtr fd = e.cast<FunctionDefId>();
                return std::to_string(fd->funcid);
            }
            break;
        case ELEMENT_TYPE_PIPELINE_ACTION:
            {
                PipeLineActionPtr pla = e.cast<PipeLineAction>();
                switch (pla->action)
                {
                case PipeLineAction::RESTART:
                    return std::string("PIPELINE_RESTART");
                    break;
                case PipeLineAction::EXIT:
                    return std::string("PIPELINE_EXIT");
                    break;
                default:
                    return "PIPELINE_UNDEFINED";
                }
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

    ElementPtr Log(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool addnl(true);
        machine.get_variable("addnl", addnl);
        for (auto& e : args)
        {
            machine.log() << toString(machine, e);
        }
        if (addnl)
            machine.log() << std::endl;
        return MakeNone();
    }

    /*****************************************************************/

    ElementPtr FunctionDefinition::Call(Machine& machine, int64_t nArgsOnStack)
    {
        std::vector<ElementPtr> args;
        while(nArgsOnStack)
        {
            ElementPtr e = machine.pop_data();
            args.push_back(e);
            --nArgsOnStack;
        }
        std::reverse(args.begin(), args.end());
        try {
            ElementPtr rtn = CallImpl(machine, args);
            rtn = machine.resolve(rtn);
            machine.pop_context();
            return rtn;
        }
        catch (std::exception& e)
        {
            machine.pop_context();
            throw;
        }
    }

    /*****************************************************************/

    
    ElementPtr BuiltInFunction::CallImpl(Machine& machine, std::vector<ElementPtr>& args)
    {
        return builtIn(machine, args);
    }

    /*****************************************************************/

    ElementPtr ShellFunction::CallImpl(Machine& machine, std::vector<ElementPtr>& args)
    {
        size_t end = std::min(arg_names.size(), args.size());
        size_t idx = 0;
        for (idx; idx < end; ++idx)
        {
            machine.store_variable(arg_names[idx], args[idx]);
        }
        for (; idx < arg_names.size(); ++idx)
        {
            machine.store_variable(arg_names[idx], MakeNone());
        }
        shellFunction.ip = 0;

        machine.log() << "Function: Start code" << std::endl;
        while(shellFunction.ip < shellFunction.size())
        {
            if (!fsh::Execute(machine, shellFunction))
                break;
            ++shellFunction.ip;
        }
        machine.log() << "Function: End code" << std::endl;
        assert(machine.size_data() > 0);
        return machine.pop_data();
    }

    /*****************************************************************/

    ElementPtr BoundFunction::CallImpl(Machine& machine, std::vector<ElementPtr>& args)
    {
        std::vector<ElementPtr> target_args;
        for (ElementPtr& ba : bound_args)
        {
            if (ba->IsIdentifier())
            {
                IdentifierPtr id = ba.cast<Identifier>();
                if (id->value[0] == '_')
                {
                    size_t dst = strtol(&id->value[1], nullptr, 10);
                    if (dst-1 >= args.size())
                        throw std::runtime_error("Bind: Invalid position arguments");
                    target_args.push_back(args[dst-1]);
                }
                else
                {
                    throw std::runtime_error("Bind: invalid position argument");
                }
            }
            else
            {
                target_args.push_back(ba);
            }
        }
        
        return target->CallImpl(machine, target_args);
    }

    ElementPtr Copy(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("Copy requires argument");
        if (e->IsMap())
        {
            MapPtr src = e.cast<Map>();
            MapPtr m = MakeMap();
            m->map = src->map;
            return m;
        }
        if (e->IsList())
        {
            ListPtr src = e.cast<List>();
            ListPtr rtn = MakeList();
            rtn->items = src->items;
            return rtn;
        }
        if (e->IsString())
        {
            StringPtr src = e.cast<String>();
            StringPtr rtn = MakeString(src->value);
            return rtn;
        }
        return e;
    }

    ElementPtr First(Machine& machine, std::vector<ElementPtr>& args)
    {
        PairPtr pr = GetPair(machine, args, 0);
        if (!pr)
            throw std::runtime_error("First expects a pair");
        return pr->first;
    }

    ElementPtr Second(Machine& machine, std::vector<ElementPtr>& args)
    {
        PairPtr pr = GetPair(machine, args, 0);
        if (!pr)
            throw std::runtime_error("Second expects a pair");
        return pr->second;
    }

    PairPtr CreatePair(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr f = GetElement(machine, args, 0);
        if (!f)
            throw std::runtime_error("Pair expects a first arg");
        ElementPtr s = GetElement(machine, args, 1);
        if (!s)
            throw std::runtime_error("Pair expects a second arg");

        return MakePair(f, s);
    }

    ElementPtr MachineProperty(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp && sp->value == "stats")
        {
            std::stringstream strm;
            strm << machine.string_table_by_id.size() << " strings in the string table" << std::endl;
            strm << machine.functions.size() << " defined functions" << std::endl;
            strm << machine.blocks.size() << " code blocks" << std::endl;
            int n = 0;
            for (auto& pr : machine.blocks)
            {
                n += pr.second.size();
            }
            n += machine.byte_code.size();
            strm << n*8 << " total code bytes" << std::endl;
            
            ExecutionContextPtr ecxt = machine.executionContext;
            while(ecxt)
            {
                strm << "Variables" << std::endl;
                for (auto& pr : ecxt->variables_)
                {
                    strm << pr.first << " : " << pr.second->stype() << std::endl;
                }
                ExecutionContextPtr tmp = ecxt->parent;
                ecxt = tmp;
            }
            return MakeString(strm.str());
        }
        return MakeNone();
    }

    void RegisterBuiltInImpl(Machine& machine, const std::string& name, std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)> f)
    {
        BuiltInFunction *fd = new BuiltInFunction();
        fd->builtIn = f;
        int64_t id = machine.registerFunction(fd);
        ElementPtr e = MakeFunctionDefId(id);
        machine.store_variable(name, e);
    }

    void RegisterBuiltIns(Machine& machine)
    {
        // IO
        RegisterBuiltInImpl(machine, "Print", fsh::Print);
        RegisterBuiltInImpl(machine, "Log", fsh::Log);
        RegisterBuiltInImpl(machine, "Input", fsh::Input);
        RegisterBuiltInImpl(machine, "ReadFile", fsh::ReadFile);
        RegisterBuiltInImpl(machine, "OpenFile", fsh::OpenFile);
        RegisterBuiltInImpl(machine, "PipeLine", fsh::PipeLine);
        RegisterBuiltInImpl(machine, "OpenProcess", fsh::OpenProcess);
        RegisterBuiltInImpl(machine, "ReadProcess", fsh::ReadProcess);
        RegisterBuiltInImpl(machine, "WriteFile", fsh::WriteFile);
        RegisterBuiltInImpl(machine, "CloseFile", fsh::CloseFile);

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
        RegisterBuiltInImpl(machine, "Set", fsh::Set);
        RegisterBuiltInImpl(machine, "Push", fsh::Push);
        RegisterBuiltInImpl(machine, "Pop", fsh::Pop);
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
        RegisterBuiltInImpl(machine, "Copy", fsh::Copy);
        RegisterBuiltInImpl(machine, "First", fsh::First);
        RegisterBuiltInImpl(machine, "Second", fsh::Second);
        RegisterBuiltInImpl(machine, "CreatePair", fsh::CreatePair);
        RegisterBuiltInImpl(machine, "MachineProperty", fsh::MachineProperty);

        // Ers
        RegisterBuiltInImpl(machine, "ParseExecutionReport", fsh::ParseExecutionReport);
    }

}


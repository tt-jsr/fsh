#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cctype>
#include "common.h"
#include "machine.h"
#include "builtins.h"
#include "ast.h"
#include "bytecode.h"

namespace fsh
{
    thread_local std::vector<ElementPtr> Machine::datastack;

    Machine::Machine(void)
    :next_string_id(0)
    ,next_function_id(0)
    ,next_block_id(0)
    ,next_element_id(0)
    {
        executionContext = MakeExecutionContext();
    }


    Machine::~Machine(void)
    {
    }

    ElementPtr Machine::Execute(Ast *pAst)
    {
        byte_code.ip = byte_code.size();
        pAst->GenerateCode(*this, get_byte_code());
        try
        {
            datastack.clear();
            while(byte_code.ip < byte_code.size())
            {
                fsh::Execute(*this, byte_code);
                ++byte_code.ip;
            }
            assert(datastack.size() > 0);
            if (datastack.size() == 0)
            {
                return ElementPtr();
            }
            return pop_data();
        }
        catch (std::exception& ex)
        {
            if (unittest_exception != nullptr)
            {
                unittest_exception(ex.what());
            }
            return MakeError(ex.what(), false);
        }
    }

    void Machine::reset()
    {
        datastack.clear();
        executionContext = MakeExecutionContext();
    }

    bool Machine::ConvertToBool(ElementPtr e)
    {
        switch(e->type())
        {
        case ELEMENT_TYPE_BOOLEAN:
            return e.cast<Boolean>()->value;
            break;
        case ELEMENT_TYPE_INTEGER:
            return e.cast<Integer>()->value != 0;
            break;
        case ELEMENT_TYPE_STRING:
            return e.cast<String>()->value.empty() == false;
        case ELEMENT_TYPE_IDENTIFIER:
            {
                IdentifierPtr id = e.cast<Identifier>();
                ElementPtr var;
                if (get_variable(id->value, var))
                    return ConvertToBool(var);
            }
            break;
        default:
            return false;
        }
        return false;
    }

    size_t Machine::size_data()
    {
        return datastack.size();
    }

    ElementPtr Machine::pop_data()
    {
        if (datastack.size() < 1)
        {
            std::stringstream strm;
            strm << "stack underflow";
            throw std::runtime_error(strm.str());
        }
        ElementPtr d = datastack.back();
        datastack.pop_back();
        return d;
    }

    ElementPtr Machine::peek_data()
    {
        if (datastack.size() < 1)
        {
            std::stringstream strm;
            strm << "stack underflow";
            throw std::runtime_error(strm.str());
        }
        ElementPtr d = datastack.back();
        return d;
    }

    void Machine::push_data(ElementPtr d)
    {
        datastack.push_back(d);
    }

    ElementPtr Machine::resolve(ElementPtr e)
    {
        if (e->IsIdentifier())
        {
            ElementPtr rtn;
            IdentifierPtr id = e.cast<Identifier>();
            if (get_variable(id->value, rtn))
                return rtn;
        }
        return e;
    }

    size_t Machine::get_record_field(const std::string& list, ElementPtr e)
    {
        if (!e->IsIdentifier())
            throw std::runtime_error("Expected identifier");
        return get_record_field(list, e.cast<Identifier>()->value);
    }

    size_t Machine::get_record_field(const std::string& list, const std::string& fieldname)
    {
        auto itRecord = record_fields.find(list);
        if (itRecord == record_fields.end())
        {
            std::stringstream strm;
            strm << "Record \"" << list << "\" not defined";
            throw std::runtime_error(strm.str());
        }
        auto itField = itRecord->second.find(fieldname);
        if (itField == itRecord->second.end())
        {
            std::stringstream strm;
            strm << "Field " << list << "." << fieldname << " not defined";
            throw std::runtime_error(strm.str());
        }
        return itField->second;
    }

    void Machine::add_record_field(const std::string& list, const std::string& field, size_t idx)
    {
        auto itRecord = record_fields.find(list);
        if (itRecord == record_fields.end())
        {
            auto pr = record_fields.emplace(list, FieldMap_t());
            itRecord = pr.first;
        }
        itRecord->second[field] = idx;
    }

    FieldMap_t * Machine::get_field_map(const std::string& list)
    {
        auto itRecord = record_fields.find(list);
        if (itRecord == record_fields.end())
        {
            return nullptr;
        }
        return &itRecord->second;
    }

    ExecutionContextPtr Machine::GetContext()
    {
        return executionContext;
    }

    ByteCode& Machine::get_byte_code()
    {
        return byte_code;
    }

    std::string Machine::string_table_get(int64_t id)
    {
        auto it = string_table_by_id.find(id);
        if (it == string_table_by_id.end())
            throw std::runtime_error("string not found");
        return it->second;
    }

    int64_t Machine::string_table_add(const std::string& s)
    {
        auto it = string_table_by_string.find(s);
        if (it != string_table_by_string.end())
            return it->second;
        string_table_by_id.emplace(++next_string_id, s);
        string_table_by_string.emplace(s, next_string_id);
        return next_string_id;
    }

    void Machine::store_variable(const std::string& name, ElementPtr d)
    {
        size_t pos = name.find_first_of('.');
        if (pos != std::string::npos)
        {
            std::string var = name.substr(0, pos);
            std::string fieldName = name.substr(pos+1);
            ElementPtr e;
            if (get_variable(var, e) == false)
            {
                std::stringstream strm;
                strm << "store variable: List variable " << var << " does not exist";
                throw std::runtime_error(strm.str());
            }
            if (e->IsList() == false)
            {
                std::stringstream strm;
                strm << "store variable: List variable " << var << " is not a list";
                throw std::runtime_error(strm.str());
            }
            ListPtr lp = e.cast<List>();

            size_t idx = get_record_field(lp->listtype, fieldName);
            if (idx >= lp->items.size())
                throw std::runtime_error("store_variable: List fieldname out of range");
            lp->items[idx] = d;
            return;
        }
        executionContext->AddVariable(name, d);
    }

    bool Machine::get_list_field(const std::string& name, ElementPtr& out)
    {
        size_t pos = name.find_first_of('.');
        if (pos == std::string::npos)
            return false;
        std::string var = name.substr(0, pos);
        std::string fieldName = name.substr(pos+1);
        ElementPtr e;
        if (get_variable(var, e) == false)
            return false;
        if (e->IsList() == false)
            return false;
        ListPtr lp = e.cast<List>();

        size_t idx = get_record_field(lp->listtype, fieldName);
        if (idx >= lp->items.size())
            throw std::runtime_error("List fieldname out of range");
        out = lp->items[idx];
        return true;
    }

    bool Machine::get_variable(const std::string& name, ElementPtr& out)
    {
        if (get_list_field(name, out))
            return true;
        out = executionContext->GetVariable(name);
        if (out)
            return true;
        return false;
    }

    bool Machine::get_variable(const std::string& name, bool& out)
    {
        ElementPtr e;
        if (get_variable(name, e))
        {
            if (e->IsBoolean())
            {
                out = e.cast<Boolean>()->value;
                return true;
            }
        }
        return false;
    }

    bool Machine::get_variable(const std::string& name, std::string& out)
    {
        ElementPtr e;
        if (get_variable(name, e))
        {
            if (e->IsString())
            {
                out = e.cast<String>()->value;
                return true;
            }
        }
        return false;
    }

    bool Machine::get_variable(const std::string& name, int64_t& out)
    {
        ElementPtr e;
        if (get_variable(name, e))
        {
            if (e->IsInteger())
            {
                out = e.cast<Integer>()->value;
                return true;
            }
        }
        return false;
    }

    bool Machine::get_variable(const std::string& name, double& out)
    {
        ElementPtr e;
        if (get_variable(name, e))
        {
            if (e->IsFloat())
            {
                out = e.cast<Float>()->value;
                return true;
            }
        }
        return false;
    }

    int64_t Machine::registerFunction(FunctionDefinition * fd)
    {
        functions[++next_function_id] = fd;
        return next_function_id;
    }

    FunctionDefinition *Machine::getFunction(int64_t id)
    {
        auto it = functions.find(id);
        if (it == functions.end())
            return nullptr;
        return it->second;
    }

    int64_t Machine::storeBlock(ByteCode& bc)
    {
        blocks[++next_block_id] = bc;
        return next_block_id;
    }

    ByteCode *Machine::getBlock(int64_t id)
    {
        auto it = blocks.find(id);
        if (it == blocks.end())
            return nullptr;
        return &it->second;
    }

    void Machine::push_context()
    {
        ExecutionContextPtr ctx = MakeExecutionContext();
        ctx->parent = executionContext;
        executionContext = ctx;
    }

    void Machine::pop_context()
    {
        ExecutionContextPtr ec = executionContext->parent;
        executionContext = ec;
    }

    void Machine::register_unittest(std::function<void (int)>& f)
    {
        unittest_callback = f;
    }
}

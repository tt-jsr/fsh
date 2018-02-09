#include "vm/machine.h"
#include "vm/instructions.h"
#include "vm/data.h"
#include "vm/rule.h"
#include <stdexcept>
#include <sstream>
#include <cassert>

namespace achtung
{
    Machine::Machine(void)
        :m_ip(0)
    {
    }


    Machine::~Machine(void)
    {
    }

    void Machine::restart()
    {
        datastack.clear();
        variables.clear();
        m_ip = 0;
    }

    void Machine::clock()
    {
        if (m_ip >= instructions.size())
        {
            // Ran off the end of the world
            throw std::runtime_error ("Invalid IP address");
        }
        std::function<void(Machine&)>& f = instructions[m_ip];
        f(*this);
        ++m_ip;
    }

    bool Machine::load_rule(Rule& mod)
    {
        m_ip = 0;
        m_pRule = &mod;
        datastack = mod.datastack;
        instructions = mod.instructions;
        return true;
    }

    bool Machine::run(Rule& mod)
    {
        load_rule(mod);
        while (m_ip < instructions.size())
            clock();
        return true;
    }

    bool Machine::get_field_value_from_message(field_id_t fid, Data& d)
    {
        if (m_pRule)
            return m_pRule->get_field_value_from_message(fid, d);
        else
            throw std::runtime_error ("get_field: No rule loaded");
    }

    bool Machine::get_field_value_from_message(const std::string& field_name, Data& d)
    {
        if (m_pRule)
            return m_pRule->get_field_value_from_message(field_name, d);
        else
            throw std::runtime_error ("get_field: No rule loaded");
    }

    bool Machine::get_enum_value_constant(const std::string& enum_name, Data& d)
    {
        if (m_pRule)
            return m_pRule->get_enum_value_constant(enum_name, d);
        else
            throw std::runtime_error ("get_enum: No rule loaded");
    }

    bool Machine::result_true()
    {
        if (datastack.size() < 1)
            return false;
        Data d = pop_data();
        if (d.is_integral() == false)
            return false;
        if (d.get_uint64() != 0)
            return true;
        return false;
    }

    Data Machine::pop_data()
    {
        if (datastack.size() < 1)
        {
            std::stringstream strm;
            strm << "rule_name: " << get_rulename() << " ip:" << get_ip() << " msg: stack underflow";
            throw std::runtime_error(strm.str());
        }
        Data d = datastack.back();
        datastack.pop_back();
        return d;
    }

    Data Machine::peek_data()
    {
        if (datastack.size() < 1)
        {
            std::stringstream strm;
            strm << "rule_name: " << get_rulename() << " ip:" << get_ip() << " msg: stack underflow";
            throw std::runtime_error(strm.str());
        }
        Data d = datastack.back();
        return d;
    }

    void Machine::push_data(Data& d)
    {
        datastack.push_back(d);
    }

    void Machine::store_variable(const std::string& name, Data& d)
    {
        assert(d.is_string() == false); // We don't allow variables to be strings
        variables[name] = d;
    }

    bool Machine::get_variable(const std::string& name, Data& out)
    {
        auto it = variables.find(name);
        if (it == variables.end())
            return false;
        out = it->second;
        return true;
    }
}

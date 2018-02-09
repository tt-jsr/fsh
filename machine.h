#pragma once

#include <vector>
#include <map>
#include "data.h"
#include "instructions.h"
#include <functional>
#include "common.h"

#define private_impl public

namespace fsh
{
    class Machine
    {
    public:
        Machine(void);
        ~Machine(void);

        void restart();
        void clock();

        bool run(Rule& mod);

        // Load the rule without running. clock() needs to be
        // called to execute each instruction
        bool load_rule(Rule& mod);

        // Pops the tos and returns true if the value is an int and
        // != 0
        bool result_true();
        void store_variable(const std::string& name, Data& d);
        bool get_variable(const std::string& name, Data& out);
        std::string get_rulename() {return m_pRule->m_rulename;}
        size_t get_ip() {return m_ip;}
    private:
        friend class Comma;
        std::vector<Data> datastack;
        std::vector<std::function<void(Machine&)> > instructions;
        std::map<std::string, Data> variables;
        size_t m_ip;
        Rule *m_pRule;
    private_impl:
        Data pop_data();
        Data peek_data();
        void push_data(Data&);
        bool get_field_value_from_message(field_id_t fid, Data& d);
        bool get_field_value_from_message(const std::string& field_name, Data& d);
        // enum_name must be in "field_name.enum_name" format
        bool get_enum_value_constant(const std::string& enum_name, Data& d);
    };
}


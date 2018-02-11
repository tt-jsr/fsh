#include <stdexcept>
#include <sstream>
#include <cassert>
#include "common.h"
#include "machine.h"
#include "instructions.h"

namespace fsh
{
    Machine::Machine(void)
    {
    }


    Machine::~Machine(void)
    {
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

    void Machine::push_data(ElementPtr& d)
    {
        datastack.push_back(d);
    }

    void Machine::store_variable(const std::string& name, ElementPtr& d)
    {
    }

    bool Machine::get_variable(const std::string& name, ElementPtr& out)
    {
        return false;
    }
}

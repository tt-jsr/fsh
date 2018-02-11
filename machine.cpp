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

    ElementPtr Machine::Execute(instruction::Instruction * i)
    {
        i->Execute(*this);
        assert(datastack.size() > 0);
        if (datastack.size() == 0)
        {
            return ElementPtr();
        }
        return pop_data();
    }


    // Substitute any $(..) variables if argument is a string
    ElementPtr Machine::resolve(ElementPtr e)
    {
        return e;
    }

    void Machine::verify(ElementPtr e, int type, const char *msg)
    {
        assert(e->type() == type);
        if (e->type() != type)
        {
            throw std::runtime_error(msg);
        }
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

    void Machine::store_variable(const std::string& name, ElementPtr& d)
    {
    }

    bool Machine::get_variable(const std::string& name, ElementPtr& out)
    {
        return false;
    }
}

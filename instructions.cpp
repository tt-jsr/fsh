#include <sstream>
#include <stdexcept>
#include <iostream>
#include "common.h"
#include "instructions.h"
#include "machine.h"
#include "data.h"

namespace
{
    void Throw(fsh::Machine& machine, const char *msg)
    {
        std::stringstream strm;
        strm << "rule_name: \"" << machine.get_rulename() << "\"" << " msg: " << msg;
        throw std::runtime_error(strm.str().c_str());
    }

    fsh::Data::Type ResolveDataType(fsh::Machine& machine, fsh::Data& d1, fsh::Data& d2)
    {
        // Our instructions only operate on ints and doubles, if we get string, it should
        // be a variable, so retrive it. The variable must resolve to an int or double.
        if (d1.is_string())
        {
            fsh::Data d;
            if (machine.get_variable(d1.get_string(), d) == false)
            {
                std::stringstream strm;
                strm << "Variable \"" << d1.get_string() << "\" does not exist";
                Throw(machine, strm.str().c_str());
            }
            if (d.is_string())
            {
                std::stringstream strm;
                strm << "Variable \"" << d1.get_string() << "\" resolved to a string";
                Throw(machine, strm.str().c_str());
            }
            d1 = d;
        }
        if (d2.is_string())
        {
            fsh::Data d;
            if (machine.get_variable(d2.get_string(), d) == false)
            {
                std::stringstream strm;
                strm << "Variable \"" << d2.get_string() << "\" does not exist";
                Throw(machine, strm.str().c_str());
            }
            if (d.is_string())
            {
                std::stringstream strm;
                strm << "Variable \"" << d2.get_string() << "\" resolved to a string";
                Throw(machine, strm.str().c_str());
            }
            d2 = d;
        }
        if (d1.is_invalid() || d2.is_invalid())
        {
            return fsh::Data::INVALID;
        }
        if (d1.is_floating() || d2.is_floating())
        {
            return fsh::Data::DOUBLE;
        }
        else
        {
            return fsh::Data::UINT64;
        }
    }
}

namespace fsh
{
    /*****************************************************/
    void And::operator()(Machine& machine)
    {
    }

    /*****************************************************/
    void Or::operator()(Machine& machine)
    {
    }

    /*****************************************************/
    void Equal::operator()(Machine& machine)
    {
    }

    /*****************************************************/
    void NotEqual::operator()(Machine& machine)
    {
    }


    /*****************************************************/
    void GreaterThan::operator()(Machine& machine)
    {
    }


    /*****************************************************/
    void GreaterThanEQ::operator()(Machine& machine)
    {
    }


    /*****************************************************/
    void LessThan::operator()(Machine& machine)
    {
    }


    /*****************************************************/
    void LessThanEQ::operator()(Machine& machine)
    {
    }

    /*****************************************************/
    void Assignment::operator()(Machine& machine)
    {
        rhs(machine);
        ElementPtr r = machine.pop_data();

        lhs(machine);
        ElementPtr l = machine.pop_data();

        machine.store_variable(l, r);
    }

    /*****************************************************/
    void FunctionCall::operator()(Machine& machine)
    {
        int nargs = 0;
        for (auto& e in args)
        {
            e(machine);
            nargs += 1;
        }
        
        ElementPtr n = MakeInt(nargs);
        machine.push_data(n);
        ElementPtr funcPtr;
        if (machine.get_variable(name, func))
            funcPtr->func(machine);
    }

    /*****************************************************/
    void FunctionDef::operator()(Machine& machine)
    {
        int nargs = 0;
        for (auto& e in args)
        {
            e(machine);
            nargs += 1;
        }
        
        ElementPtr n = MakeInt(nargs);
        machine.push_data(n);
        ElementPtr funcPtr;
        if (machine.get_variable(name, func))
            funcPtr->func(machine);
    }

    /*****************************************************/
    void Identifier::operator()(Machine& machine)
    {
        ElementPtr e = MakeString(name);

        machine.push_data(e);
    }

    /*****************************************************/
    void Integer::operator()(Machine& machine)
    {
        ElementPtr e = MakeInt(value);

        machine.push_data(e);
    }
 }


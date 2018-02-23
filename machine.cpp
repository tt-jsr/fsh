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
        executionContext = MakeExecutionContext();
    }


    Machine::~Machine(void)
    {
    }

    ElementPtr Machine::Execute(instruction::InstructionPtr i)
    {
        try
        {
            datastack.clear();
            i->Execute(*this);
            assert(datastack.size() > 0);
            if (datastack.size() == 0)
            {
                return ElementPtr();
            }
            return pop_data();
        }
        catch (std::runtime_error& ex)
        {
            return MakeError(ex.what());
        }
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

    void Machine::store_variable(const std::string& name, ElementPtr& d)
    {
        executionContext->AddVariable(name, d);
    }

    bool Machine::get_variable(const std::string& name, ElementPtr& out)
    {
        out = executionContext->GetVariable(name);
        if (out)
            return true;
        return false;
    }

    void Machine::register_builtin(const std::string& name, 
            std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)> func)
    {
        FunctionDefinitionPtr f = MakeFunctionDefinition();
        f->builtInBody = func;
        f->isBuiltIn = true;
        ElementPtr e = f.cast<Element>();
        store_variable(name, e);
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

        // This crashes for some reason
        //executionContext = executionContext->parent;
    }
}

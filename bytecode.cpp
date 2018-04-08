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
    void ExecuteInteger(Machine& machine, int op, fsh::IntegerPtr lhs, fsh::IntegerPtr rhs)
    {
        switch(op)
        {
        case BC_BINARY_ADD:
            machine.push_data(MakeInteger(lhs->value + rhs->value));
            break;
        case BC_BINARY_SUBTRACT:
            machine.push_data(MakeInteger(lhs->value - rhs->value));
            break;
        case BC_BINARY_MULTIPLY:
            machine.push_data(MakeInteger(lhs->value * rhs->value));
            break;
        case BC_BINARY_DIVIDE:
            machine.push_data(MakeInteger(lhs->value / rhs->value));
            break;
        case BC_RELATIONAL_GT:
            machine.push_data(MakeBoolean(lhs->value > rhs->value));
            break;
        case BC_RELATIONAL_GTE:
            machine.push_data(MakeBoolean(lhs->value >= rhs->value));
            break;
        case BC_RELATIONAL_LT:
            machine.push_data(MakeBoolean(lhs->value < rhs->value));
            break;
        case BC_RELATIONAL_LTE:
            machine.push_data(MakeBoolean(lhs->value <= rhs->value));
            break;
        case BC_RELATIONAL_EQ:
            machine.push_data(MakeBoolean(lhs->value == rhs->value));
            break;
        case BC_RELATIONAL_NEQ:
            machine.push_data(MakeBoolean(lhs->value != rhs->value));
            break;
        }
    }

    void ExecuteFloat(Machine& machine, int op, fsh::FloatPtr lhs, fsh::FloatPtr rhs)
    {
        switch(op)
        {
        case BC_BINARY_ADD:
            machine.push_data(MakeFloat(lhs->value + rhs->value));
            break;
        case BC_BINARY_SUBTRACT:
            machine.push_data(MakeFloat(lhs->value - rhs->value));
            break;
        case BC_BINARY_MULTIPLY:
            machine.push_data(MakeFloat(lhs->value * rhs->value));
            break;
        case BC_BINARY_DIVIDE:
            machine.push_data(MakeFloat(lhs->value / rhs->value));
            break;
        case BC_RELATIONAL_GT:
            machine.push_data(MakeBoolean(lhs->value > rhs->value));
            break;
        case BC_RELATIONAL_GTE:
            machine.push_data(MakeBoolean(lhs->value >= rhs->value));
            break;
        case BC_RELATIONAL_LT:
            machine.push_data(MakeBoolean(lhs->value < rhs->value));
            break;
        case BC_RELATIONAL_LTE:
            machine.push_data(MakeBoolean(lhs->value <= rhs->value));
            break;
        case BC_RELATIONAL_EQ:
            machine.push_data(MakeBoolean(lhs->value == rhs->value));
            break;
        case BC_RELATIONAL_NEQ:
            machine.push_data(MakeBoolean(lhs->value != rhs->value));
            break;
        }
    }

    void ExecuteString(Machine& machine, int op, fsh::StringPtr lhs, fsh::StringPtr rhs)
    {
        switch(op)
        {
        case BC_BINARY_ADD:
            machine.push_data(MakeString(lhs->value + rhs->value));
            break;
        case BC_RELATIONAL_EQ:
            machine.push_data(MakeBoolean(lhs->value == rhs->value));
            break;
        case BC_RELATIONAL_NEQ:
            machine.push_data(MakeBoolean(lhs->value != rhs->value));
            break;
        default:
            throw std::runtime_error("Invalid operator with strings");
        }
    }

    void binary_operator(Machine& machine, int op)
    {
        ElementPtr lhs = machine.pop_data();
        ElementPtr rhs = machine.pop_data();
        ElementPtr ldata = machine.resolve(lhs);
        ElementPtr rdata = machine.resolve(rhs);
        if (op == BC_LOGICAL_AND)
        {
            bool b = machine.ConvertToBool(ldata);
            if (b == false)
            {
                ElementPtr e = MakeBoolean(false);
                machine.push_data(e);
                return;
            }
            b = machine.ConvertToBool(rdata);
            if (b == false)
            {
                ElementPtr e = MakeBoolean(false);
                machine.push_data(e);
                return;
            }
            ElementPtr e = MakeBoolean(true);
            machine.push_data(e);
            return;
        }
        if (op == BC_LOGICAL_OR)
        {
            bool b = machine.ConvertToBool(ldata);
            if (b == true)
            {
                ElementPtr e = MakeBoolean(true);
                machine.push_data(e);
                return;
            }
            b = machine.ConvertToBool(rdata);
            if (b == false)
            {
                ElementPtr e = MakeBoolean(false);
                machine.push_data(e);
                return;
            }
            ElementPtr e = MakeBoolean(true);
            machine.push_data(e);
            return;
        }
        
        if (rdata->IsString() && ldata->IsString())
        {
            ExecuteString(machine, op, ldata.cast<fsh::String>(), rdata.cast<fsh::String>());
        }
        else if (rdata->IsFloat() && ldata->IsInteger())
        {
            fsh::FloatPtr l = MakeFloat((double)ldata.cast<fsh::Integer>()->value);
            fsh::FloatPtr r = rdata.cast<fsh::Float>();
            ExecuteFloat(machine, op, l, r);
        }
        else if (rdata->IsInteger() && ldata->IsFloat())
        {
            fsh::FloatPtr r = MakeFloat((double)rdata.cast<fsh::Integer>()->value);
            ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), r);
        }
        else if (rdata->IsFloat() && ldata->IsFloat())
        {
            ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), rdata.cast<fsh::Float>());
        }
        else if (rdata->IsInteger() && ldata->IsInteger())
        {
            ExecuteInteger(machine, op, ldata.cast<fsh::Integer>(), rdata.cast<fsh::Integer>());
        }
        else
        {
            std::stringstream strm;
            strm << "Binary Operator: Invalid types";
            throw std::runtime_error(strm.str());
        }
    }

//#define DEBUGLOG
#if defined(DEBUGLOG)
#define LOG std::cout 
#else
#define LOG false && std::cout
#endif

    void Execute(Machine& machine, ByteCode& bc, size_t& ip)
    {
        switch(bc[ip])
        {
        case BC_BINARY_ADD:
            LOG << "BINARY_ADD" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_BINARY_SUBTRACT:
            LOG << "BINARY_SUBTRACT" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_BINARY_MULTIPLY:
            LOG << "BINARY_MULTIPLY" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_BINARY_DIVIDE:
            LOG << "BINARY_DIVIDE" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_RELATIONAL_GT:
            LOG << "BC_RELATIONAL_GT" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_RELATIONAL_GTE:
            LOG << "BC_RELATIONAL_GTE" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_RELATIONAL_LT:
            LOG << "BC_RELATIONAL_LT" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_RELATIONAL_LTE:
            LOG << "BC_RELATIONAL_LTE" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_RELATIONAL_EQ:
            LOG << "BC_RELATIONAL_EQ" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_RELATIONAL_NEQ:
            LOG << "BC_RELATIONAL_NEQ" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_LOGICAL_AND:
            LOG << "BC_LOGICAL_AND" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_LOGICAL_OR:
            LOG << "BC_LOGICAL_OR" << std::endl;
            binary_operator(machine, bc[ip]);
            break;
        case BC_UNARY_NEGATE:
            {
                LOG << "BC_UNARY_NEGATE" << std::endl;
                ElementPtr e = machine.pop_data();
                e = machine.resolve(e);
                if (e->IsInteger())
                    e = MakeInteger(-e.cast<Integer>()->value);
                else if (e->IsFloat())
                    e = MakeFloat(-e.cast<Float>()->value);
                else
                    throw std::runtime_error("Unary minus: Invalid operand");
                machine.push_data(e);
            }
            break;
        case BC_LOAD_NONE:
            {
                LOG << "BC_LOAD_NONE" << std::endl;
                machine.push_data(MakeNone());
            }
            break;
        case BC_LOAD_TRUE:
            {
                LOG << "BC_LOAD_TRUE" << std::endl;
                machine.push_data(MakeBoolean(true));
            }
            break;
        case BC_LOAD_FALSE:
            {
                LOG << "BC_LOAD_FALSE" << std::endl;
                machine.push_data(MakeBoolean(false));
            }
            break;
        case BC_LOAD_INTEGER:
            {
                ++ip;
                machine.push_data(MakeInteger(bc[ip]));
                LOG << "BC_LOAD_INTEGER " << bc[ip] << std::endl;
            }
            break;
        case BC_LOAD_FLOAT:
            {
                ++ip;
                double *d = (double *)&bc.byte_codes[ip];
                machine.push_data(MakeFloat(*d));
                LOG << "BC_LOAD_FLOAT" << *d << std::endl;
            }
            break;
        case BC_LOAD_STRING:
            {
                ++ip;
                std::string s = machine.string_table_get(bc[ip]);
                machine.push_data(MakeString(s));
                LOG << "BC_LOAD_STRING " << s << std::endl;
            }
            break;
        case BC_LOAD_IDENTIFIER:
            {
                ++ip;
                std::string s = machine.string_table_get(bc[ip]);
                machine.push_data(MakeIdentifier(s));
                LOG << "BC_LOAD_IDENTIFIER " << s << std::endl;
            }
            break;
        case BC_STORE_VAR:
            {
                LOG << "BC_STORE_VAR" << std::endl;
                ElementPtr id = machine.pop_data();
                ElementPtr v = machine.pop_data();
                if (!id->IsIdentifier())
                    throw std::runtime_error("Expected identifier");
                machine.store_variable(id.cast<Identifier>()->value, v);
                machine.push_data(v);
            }
            break;
        case BC_JUMP_IF_FALSE:
            {
                LOG << "BC_JUMP_IF_FALSE" << std::endl;
                ElementPtr e = machine.pop_data();
                bool b = machine.ConvertToBool(e);
                ++ip; // pointing to jump location
                if (!b)
                {
                    ip = bc[ip];
                }
            }
            break;
        case BC_JUMP:
            {
                LOG << "BC_JUMP" << std::endl;
                ++ip;
                ip = bc[ip];
            }
            break;
        case BC_CALL:
            {
                LOG << "BC_CALL" << std::endl;
                ElementPtr callId = machine.pop_data();
                callId = machine.resolve(callId);
                if (callId->type() != ELEMENT_TYPE_INTEGER)
                    throw std::runtime_error("Function call requires name/id");

                // Function id
                int64_t id = callId.cast<Integer>()->value;

                ElementPtr numargs = machine.pop_data();
                assert (numargs->type() == ELEMENT_TYPE_INTEGER);

                // Number of args that was pushed
                int64_t num = numargs.cast<Integer>()->value;

                FunctionDefinition *fd = machine.getFunction(id);
                if (fd == nullptr)
                    throw std::runtime_error("Function not found");
                
                if (fd->isBuiltIn)
                {
                    std::vector<ElementPtr> args;
                    while(num)
                    {
                        ElementPtr e = machine.pop_data();
                        e = machine.resolve(e);
                        args.push_back(e);
                        --num;
                    }
                    std::reverse(args.begin(), args.end());
                    try {
                        machine.push_context();
                        ElementPtr e = fd->builtIn(machine, args);
                        machine.push_data(e);
                        machine.pop_context();
                    }
                    catch(std::exception& e)
                    {
                        machine.pop_context();
                        throw;
                    }
                }
                else
                {

                }
            }
            break;
        case BC_LOAD_FUNCTION_DEF:
            {
                ++ip;
                int64_t id = bc[ip];
                machine.push_data(MakeFunctionDef(id));
            }
            break;
        default:
            assert(false);
            break;
        }
    }
}


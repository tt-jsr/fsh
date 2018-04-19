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

    bool ExecuteEQ(Machine& machine, ElementPtr lhs, ElementPtr rhs)
    {
        if(lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(true));
            return true;
        }
        if(!lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(false));
            return true;
        }
        if(lhs->IsNone() && !rhs->IsNone())
        {
            machine.push_data(MakeBoolean(false));
            return true;
        }
        if(lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(lhs.cast<Boolean>()->value == rhs.cast<Boolean>()->value));
            return true;
        }
        if(lhs->IsBoolean() && !rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(false));
            return true;
        }
        if(!lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(false));
            return true;
        }
        return false;
    }

    bool ExecuteNEQ(Machine& machine, ElementPtr lhs, ElementPtr rhs)
    {
        if(lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(false));
            return true;
        }
        if(!lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(true));
            return true;
        }
        if(lhs->IsNone() && !rhs->IsNone())
        {
            machine.push_data(MakeBoolean(true));
            return true;
        }
        if(lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(lhs.cast<Boolean>()->value != rhs.cast<Boolean>()->value));
            return true;
        }
        if(lhs->IsBoolean() && !rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(true));
            return true;
        }
        if(!lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(true));
            return true;
        }
        return false;
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
        if (op == BC_RELATIONAL_EQ)
        {
            if (ExecuteEQ(machine, ldata, rdata))
                return;
        }
        if (op == BC_RELATIONAL_NEQ)
        {
            if (ExecuteNEQ(machine, ldata, rdata))
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

    void Execute(Machine& machine, ByteCode& bc)
    {
        switch(bc[bc.ip])
        {
        case BC_BINARY_ADD:
            LOG << "BINARY_ADD" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_BINARY_SUBTRACT:
            LOG << "BINARY_SUBTRACT" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_BINARY_MULTIPLY:
            LOG << "BINARY_MULTIPLY" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_BINARY_DIVIDE:
            LOG << "BINARY_DIVIDE" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_GT:
            LOG << "BC_RELATIONAL_GT" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_GTE:
            LOG << "BC_RELATIONAL_GTE" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_LT:
            LOG << "BC_RELATIONAL_LT" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_LTE:
            LOG << "BC_RELATIONAL_LTE" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_EQ:
            LOG << "BC_RELATIONAL_EQ" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_NEQ:
            LOG << "BC_RELATIONAL_NEQ" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_LOGICAL_AND:
            LOG << "BC_LOGICAL_AND" << std::endl;
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_LOGICAL_OR:
            LOG << "BC_LOGICAL_OR" << std::endl;
            binary_operator(machine, bc[bc.ip]);
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
                ++bc.ip;
                machine.push_data(MakeInteger(bc[bc.ip]));
                LOG << "BC_LOAD_INTEGER " << bc[bc.ip] << std::endl;
            }
            break;
        case BC_LOAD_FLOAT:
            {
                ++bc.ip;
                double *d = (double *)&bc.byte_codes[bc.ip];
                machine.push_data(MakeFloat(*d));
                LOG << "BC_LOAD_FLOAT" << *d << std::endl;
            }
            break;
        case BC_LOAD_STRING:
            {
                ++bc.ip;
                std::string *ps = (std::string *)bc[bc.ip];
                machine.push_data(MakeString(*ps));
                LOG << "BC_LOAD_STRING " << *ps << std::endl;
            }
            break;
        case BC_LOAD_IDENTIFIER:
            {
                ++bc.ip;
                std::string *ps = (std::string *)bc[bc.ip];
                machine.push_data(MakeIdentifier(*ps));
                LOG << "BC_LOAD_IDENTIFIER " << *ps << std::endl;
            }
            break;
        case BC_RESOLVE:
            {
                ElementPtr e = machine.pop_data();
                e = machine.resolve(e);
                machine.push_data(e);
                LOG << "BC_RESOLVE " << std::endl;
            }
            break;
        case BC_INCREMENT_LOCATION:
            {
                ++bc.ip;
                size_t location = bc[bc.ip];
                ++bc[location];
            }
            break;
        case BC_LOAD_INTEGER_LOCATION:
            {
                ++bc.ip;
                size_t location = bc[bc.ip];
                machine.push_data(MakeInteger(bc[location]));
                LOG << "BC_INTEGER_LOCATION " << bc[location] << " from " << location << std::endl;
            }
            break;
        case BC_STORE_VAR:
            {
                ElementPtr id = machine.pop_data();
                ElementPtr v = machine.pop_data();
                if (!id->IsIdentifier())
                    throw std::runtime_error("Expected identifier");
                machine.store_variable(id.cast<Identifier>()->value, v);
                machine.push_data(v);
                LOG << "BC_STORE_VAR " << id.cast<Identifier>()->value << std::endl;
            }
            break;
        case BC_LOAD_LIST_ITEM:
            {
                ElementPtr idx = machine.pop_data();
                ElementPtr lst = machine.pop_data();
                if (!idx->IsInteger())
                    throw std::runtime_error("BC_LOAD_LIST_ITEM Expected integer");
                if (!lst->IsList())
                    throw std::runtime_error("BC_LOAD_LIST_ITEM Expected list");
                int n = idx.cast<Integer>()->value;
                LOG << "BC_LOAD_LIST_ITEM " << n << std::endl;
                auto& vec = lst.cast<List>()->items;
                if (n < 0 || n >= vec.size())
                {
                    machine.set_gp_register(false);
                }
                else
                {
                    machine.set_gp_register(true);
                    machine.push_data(vec[n]);
                }
            }
            break;
        case BC_JUMP_GP_FALSE:
            {
                ++bc.ip; // pointing to jump location
                if (machine.get_gp_register() == false)
                {
                    LOG << "BC_JUMP_GP_FALSE : jump taken" << std::endl;
                    bc.ip = bc[bc.ip];
                }
                else
                    LOG << "BC_JUMP_GP_FALSE : jump not taken" << std::endl;
            }
            break;
        case BC_JUMP_IF_FALSE:
            {
                LOG << "BC_JUMP_IF_FALSE" << std::endl;
                ElementPtr e = machine.pop_data();
                bool b = machine.ConvertToBool(e);
                ++bc.ip; // pointing to jump location
                if (!b)
                {
                    bc.ip = bc[bc.ip];
                }
            }
            break;
        case BC_JUMP:
            {
                LOG << "BC_JUMP" << std::endl;
                ++bc.ip;
                bc.ip = bc[bc.ip];
            }
            break;
        case BC_DATA:
                LOG << "BC_DATA" << std::endl;
                ++bc.ip;  // skip data, don't do anything
                break;
        case BC_CALL:
            {
                ElementPtr callId = machine.pop_data();
                if (callId->type() != ELEMENT_TYPE_FUNCTION_DEF_ID)
                    throw std::runtime_error("Function call requires name/id");

                // Function id
                int64_t id = callId.cast<FunctionDefId>()->funcid;
                LOG << "BC_CALL " << id << std::endl;

                ElementPtr numargs = machine.pop_data();
                assert (numargs->type() == ELEMENT_TYPE_INTEGER);

                // Number of args that was pushed
                int64_t num = numargs.cast<Integer>()->value;

                FunctionDefinition *fd = machine.getFunction(id);
                if (fd == nullptr)
                    throw std::runtime_error("Function not found");
                
                ElementPtr rtn = fd->Call(machine, num);
                machine.push_data(rtn);
            }
            break;
        case BC_BIND:
            {
                ElementPtr callId = machine.pop_data();
                if (callId->type() != ELEMENT_TYPE_FUNCTION_DEF_ID)
                    throw std::runtime_error("Function call requires name/id");

                // Target function id
                int64_t id = callId.cast<FunctionDefId>()->funcid;
                LOG << "BC_BIND " << id << std::endl;

                ElementPtr numargs = machine.pop_data();
                assert (numargs->type() == ELEMENT_TYPE_INTEGER);

                // Number of args that was pushed
                int64_t num = numargs.cast<Integer>()->value;

                FunctionDefinition *fd = machine.getFunction(id);
                if (fd == nullptr)
                    throw std::runtime_error("Function not found");

                BoundFunction *pbf = new BoundFunction();
                while (num)
                {
                    ElementPtr e = machine.pop_data();
                    pbf->bound_args.push_back(e);
                    --num;
                }

                pbf->target = fd;
                ++bc.ip;
                if (bc[bc.ip])
                    pbf->attributes = *(ByteCode *)bc[bc.ip];
                id = machine.registerFunction(pbf);
                machine.push_data(MakeFunctionDefId(id));
            }
            break;
        case BC_LOAD_FUNCTION_DEF:
            {
                ++bc.ip;
                int64_t id = bc[bc.ip];
                machine.push_data(MakeFunctionDefId(id));
                LOG << "BC_LOAD_FUNCTION_DEF " << id << std::endl;
            }
            break;
        case BC_SYSTEM:
            {
                ++bc.ip;
                int64_t id = bc[bc.ip];
                std::string cmd = machine.string_table_get(id);
                system(cmd.c_str());
                machine.push_data(MakeNone());
            }
            break;
        case BC_PUSH_CONTEXT:
            machine.push_context();
            break;
        case BC_POP:
            machine.pop_data();
            break;
        case BC_TRY:
            {
                size_t try_id = bc[++bc.ip];
                size_t catch_id = bc[++bc.ip];
                ByteCode *bctry = machine.getBlock(try_id);
                ByteCode *bccatch = machine.getBlock(catch_id);
                assert(bctry);
                assert(bccatch);
                try
                {
                    bctry->ip = 0;
                    while(bctry->ip < bctry->size())
                    {
                        fsh::Execute(machine, *bctry);
                        ++bctry->ip;
                    }
                }
                catch (std::exception& e)
                {
                    ElementPtr ex = MakeString(e.what());
                    machine.store_variable("_exception", ex);
                    bccatch->ip = 0;
                    while(bccatch->ip < bccatch->size())
                    {
                        fsh::Execute(machine, *bccatch);
                        ++bccatch->ip;
                    }
                }
            }
            break;
        default:
            assert(false);
            break;
        }
    }
}


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
            //machine.log() << "BC_BINARY_ADD:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_BINARY_SUBTRACT:
            machine.push_data(MakeInteger(lhs->value - rhs->value));
            //machine.log() << "BC_BINARY_SUBTRACT:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_BINARY_MULTIPLY:
            machine.push_data(MakeInteger(lhs->value * rhs->value));
            //machine.log() << "BC_BINARY_MULTIPLY:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_BINARY_DIVIDE:
            machine.push_data(MakeInteger(lhs->value / rhs->value));
            //machine.log() << "BC_BINARY_DIVIDE:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_GT:
            machine.push_data(MakeBoolean(lhs->value > rhs->value));
            //machine.log() << "BC_RELATIONAL_GT:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_GTE:
            machine.push_data(MakeBoolean(lhs->value >= rhs->value));
            //machine.log() << "BC_RELATIONAL_GTE:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_LT:
            machine.push_data(MakeBoolean(lhs->value < rhs->value));
            //machine.log() << "BC_RELATIONAL_LT:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_LTE:
            machine.push_data(MakeBoolean(lhs->value <= rhs->value));
            //machine.log() << "BC_RELATIONAL_LTE:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_EQ:
            machine.push_data(MakeBoolean(lhs->value == rhs->value));
            //machine.log() << "BC_RELATIONAL_EQ:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_NEQ:
            machine.push_data(MakeBoolean(lhs->value != rhs->value));
            //machine.log() << "BC_RELATIONAL_NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        }
    }

    void ExecuteFloat(Machine& machine, int op, fsh::FloatPtr lhs, fsh::FloatPtr rhs)
    {
        switch(op)
        {
        case BC_BINARY_ADD:
            machine.push_data(MakeFloat(lhs->value + rhs->value));
            //machine.log() << "BC_BINARY_ADD:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_BINARY_SUBTRACT:
            machine.push_data(MakeFloat(lhs->value - rhs->value));
            //machine.log() << "BC_BINARY_SUBTRACT:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_BINARY_MULTIPLY:
            machine.push_data(MakeFloat(lhs->value * rhs->value));
            //machine.log() << "BC_BINARY_MULTIPLY:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_BINARY_DIVIDE:
            machine.push_data(MakeFloat(lhs->value / rhs->value));
            //machine.log() << "BC_BINARY_DIVIDE:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_GT:
            machine.push_data(MakeBoolean(lhs->value > rhs->value));
            //machine.log() << "BC_RELATIONAL_GT:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_GTE:
            machine.push_data(MakeBoolean(lhs->value >= rhs->value));
            //machine.log() << "BC_RELATIONAL_GTE:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_LT:
            machine.push_data(MakeBoolean(lhs->value < rhs->value));
            //machine.log() << "BC_RELATIONAL_LT:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_LTE:
            machine.push_data(MakeBoolean(lhs->value <= rhs->value));
            //machine.log() << "BC_RELATIONAL_LTE:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_EQ:
            machine.push_data(MakeBoolean(lhs->value == rhs->value));
            //machine.log() << "BC_RELATIONAL_EQ:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_NEQ:
            machine.push_data(MakeBoolean(lhs->value != rhs->value));
            //machine.log() << "BC_RELATIONAL_NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        }
    }

    void ExecuteString(Machine& machine, int op, fsh::StringPtr lhs, fsh::StringPtr rhs)
    {
        switch(op)
        {
        case BC_BINARY_ADD:
            machine.push_data(MakeString(lhs->value + rhs->value));
            //machine.log() << "BC_BINARY_ADD:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_EQ:
            machine.push_data(MakeBoolean(lhs->value == rhs->value));
            //machine.log() << "BC_RELATIONAL_EQ:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_RELATIONAL_NEQ:
            machine.push_data(MakeBoolean(lhs->value != rhs->value));
            //machine.log() << "BC_RELATIONAL_NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        default:
            throw std::runtime_error("Invalid operator with strings");
        }
    }

    bool ExecuteEQ(Machine& machine, ElementPtr lhs, ElementPtr rhs)
    {
        if (lhs->type() != rhs->type())
        {
            machine.push_data(MakeBoolean(false));
            //machine.log() << "EQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(true));
            //machine.log() << "EQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(!lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(false));
            //machine.log() << "EQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsNone() && !rhs->IsNone())
        {
            machine.push_data(MakeBoolean(false));
            //machine.log() << "EQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(lhs.cast<Boolean>()->value == rhs.cast<Boolean>()->value));
            //machine.log() << "EQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsBoolean() && !rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(false));
            //machine.log() << "EQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(!lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(false));
            //machine.log() << "EQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        return false;
    }

    bool ExecuteNEQ(Machine& machine, ElementPtr lhs, ElementPtr rhs)
    {
        if (lhs->type() != rhs->type())
        {
            machine.push_data(MakeBoolean(true));
            //machine.log() << "NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(false));
            //machine.log() << "NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(!lhs->IsNone() && rhs->IsNone())
        {
            machine.push_data(MakeBoolean(true));
            //machine.log() << "NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsNone() && !rhs->IsNone())
        {
            machine.push_data(MakeBoolean(true));
            //machine.log() << "NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(lhs.cast<Boolean>()->value != rhs.cast<Boolean>()->value));
            //machine.log() << "NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(lhs->IsBoolean() && !rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(true));
            //machine.log() << "NEQ:" << toString(machine, machine.peek_data()) << std::endl;
            return true;
        }
        if(!lhs->IsBoolean() && rhs->IsBoolean())
        {
            machine.push_data(MakeBoolean(true));
            //machine.log() << "NEQ:" << toString(machine, machine.peek_data()) << std::endl;
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
        if (ldata->IsIdentifier())
        {
            IdentifierPtr id = ldata.cast<Identifier>();
            std::stringstream strm;
            strm << "Unknown identifier \"" << id->value << "\"";
            throw std::runtime_error(strm.str());
        }
        if (rdata->IsIdentifier())
        {
            IdentifierPtr id = rdata.cast<Identifier>();
            std::stringstream strm;
            strm << "Unknown identifier \"" << id->value << "\"";
            throw std::runtime_error(strm.str());
        }

        if (op == BC_LOGICAL_AND)
        {
            bool b = machine.ConvertToBool(ldata);
            if (b == false)
            {
                ElementPtr e = MakeBoolean(false);
                machine.push_data(e);
                //machine.log() << "BC_LOGICAL_AND:" << toString(machine, machine.peek_data()) << std::endl;
                return;
            }
            b = machine.ConvertToBool(rdata);
            if (b == false)
            {
                ElementPtr e = MakeBoolean(false);
                machine.push_data(e);
                //machine.log() << "BC_LOGICAL_AND:" << toString(machine, machine.peek_data()) << std::endl;
                return;
            }
            ElementPtr e = MakeBoolean(true);
            machine.push_data(e);
            //machine.log() << "BC_LOGICAL_AND:" << toString(machine, machine.peek_data()) << std::endl;
            return;
        }
        if (op == BC_LOGICAL_OR)
        {
            bool b = machine.ConvertToBool(ldata);
            if (b == true)
            {
                ElementPtr e = MakeBoolean(true);
                machine.push_data(e);
                //machine.log() << "BC_LOGICAL_OR:" << toString(machine, machine.peek_data()) << std::endl;
                return;
            }
            b = machine.ConvertToBool(rdata);
            if (b == false)
            {
                ElementPtr e = MakeBoolean(false);
                machine.push_data(e);
                //machine.log() << "BC_LOGICAL_OR:" << toString(machine, machine.peek_data()) << std::endl;
                return;
            }
            ElementPtr e = MakeBoolean(true);
            machine.push_data(e);
            //machine.log() << "BC_LOGICAL_OR:" << toString(machine, machine.peek_data()) << std::endl;
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
            strm << "Binary Operator: Invalid types lhs: " << ldata->type() << " rhs: " << rdata->type();
            //machine.log() << strm.str() << std::endl;
            throw std::runtime_error(strm.str());
        }
    }

    bool ExecuteImpl(Machine& machine, ByteCode& bc)
    {
        //machine.log() << bc.ip << ": ";
        switch(bc[bc.ip])
        {
        case BC_BINARY_ADD:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_BINARY_SUBTRACT:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_BINARY_MULTIPLY:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_BINARY_DIVIDE:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_GT:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_GTE:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_LT:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_LTE:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_EQ:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_RELATIONAL_NEQ:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_LOGICAL_AND:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_LOGICAL_OR:
            binary_operator(machine, bc[bc.ip]);
            break;
        case BC_UNARY_NEGATE:
            {
                ElementPtr e = machine.pop_data();
                e = machine.resolve(e);
                if (e->IsInteger())
                    e = MakeInteger(-e.cast<Integer>()->value);
                else if (e->IsFloat())
                    e = MakeFloat(-e.cast<Float>()->value);
                else
                    throw std::runtime_error("Unary minus: Invalid operand");
                machine.push_data(e);
                //machine.log() << "BC_UNARY_NEGATE:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_NONE:
            {
                machine.push_data(MakeNone());
                //machine.log() << "BC_LOAD_NONE:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_TRUE:
            {
                machine.push_data(MakeBoolean(true));
                //machine.log() << "BC_LOAD_TRUE:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_FALSE:
            {
                machine.push_data(MakeBoolean(false));
                //machine.log() << "BC_LOAD_FALSE:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_INTEGER:
            {
                ++bc.ip;
                machine.push_data(MakeInteger(bc[bc.ip]));
                //machine.log() << "BC_LOAD_INTEGER:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_FLOAT:
            {
                ++bc.ip;
                double *d = (double *)&bc.byte_codes[bc.ip];
                machine.push_data(MakeFloat(*d));
                //machine.log() << "BC_LOAD_FLOAT:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_STRING:
            {
                ++bc.ip;
                std::string *ps = (std::string *)bc[bc.ip];
                machine.push_data(MakeString(*ps));
                //machine.log() << "BC_LOAD_STRING:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_IDENTIFIER:
            {
                ++bc.ip;
                std::string *ps = (std::string *)bc[bc.ip];
                machine.push_data(MakeIdentifier(*ps));
                //machine.log() << "BC_LOAD_IDENTIFIER: " << *ps << " as " << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_RESOLVE:
            {
                ElementPtr e = machine.pop_data();
                e = machine.resolve(e);
                machine.push_data(e);
                //machine.log() << "BC_RESOLVE: type=" << e->type() << " val: " << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_INCREMENT_LOCATION:
            {
                ++bc.ip;
                size_t location = bc[bc.ip];
                ++bc[location];
                //machine.log() << "BC_INCREMENT_LOCATION" << std::endl;
            }
            break;
        case BC_STORE_LOCATION:
            {
                ++bc.ip;
                size_t location = bc[bc.ip];
                ++bc.ip;
                int64_t value = bc[bc.ip];
                bc[location] = value;
                //machine.log() << "BC_STORE_LOCATION" << std::endl;
            }
            break;
        case BC_LOAD_INTEGER_LOCATION:
            {
                ++bc.ip;
                size_t location = bc[bc.ip];
                machine.push_data(MakeInteger(bc[location]));
                //machine.log() << "BC_LOAD_INTEGER_LOCATION:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_DELETE_ITERATOR:
            {
                ++bc.ip;
                int64_t location = bc[bc.ip];
                Element *pElement = (Element *)bc[location];
                if (pElement)
                    pElement->dec_ref();
                bc[location] = 0;
                //machine.log() << "BC_DELETE_ITERATOR:" << std::endl;
            }
            break;
        case BC_INCREMENT_ITERATOR:
            {
                ++bc.ip;
                size_t iteratorIP = bc[bc.ip];
                int64_t id = bc[iteratorIP];
                ElementPtr cont = machine.pop_data();
                if (cont->IsList())
                {
                    Integer *raw = (Integer *)bc[iteratorIP];
                    if (raw == nullptr)
                    {
                        raw = new Integer(0);
                        raw->inc_ref();
                        bc[iteratorIP] = (uintptr_t)raw;
                    }
                    else
                    {
                        assert(raw->IsInteger());
                        ++raw->value;
                    }
                    machine.push_data(cont);
                    //machine.log() << "BC_INCREMENT_ITERATOR list cont:" << toString(machine, machine.peek_data()) << std::endl;
                    IntegerPtr ip(raw);
                    machine.push_data(ip);
                    //machine.log() << "BC_INCREMENT_ITERATOR list idx:" << toString(machine, machine.peek_data()) << std::endl;
                }
                else if (cont->IsMap())
                {
                    ElementPtr key;
                    MapPtr m = cont.cast<Map>();
                    if (bc[iteratorIP] == 0)
                    {
                        auto it = m->map.begin();
                        if (it != m->map.end())
                        {
                            key = it->first;
                            bc[iteratorIP] = (uintptr_t)key.get();
                        }
                        else
                        {
                            key = MakeString("EnDoFmApItErAtOr");
                        }
                    }
                    else
                    {
                        key = (Element *)bc[iteratorIP];
                        auto it = m->map.find(key);
                        if (it != m->map.end())
                        {
                            ++it;
                        }
                        if (it != m->map.end())
                        {
                            key = it->first;
                            bc[iteratorIP] = (uintptr_t)key.get();
                        }
                        else
                        {
                            key = MakeString("EnDoFmApItErAtOr");
                        }
                    }
                    machine.push_data(m);
                    //machine.log() << "BC_INCREMENT_ITERATOR map cont:" << toString(machine, machine.peek_data()) << std::endl;
                    machine.push_data(key);
                    //machine.log() << "BC_INCREMENT_ITERATOR map key:" << toString(machine, machine.peek_data()) << std::endl;
                }
                else
                {
                    throw std::runtime_error("Iterator requires list or map");
                }
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
                //machine.log() << "BC_STORE_VAR:" << toString(machine, machine.peek_data()) << " to " << 
                    //id.cast<Identifier>()->value << std::endl;
            }
            break;
        case BC_LOAD_CONTAINER_ITEM:
            {
                ElementPtr key = machine.pop_data();
                ElementPtr C = machine.pop_data();
                if (C->IsList())
                {
                    if (!key->IsInteger())
                        throw std::runtime_error("BC_LOAD_CONTAINER_ITEM Expected integer");
                    int n = key.cast<Integer>()->value;
                    auto& vec = C.cast<List>()->items;
                    if (n < 0 || n >= vec.size())
                    {
                        machine.set_gp_register(false);
                        machine.push_data(MakeNone());
                        //machine.log() << "BC_LOAD_CONRTAINER_ITEM (list):" << toString(machine, machine.peek_data()) << std::endl;
                    }
                    else
                    {
                        machine.set_gp_register(true);
                        machine.push_data(vec[n]);
                        //machine.log() << "BC_LOAD_CONRTAINER_ITEM (list):" << toString(machine, machine.peek_data()) << std::endl;
                    }
                }
                else if (C->IsMap())
                {
                    MapPtr m = C.cast<Map>();
                    auto it = m->map.find(key);
                    if (it == m->map.end())
                    {
                        machine.set_gp_register(false);
                        machine.push_data(MakeNone());
                        //machine.log() << "BC_LOAD_CONRTAINER_ITEM (map):" << toString(machine, machine.peek_data()) << std::endl;
                    }
                    else
                    {
                        machine.set_gp_register(true);
                        ElementPtr pr = MakePair(it->first, it->second);
                        machine.push_data(pr);
                        //machine.log() << "BC_LOAD_CONRTAINER_ITEM (map):" << toString(machine, machine.peek_data()) << std::endl;
                    }
                }
                else
                    throw std::runtime_error("FOR: expected list or map");
            }
            break;
        case BC_JUMP_GP_FALSE:
            {
                ++bc.ip; // pointing to jump location
                if (machine.get_gp_register() == false)
                {
                    bc.ip = bc[bc.ip];
                    //machine.log() << "BC_JUMP_GP_FALSE, jump taken, jump to " << bc.ip << std::endl;
                }
                //else
                    //machine.log() << "BC_JUMP_GP_FALSE, jump not taken:" << std::endl;
            }
            break;
        case BC_JUMP_IF_FALSE:
            {
                ElementPtr e = machine.pop_data();
                bool b = machine.ConvertToBool(e);
                ++bc.ip; // pointing to jump location
                if (!b)
                {
                    bc.ip = bc[bc.ip];
                    //machine.log() << "BC_JUMP_IF_FALSE, jump taken, jump to " << bc.ip << std::endl;
                }
                //else 
                    //machine.log() << "BC_JUMP_IF_FALSE, jump not taken:" << std::endl;
            }
            break;
        case BC_JUMP:
            {
                ++bc.ip;
                bc.ip = bc[bc.ip];
                //machine.log() << "BC_JUMP to " << bc.ip << std::endl;
            }
            break;
        case BC_RETURN:
            //machine.log() << "BC_RETURN" << std::endl;
            return false;
        case BC_DATA:
            //machine.log() << "BC_DATA" << std::endl;
            ++bc.ip;  // skip data, don't do anything
            break;
        case BC_CALL:
            {
                ElementPtr callId = machine.pop_data();
                if (callId->type() != ELEMENT_TYPE_FUNCTION_DEF_ID)
                    throw std::runtime_error("Function call requires name/id");

                // Function id
                uintptr_t id = callId.cast<FunctionDefId>()->funcid;

                ElementPtr numargs = machine.pop_data();
                assert (numargs->type() == ELEMENT_TYPE_INTEGER);

                // Number of args that was pushed
                uintptr_t num = numargs.cast<Integer>()->value;

                FunctionDefinition *fd = machine.getFunction(id);
                if (fd == nullptr)
                    throw std::runtime_error("Function not found");
                
                //machine.log() << "BC_CALL:" << std::endl;
                ElementPtr rtn = fd->Call(machine, num);
                machine.push_data(rtn);
                //machine.log() << "BC_CALL rtn:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_BIND:
            {
                ElementPtr callId = machine.pop_data();
                if (callId->type() != ELEMENT_TYPE_FUNCTION_DEF_ID)
                    throw std::runtime_error("Function call requires name/id");

                // Target function id
                uintptr_t id = callId.cast<FunctionDefId>()->funcid;

                ElementPtr numargs = machine.pop_data();
                assert (numargs->type() == ELEMENT_TYPE_INTEGER);

                // Number of args that was pushed
                uintptr_t num = numargs.cast<Integer>()->value;

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
                std::reverse(pbf->bound_args.begin(), pbf->bound_args.end());

                pbf->target = fd;
                ++bc.ip;
                if (bc[bc.ip])
                    pbf->attributes = *(ByteCode *)bc[bc.ip];
                id = machine.registerFunction(pbf);
                machine.push_data(MakeFunctionDefId(id));
                //machine.log() << "BC_BIND:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_LOAD_FUNCTION_DEF:
            {
                ++bc.ip;
                uintptr_t id = bc[bc.ip];
                machine.push_data(MakeFunctionDefId(id));
                //machine.log() << "BC_LOAD_FUNCTION_DEF:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_SYSTEM:
            {
                ++bc.ip;
                uintptr_t id = bc[bc.ip];
                std::string cmd = machine.string_table_get(id);
                system(cmd.c_str());
                machine.push_data(MakeNone());
                //machine.log() << "BC_SYSTEM:" << toString(machine, machine.peek_data()) << std::endl;
            }
            break;
        case BC_PUSH_CONTEXT:
            machine.push_context();
            //machine.log() << "BC_PUSH_CONTEXT:" << std::endl;
            break;
        case BC_POP:
            machine.pop_data();
            //machine.log() << "BC_POP:" << toString(machine, machine.peek_data()) << std::endl;
            break;
        case BC_TRY:
            {
                size_t try_id = bc[++bc.ip];
                size_t catch_id = bc[++bc.ip];
                ByteCode *bctry = machine.getBlock(try_id);
                ByteCode *bccatch = machine.getBlock(catch_id);
                assert(bctry);
                assert(bccatch);
                //machine.log() << "BC_TRY enter try"  << std::endl;
                try
                {
                    bctry->ip = 0;
                    while(bctry->ip < bctry->size())
                    {
                        if (false == fsh::Execute(machine, *bctry))
                            return true;
                        ++bctry->ip;
                    }
                    //machine.log() << "BC_TRY return from try"  << std::endl;
                }
                catch (std::exception& e)
                {
                    ElementPtr ex = MakeString(e.what());
                    machine.store_variable("_exception", ex);
                    bccatch->ip = 0;
                    //machine.log() << "BC_TRY enter catch"  << std::endl;
                    while(bccatch->ip < bccatch->size())
                    {
                        if (false == fsh::Execute(machine, *bccatch))
                            return true;
                        ++bccatch->ip;
                    }
                    //machine.log() << "BC_TRY return catch"  << std::endl;
                }
            }
            break;
        default:
            assert(false);
            break;
        }
        return true;
    }
    bool Execute(Machine& machine, ByteCode& bc)
    {
        bool b = ExecuteImpl(machine, bc);
        return b;
    }
}


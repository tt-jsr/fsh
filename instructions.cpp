#include <sstream>
#include <stdexcept>
#include <iostream>
#include "common.h"
#include "common.h"
#include "instructions.h"
#include "machine.h"
#include "parser.h"

namespace fsh
{
    
    namespace instruction
    {
        /*****************************************************/
        void ExecuteInteger(Machine& machine, int op, fsh::IntegerPtr lhs, fsh::IntegerPtr rhs)
        {
            switch(op)
            {
            case TOKEN_PLUS:
                machine.push_data(MakeInteger(lhs->value + rhs->value));
                break;
            case TOKEN_MINUS:
                machine.push_data(MakeInteger(lhs->value - rhs->value));
                break;
            case TOKEN_MULTIPLY:
                machine.push_data(MakeInteger(lhs->value * rhs->value));
                break;
            case TOKEN_DIVIDE:
                machine.push_data(MakeInteger(lhs->value / rhs->value));
                break;
            }
        }

        void ExecuteFloat(Machine& machine, int op, fsh::FloatPtr lhs, fsh::FloatPtr rhs)
        {
            switch(op)
            {
            case TOKEN_PLUS:
                machine.push_data(MakeFloat(lhs->value + rhs->value));
                break;
            case TOKEN_MINUS:
                machine.push_data(MakeFloat(lhs->value - rhs->value));
                break;
            case TOKEN_MULTIPLY:
                machine.push_data(MakeFloat(lhs->value * rhs->value));
                break;
            case TOKEN_DIVIDE:
                machine.push_data(MakeFloat(lhs->value / rhs->value));
                break;
            }
        }
        BinaryOperator::~BinaryOperator()
        {
        }

        void BinaryOperator::Execute(Machine& machine)
        {
            lhs->Execute(machine);
            rhs->Execute(machine);
            ElementPtr rdata = machine.pop_data();
            ElementPtr ldata = machine.pop_data();
            if (op == TOKEN_ASSIGNMENT)
            {
                if (ldata->type() != fsh::ELEMENT_TYPE_IDENTIFIER)
                {
                    std::stringstream strm;
                    strm << "lhs of assignment is not an identifier";
                    throw std::runtime_error(strm.str());
                }
                machine.store_variable(ldata.cast<fsh::Identifier>()->name, rdata);
                machine.push_data(rdata);
            }
            else
            {
                if (rdata->IsFloat() && ldata->IsInteger())
                {
                    fsh::FloatPtr l = MakeFloat((double)ldata.cast<fsh::Integer>()->value);
                    fsh::FloatPtr r = rdata.cast<fsh::Float>();
                    ExecuteFloat(machine, op, l, r);
                }
                if (rdata->IsInteger() && ldata->IsFloat())
                {
                    fsh::FloatPtr r = MakeFloat((double)rdata.cast<fsh::Integer>()->value);
                    ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), r);
                }
                if (rdata->IsFloat() && ldata->IsFloat())
                {
                    ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), rdata.cast<fsh::Float>());
                }
                if (rdata->IsInteger() && ldata->IsInteger())
                {
                    ExecuteInteger(machine, op, ldata.cast<fsh::Integer>(), rdata.cast<fsh::Integer>());
                }
            }
        }

        void BinaryOperator::dump(std::ostream& strm)
        {
            lhs->dump(strm);
            if (rhs)
            {
                rhs->dump(strm);
            }
            switch(op)
            {
            case TOKEN_PLUS:
                strm << " + ";
                break;
            case TOKEN_MINUS:
                strm << " - ";
                break;
            case TOKEN_MULTIPLY:
                strm << " * ";
                break;
            case TOKEN_DIVIDE:
                strm << " / ";
                break;
            }
        }

        /*****************************************************/
        void FunctionCall::Execute(Machine& machine)
        {
            fsh::ElementPtr e;
            if (machine.get_variable(name, e) == false)
            {
                std::stringstream strm;
                strm << "Function \"" << name << "\" not found";
                throw std::runtime_error(strm.str());
            }
            if (e->IsFunctionBuiltIn() == false)
            {
                std::stringstream strm;
                strm << "Function \"" << name << "\" is not a function";
                throw std::runtime_error(strm.str());
            }

            fsh::FunctionBuiltInPtr func = e.cast<FunctionBuiltIn>();

            if (args)
            {
                for (auto& in : args->expressions)
                {
                    in->Execute(machine);
                    func->args.push_back(machine.pop_data());
                }
            }
            func->Execute(machine);
            machine.push_data(func->returnVal);
        }

        void FunctionCall::dump(std::ostream&)
        {
        }

        /*****************************************************/
        void Identifier::Execute(Machine& machine)
        {
            if (name[0] == '$')
            {
                ElementPtr e;
                if (machine.get_variable(&name[1], e) == false)
                {
                    std::stringstream strm;
                    strm << "Variable \"" << name << "\" not found";
                    throw std::runtime_error(strm.str());
                }
                machine.push_data(e);
            }
            else
            {
                ElementPtr e = fsh::MakeIdentifier(name);
                machine.push_data(e);
            }
        }

        void Identifier::dump(std::ostream& strm)
        {
            strm << " " << name << " ";
        }

        /*****************************************************/
        void Integer::Execute(Machine& machine)
        {
            machine.push_data(MakeInteger(value));
        }

        void Integer::dump(std::ostream& strm)
        {
            strm << " " << value << " ";
        }

        /*****************************************************/

        void Float::Execute(Machine& machine)
        {
            machine.push_data(MakeFloat(value));
        }

        void Float::dump(std::ostream& strm)
        {
            strm << " " << value << " ";
        }

        /**************************************************/
        void ExpressionList::Execute(Machine& machine)
        {
            for (auto &in : expressions)
            {
                in->Execute(machine);
            }
        }

        void ExpressionList::dump(std::ostream& strm)
        {
        }
    }
 }


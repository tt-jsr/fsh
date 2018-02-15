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
        bool GetIntegers(ElementPtr e1, ElementPtr e2, int64_t& v1, int64_t& v2)
        {
            if (e1->type() != ELEMENT_TYPE_INTEGER)
                return false;
            if (e2->type() != ELEMENT_TYPE_INTEGER)
                return false;
            v1 = e1.cast<fsh::Integer>()->value;
            v2 = e2.cast<fsh::Integer>()->value;
            return true;
        }
        /*****************************************************/
        BinaryOperator::~BinaryOperator()
        {
        }

        void BinaryOperator::Execute(Machine& machine)
        {
            lhs->Execute(machine);
            rhs->Execute(machine);
            ElementPtr rdata = machine.pop_data();
            ElementPtr ldata = machine.pop_data();
            int64_t lv, rv;
            if (GetIntegers(ldata, rdata, lv, rv) == false)
                throw std::runtime_error("Expected integer");
            switch(op)
            {
            case TOKEN_PLUS:
                machine.push_data(MakeInteger(lv + rv));
                break;
            case TOKEN_MINUS:
                machine.push_data(MakeInteger(lv - rv));
                break;
            case TOKEN_MULTIPLY:
                machine.push_data(MakeInteger(lv * rv));
                break;
            case TOKEN_DIVIDE:
                machine.push_data(MakeInteger(lv / rv));
                break;
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
        /*
        void Function::Execute(Machine& machine)
        {
        }

        void Function::dump(std::ostream&)
        {
        }
        */

        /*****************************************************/
        void Identifier::Execute(Machine& machine)
        {
            
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
    }
 }


#include <sstream>
#include <stdexcept>
#include <iostream>
#include "common.h"
#include "instructions.h"
#include "machine.h"
#include "parser.h"

namespace fsh
{
    namespace instruction
    {
        /*****************************************************/
        void BinaryOperator::Execute(Machine& machine)
        {
            ElementPtr lhs = machine.pop_data();
            ElementPtr rhs = machine.pop_data();

        }

        void BinaryOperator::dump(std::ostream& strm)
        {
            lhs->dump(strm);
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
            if (rhs)
                rhs->dump(strm);
        }

        /*****************************************************/
        void Function::Execute(Machine& machine)
        {
        }

        void Function::dump(std::ostream&)
        {
        }

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
        }

        void Integer::dump(std::ostream& strm)
        {
            strm << " " << value << " ";
        }
    }
 }


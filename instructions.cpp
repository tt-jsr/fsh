#include <sstream>
#include <stdexcept>
#include <iostream>
#include "common.h"
#include "instructions.h"
#include "machine.h"

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

        /*****************************************************/
        void Function::Execute(Machine& machine)
        {
        }

        /*****************************************************/
        void Identifier::Execute(Machine& machine)
        {
        }

        /*****************************************************/
        void Integer::Execute(Machine& machine)
        {
        }
    }
 }


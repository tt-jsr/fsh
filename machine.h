#pragma once

#include <vector>
#include <map>
#include "element.h"

#define private_impl public

namespace fsh
{
    class Machine
    {
    public:
        Machine(void);
        ~Machine(void);

        // Pops the tos and returns true if the value is an int and
        // != 0
        bool result_true();
        void store_variable(const std::string& name, ElementPtr& d);
        bool get_variable(const std::string& name, ElementPtr& out);
    private:
        std::vector<ElementPtr> datastack;
    private_impl:
        ElementPtr pop_data();
        ElementPtr peek_data();
        void push_data(ElementPtr&);
    };
}


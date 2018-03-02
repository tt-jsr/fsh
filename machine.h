#pragma once

#include <vector>
#include <map>
#include <functional>
#include "element.h"
#include "execution_context.h"

#define private_impl public

namespace fsh
{
    namespace instruction
    {
        class Instruction;
        typedef fsh::instrusive_ptr<Instruction> InstructionPtr;
    }

    class Machine
    {
    public:
        Machine(void);
        ~Machine(void);

        ElementPtr Execute(instruction::InstructionPtr);

        // Register a builtin function.
        void register_builtin(const std::string& name, 
                std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)>);
        void store_variable(const std::string& name, ElementPtr& d);
        bool get_variable(const std::string& name, ElementPtr& out);
        bool ConvertToBool(ElementPtr);
    private:
        static thread_local std::vector<ElementPtr> datastack;
        ExecutionContextPtr executionContext;
    private_impl:
        ElementPtr pop_data();
        ElementPtr peek_data();
        size_t size_data();
        void push_data(ElementPtr);
        void push_context();
        void pop_context();
    };
}


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

        // Substitute any $(..) variables if argument is a string
        ElementPtr resolve(ElementPtr);
        void verify(ElementPtr, int type, const char *msg);

        // Pops the tos and returns true if the value is an int and
        // != 0
        bool result_true();

        // Register a builtin function.
        // The signiture is ElementPtr (Machine&, std::vector<ElementPtr>& args);
        void register_builtin(const std::string& name, 
                std::function<ElementPtr (Machine&, std::vector<instruction::InstructionPtr>&)>);
        void store_variable(const std::string& name, ElementPtr& d);
        bool get_variable(const std::string& name, ElementPtr& out);
    private:
        std::vector<ElementPtr> datastack;
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


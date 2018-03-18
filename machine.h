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

    typedef std::map<std::string, size_t> FieldMap_t;
    typedef std::map<std::string, FieldMap_t> RecordMap_t;

    class Machine
    {
    public:
        Machine(void);
        ~Machine(void);

        ElementPtr Execute(instruction::InstructionPtr);

        // Resolve the arg if it is an identifier, will return the 
        // element it points to
        ElementPtr resolve(ElementPtr);

        void register_unittest(std::function<void (int)>&);
        // Register a builtin function.
        void register_builtin(const std::string& name, 
                std::function<ElementPtr (Machine&, std::vector<ElementPtr>&)>);
        void store_variable(const std::string& name, ElementPtr d);
        bool get_variable(const std::string& name, ElementPtr& out);
        bool get_variable(const std::string& name, bool& out);
        bool get_variable(const std::string& name, std::string& out);
        bool get_variable(const std::string& name, int64_t& out);
        bool get_variable(const std::string& name, double& out);
        bool ConvertToBool(ElementPtr);
        ExecutionContextPtr GetContext();
        // Given a listtype and an Identifier of the fieldname, return the index into the list
        size_t get_record_field(const std::string& list, ElementPtr);

        // Given a listtype and and fieldname, return the index into the list.
        size_t get_record_field(const std::string& list, const std::string& fieldname);

        void add_record_field(const std::string& list, const std::string& field, size_t idx);

        // Given a name in dotted format, return the field of the list
        bool get_list_field(const std::string& name, ElementPtr& out);
        FieldMap_t * get_field_map(const std::string&);
    private:
        static thread_local std::vector<ElementPtr> datastack;
        ExecutionContextPtr executionContext;
        RecordMap_t record_fields;
    private_impl:
        void reset();
        ElementPtr pop_data();
        ElementPtr peek_data();
        size_t size_data();
        void push_data(ElementPtr);
        void push_context();
        void pop_context();
        std::function<void (int)> unittest_callback;
        std::function<void (const char *)> unittest_exception;
    };
}


#pragma once

#include <vector>
#include <map>
#include <functional>
#include "element.h"
#include "execution_context.h"
#include "bytecode.h"

#define private_impl public

namespace fsh
{
    class Ast;
    typedef std::map<std::string, size_t> FieldMap_t;
    typedef std::map<std::string, FieldMap_t> RecordMap_t;

    class Machine
    {
    public:
        Machine(void);
        ~Machine(void);

        ElementPtr Execute(Ast *);

        // Resolve the arg if it is an identifier, will return the 
        // element it points to
        ElementPtr resolve(ElementPtr);

        void set_gp_register(bool b) {gp_register = b;}
        bool get_gp_register() {return gp_register;}

        void register_unittest(std::function<void (int)>&);
        // Register a function.
        int64_t registerFunction(FunctionDefinition *);
        FunctionDefinition *getFunction(int64_t id);

        int64_t storeBlock(ByteCode&);
        ByteCode *getBlock(int64_t id);

        ByteCode& get_byte_code();
        // Store constant string in the string table
        std::string string_table_get(int64_t id);
        int64_t string_table_add(const std::string& s);
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
    private_impl:
        bool gp_register;
        ByteCode byte_code;
        std::unordered_map<int64_t, std::string> string_table_by_id;
        std::unordered_map<std::string, int64_t> string_table_by_string;
        int64_t next_string_id;
        std::unordered_map<int64_t, FunctionDefinition *> functions;
        int64_t next_function_id;
        std::unordered_map<int64_t, ByteCode> blocks;
        int64_t next_block_id;
        std::function<void (int)> unittest_callback;
        std::function<void (const char *)> unittest_exception;
    };
}


#pragma once
#include <vector>

namespace fsh
{
    class Machine;

    struct ByteCode
    {
        ByteCode()
        :ip(0)
        {}

        size_t size() {return byte_codes.size();}
        void clear() {byte_codes.clear();}

        uintptr_t& operator[](size_t idx)
        {
            return byte_codes[idx];
        }

        size_t current_location()
        {
            return size()-1;
        }

        size_t jump_forward(uintptr_t op)
        {
            byte_codes.push_back(op);
            byte_codes.push_back(0);
            return byte_codes.size() - 1;
        }

        size_t jump_to(uintptr_t op, uintptr_t loc)
        {
            byte_codes.push_back(op);
            byte_codes.push_back(loc);
            return byte_codes.size() - 1;
        }

        size_t code(uintptr_t n)
        {
            byte_codes.push_back(n);
            return byte_codes.size() - 1;
        }

        size_t code(uintptr_t n, uintptr_t n2)
        {
            byte_codes.push_back(n);
            byte_codes.push_back(n2);
            return byte_codes.size() - 1;
        }

        size_t code(uintptr_t n, uintptr_t n2, uintptr_t n3)
        {
            byte_codes.push_back(n);
            byte_codes.push_back(n2);
            byte_codes.push_back(n3);
            return byte_codes.size() - 1;
        }

        void set_jump_location(size_t idx)
        {
            byte_codes[idx] = byte_codes.size()-1;
        }

        std::vector<uintptr_t> byte_codes;
        size_t ip;
    };

    bool Execute(Machine&, ByteCode&);
}

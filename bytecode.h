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

        int64_t& operator[](size_t idx)
        {
            return byte_codes[idx];
        }

        size_t current_location()
        {
            return size()-1;
        }

        size_t jump_forward(int64_t op)
        {
            byte_codes.push_back(op);
            byte_codes.push_back(0);
            return byte_codes.size() - 1;
        }

        size_t jump_to(int64_t op, int64_t loc)
        {
            byte_codes.push_back(op);
            byte_codes.push_back(loc);
            return byte_codes.size() - 1;
        }

        size_t code(int64_t n)
        {
            byte_codes.push_back(n);
            return byte_codes.size() - 1;
        }

        size_t code(int64_t n, int64_t n2)
        {
            byte_codes.push_back(n);
            byte_codes.push_back(n2);
            return byte_codes.size() - 1;
        }

        void set_jump_location(size_t idx)
        {
            byte_codes[idx] = byte_codes.size()-1;
        }

        std::vector<int64_t> byte_codes;
        size_t ip;
    };

    void Execute(Machine&, ByteCode&);
}

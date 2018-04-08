#pragma once
#include <vector>

namespace fsh
{
    class Machine;

    struct ByteCode
    {
        size_t size() {return byte_codes.size();}
        void clear() {byte_codes.clear();}

        int64_t operator[](size_t idx)
        {
            return byte_codes[idx];
        }

        size_t push_jump(int64_t op, size_t loc = 0)
        {
            byte_codes.push_back(op);
            byte_codes.push_back(loc);
            return byte_codes.size() - 1;
        }

        size_t push_back(int64_t n)
        {
            byte_codes.push_back(n);
            return byte_codes.size() - 1;
        }

        void setToCurrent(size_t idx)
        {
            byte_codes[idx] = byte_codes.size()-1;
        }

        std::vector<int64_t> byte_codes;
    };

    void Execute(Machine&, ByteCode&, size_t& ip);
}

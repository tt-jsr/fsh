#pragma once
#include <memory>
#include <cstring>
#include <iostream>

namespace fsh
{
    class Data
    {
	public:
        enum Type {INVALID, INT64, STRING};
        Data()
            :type(INVALID)
        {
            memset(&u, 0, sizeof(u));
        }

        ~Data()
        {
            reset();
        }

        Data(const Data& d)
        {
            copy(d);
        }

        Data& operator=(const Data& rhs)
        {
            if (this == &rhs)
                return *this;
            copy(rhs);
            return *this;
        }

        void set_int64(int64_t n)
        {
            reset();
            type = UINT64;
            u.i = n;
        }

        void set_string(const std::string& s)
        {
            reset();
            set_string(s.c_str());
        }

        void set_string(const char *s)
        {
            reset();
            type = STRING;
            u.s = strdup(s);
        }

        void set_invalid()
        {
            reset();
        }

        uint64_t get_int64() const;
        std::string get_string() const;
        bool is_integral() const {return type == INT64;}
        bool is_string() const {return type == STRING;}
        bool is_invalid() const {return type == INVALID;}

        void dump(std::ostream& strn);

        Type type;
        union 
        {
            int64_t i;
            const char *s;
        } u;
    private:
        void reset()
        {
            if (type == STRING)
                delete u.s;
            type = INVALID;
            memset(&u, 0, sizeof(u));
        }

        void copy(const Data& cc)
        {
            switch(cc.type)
            {
                case STRING:
                {
                    set_string(cc.u.s);
                    break;
                }
                case UINT64:
                    set_int64(cc.u.i);
                    break;
            }
        }
    };
}


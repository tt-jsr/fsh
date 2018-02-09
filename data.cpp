#include "data.h"
#include <cstdlib>

namespace fsh
{
    uint64_t Data::get_int64() const
    {
        if (type == Data::STRING)
        {
            return std::strtoull(u.s, nullptr, 10);
        }
        if (type == Data::INT64)
        {
            return u.i;
        }
        return 0;
    }

    std::string Data::get_string() const
    {
        if (type == Data::INT64)
        {
            char buf[64];
            snprintf(buf, sizeof(buf), "%lu", u.i);
            return std::string(buf);
        }
        else if (type == Data::STRING)
        {
            return std::string(u.s);
        }
        return std::string("");
    }

    void Data::dump(std::ostream& strm)
    {
        switch(type)
        {
        case STRING:
            strm << "STRING: " << u.s << std::endl;
            break;
        case INT64:
            strm << "INT64: " << u.i << std::endl;
            break;
        case INVALID:
            strm << "INVALID" << std::endl;
            break;
        default:
            strm << "Unknown type:" << type << std::endl;
        }
    }
}


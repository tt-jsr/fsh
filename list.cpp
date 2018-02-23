#include <cstdint>
#include <cassert>
#include <iostream>
#include "common.h"
#include "list.h"
#include "instructions.h"

namespace fsh
{
    ListPtr Error(const std::string& str)
    {
        ListPtr r = MakeList(HEAD_TYPE_ERROR);

        r->elements.push_back(MakeString(str));
        return r;
    }

    ListPtr Part(ListPtr list, ListPtr args)
    {
        assert(args);
        if (args->elements.size() == 1)
            return Error("Part: invalid args");
        if (args->elements.size() == 2)
        {
            if (args->elements[1]->IsInteger() == false)
                return Error("Part: invalid args, expected int");
            IntegerPtr n = args->elements[1].cast<Integer>();
            if (n->value > 0)
            {
                if (n->value >= list->size())
                    return Error("Part: index out of range");
                ListPtr r = MakeList(HEAD_TYPE_LIST);
                r->elements.push_back(list->elements[n->value]);
                return r;
            }
            if (n->value < 0)
            {
                int64_t idx = static_cast<int64_t>(list->size()) + n->value;
                if (idx <= 0)
                    return Error("Part: index out of range");
                ListPtr r = MakeList(HEAD_TYPE_LIST);
                r->elements.push_back(list->elements[idx]);
                return r;
            }
        }
        return Error("Part: multiple args not supported");
    }

    void List::Add(const std::string& s)
    {
        elements.push_back(MakeString(s));
    }

    void List::Add(int64_t n)
    {
        elements.push_back(MakeInteger(n));
    }

    void Dump(ElementPtr e)
    {
        switch (e->type())
        {
        case ELEMENT_TYPE_STRING:
            {
                StringPtr sp = e.cast<String>();
                std::cout << "STRING: " << sp->value << std::endl;
            }
            break;
        case ELEMENT_TYPE_INTEGER:
            {
                IntegerPtr ip = e.cast<Integer>();
                std::cout << "INT: " << ip->value << std::endl;
            }
            break;
        case ELEMENT_TYPE_LIST:
            {
                ListPtr lp = e.cast<List>();
                std::cout << "LIST: ";
                Dump(lp);
            }
            break;
        case ELEMENT_TYPE_HEAD:
            {
                HeadPtr hp = e.cast<Head>();
                std::cout << "HEAD: " << hp->value << std::endl;
            }
            break;
        }
    }

    void Dump(ListPtr l)
    {
        for (auto e : l->elements)
        {
            Dump(e);
        }
    }
}

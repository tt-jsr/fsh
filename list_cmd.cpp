#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <limits>
#include "common.h"
#include "element.h"
#include "machine.h"
#include "builtins.h"

namespace fsh
{
    ElementPtr Part(int64_t start, int64_t end, const std::string& src)
    {
        if(start < 0)
            start = src.size() + start;
        if (end < 0)
            end = src.size() + end;
        if (start < 0 || end < 0)
            throw std::runtime_error("Invalid part");
        if (start > end)
            throw std::runtime_error("Invalid part");
        if (start > src.size() || end > src.size())
            throw std::runtime_error("Invalid part");
        std::string s = src.substr(start,end-start);
        return MakeString(s);
    }

    ElementPtr Part(int64_t start, int64_t end, const std::vector<ElementPtr>& src)
    {
        if(start <0)
            start = src.size()+ start;
        else
            ++start;
        if (end < 0)
            end = src.size() + end;
        else
            ++end;
        if (start < 0 || end < 0)
            throw std::runtime_error("Invalid part");
        if (start > end)
            throw std::runtime_error("Invalid part");
        if (start > src.size() || end > src.size())
            throw std::runtime_error("Invalid part");

        ListPtr rtn = MakeList(HEAD_TYPE_LIST);  // Parting loses the original list type
        for (size_t idx = start; idx < end; ++idx)
        {
            rtn->items.push_back(src[idx]);
        }
        return rtn;
    }

    ElementType GetElementType(size_t idx, std::vector<ElementPtr>& args)
    {
        if (idx >= args.size())
            return ELEMENT_TYPE_NONE;
        ElementType e = args[idx]->type();
        if (e == ELEMENT_TYPE_INTEGER)
            return e;
        if (e == ELEMENT_TYPE_STRING)
        { 
            if (args[idx].cast<String>()->value == ":")
                return e;
        }
        throw std::runtime_error("Part: invalid argument");
    }

    size_t GetListSize(ElementPtr e)
    {
        if (e->IsList())
        {
            return e.cast<List>()->items.size() - 1;
        }
        if (e->IsString())
        {
            return e.cast<String>()->value.size();
        }
        return 0;
    }

    ElementPtr Part(Machine& machine, std::vector<ElementPtr>& args)
    {
        int64_t start = std::numeric_limits<int64_t>::max();
        int64_t end = std::numeric_limits<int64_t>::max();

        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("Part requires a sequence");
        if (GetElementType(1, args) == ELEMENT_TYPE_INTEGER)
        {
            start = args[1].cast<Integer>()->value;
            if (GetElementType(2, args) == ELEMENT_TYPE_STRING) // ':'
            {
                if (GetElementType(3, args) == ELEMENT_TYPE_INTEGER)
                    end = args[3].cast<Integer>()->value;
                else
                    end = GetListSize(e);
            }
            else
            {
                end = start+1;
            }
        }
        else
        {
            start = 0;
            if (GetElementType(2, args) == ELEMENT_TYPE_INTEGER)
                end = args[2].cast<Integer>()->value;
            else
                end = GetListSize(e);
        }

        if (start == std::numeric_limits<int64_t>::max())
        {
            throw std::runtime_error("Part: uninitialized start");
        }
        if (end == std::numeric_limits<int64_t>::max())
        {
            throw std::runtime_error("Part: uninitialized end");
        }
        if (e->IsString())
        {
            StringPtr p = e.cast<String>();
            return Part(start,end, p->value);
        }
        if (e->IsList())
        {
            ListPtr p = e.cast<List>();
            return Part(start,end, p->items);
        }
        throw std::runtime_error("Trying to part non string or list");
    }


    ElementPtr MakeRecord(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = MakeList(HEAD_TYPE_LIST);
        size_t idx = 0;
        for(auto& e : args)
        {
            if (!e->IsIdentifier())
                throw std::runtime_error("MakeRecord: Argument must be an identifier");
            IdentifierPtr id = e.cast<Identifier>();
            lst->items.push_back(MakeNone());
            ExecutionContextPtr ctx = machine.GetContext()->parent;
            ctx->AddVariable(id->value, MakeInteger(idx));
            ++idx;
        }
        return lst;
    }

    IntegerPtr Len(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Len: arument must be a list");
        int64_t s = (int64_t)lst->items.size();
        return MakeInteger(s-1);
    }
}


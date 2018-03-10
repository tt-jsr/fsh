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
            start = src.size() + start;
        if (end < 0)
            end = src.size() + end;
        if (start < 0 || end < 0)
            throw std::runtime_error("Invalid part");
        if (start > end)
            throw std::runtime_error("Invalid part");
        if (start > src.size() || end > src.size())
            throw std::runtime_error("Invalid part");

        ListPtr rtn = MakeList("__list__");  // Parting loses the original list type
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
            return e.cast<List>()->items.size();
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


    ElementPtr DefineRecord(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (!sp)
            throw std::runtime_error("MakeRecord: First argument must be the name of the list");

        if (machine.get_field_map(sp->value) != nullptr)
        {
            std::stringstream strm;
            strm << "DefineRecord: record \"" << sp->value << "\" already exists";
            throw std::runtime_error(strm.str());
        }
        size_t idx = 0;
        for (size_t idx = 1; idx < args.size(); idx++)
        {
            IdentifierPtr id = GetIdentifier(machine, args, idx);
            if (!id)
                throw std::runtime_error("MakeRecord: Argument must be an identifier");
            machine.add_record_field(sp->value, id->value, idx-1);
        }
        return MakeNone();
    }

    ElementPtr MakeRecord(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (!sp)
            throw std::runtime_error("MakeRecord: First argument must be the name of the list");
        FieldMap_t *pFields = machine.get_field_map(sp->value);
        if (pFields == nullptr)
        {
            std::stringstream strm;
            strm << "Record " << sp->value << " doe not exist";
            throw std::runtime_error(strm.str());
        }

        ListPtr lst = MakeList(sp->value.c_str());
        size_t nfields = pFields->size();
        for (size_t idx = 0; idx < nfields; idx++)
        {
            lst->items.push_back(MakeNone());
        }
        return lst;
    }

    IntegerPtr Len(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            int64_t s = (int64_t)sp->value.size();
            return MakeInteger(s);
        }
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Len: arument must be a list or string");
        int64_t s = (int64_t)lst->items.size();
        return MakeInteger(s);
    }
}


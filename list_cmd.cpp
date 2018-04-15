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
        std::string s = src.substr(start,end-start);
        return MakeString(s);
    }

    ElementPtr Part(int64_t start, int64_t end, const std::vector<ElementPtr>& src)
    {
        if (end-start== 1)
        {
            return src[start];
        }
        else
        {
            ListPtr rtn = MakeList("__list__");  // Parting loses the original list type
            for (size_t idx = start; idx < end; ++idx)
            {
                rtn->items.push_back(src[idx]);
            }
            return rtn;
        }
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

    ElementPtr Subscript(Machine& machine, std::vector<ElementPtr>& args)
    {
        const int64_t maxint = std::numeric_limits<int64_t>::max();
        int64_t start = maxint;
        int64_t end = maxint;
        bool hasRange = false;

        ElementPtr sequence = GetElement(machine, args, 0);
        if (!sequence)
            throw std::runtime_error("[[]] requires a sequence");
        IntegerPtr eStart = GetInteger(machine, args, 1);
        BooleanPtr eHasRange = GetBoolean(machine, args, 2);
        IntegerPtr eEnd = GetInteger(machine, args, 3);

        if (eStart)
            start = eStart->value;
        if (eEnd)
            end = eEnd->value;
        if (eHasRange)
            hasRange = eHasRange->value;

        // list[m]
        if (start != maxint && !hasRange && end == maxint)
        {
            if (start < 0)
                start = GetListSize(sequence) + start;
            end = start+1;
        }

        // list[m : n]
        if (start != maxint && hasRange && end != maxint)
        {
            if (start < 0)
                start = GetListSize(sequence) + start;
            if (end < 0)
                end = GetListSize(sequence) + end;
        }

        // List[: n]
        if (start == maxint && hasRange && end != maxint)
        {
            start = 0;
            if (end < 0)
                end = GetListSize(sequence) + end;
        }

        //List[n : ]
        if (start != maxint && hasRange && end == maxint)
        {
            if (start < 0)
                start = GetListSize(sequence) + start;
            end = GetListSize(sequence);
        }

        if (start >= GetListSize(sequence) || start < 0)
        {
            throw std::runtime_error("[[]]: start index out of range");
        }
        if (end < 0 || end > GetListSize(sequence))
        {
            throw std::runtime_error("[[]]: end index out of range");
        }
        if (sequence->IsString())
        {
            StringPtr p = sequence.cast<String>();
            return Part(start,end, p->value);
        }
        if (sequence->IsList())
        {
            ListPtr p = sequence.cast<List>();
            return Part(start,end, p->items);
        }
        throw std::runtime_error("Trying to [[]] non string or list");
    }

    ElementPtr Part(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        IntegerPtr n = GetInteger(machine, args, 1);
        if (!n)
        {
            throw std::runtime_error("Part an integer indicies");
        }
        int64_t idx = n->value;
        size_t size = GetListSize(e);
        if (idx < 0)
            idx = GetListSize(e) + idx;
        if (idx < 0 || idx >= size)
            throw std::runtime_error("Index out of range");
        if (e->IsString())
        {
            StringPtr p = e.cast<String>();
            std::string s;
            s.push_back(p->value[idx]);
            return MakeString(s);
        }
        if (e->IsList())
        {
            ListPtr p = e.cast<List>();
            return p->items[idx];
        }
        throw std::runtime_error("Trying to part non string or list");
    }


    NonePtr DefineRecord(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (!sp)
            throw std::runtime_error("DefineRecord: First argument must be the name of the list");

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
                throw std::runtime_error("DefineRecord: Argument must be an identifier");
            machine.add_record_field(sp->value, id->value, idx-1);
        }
        return MakeNone();
    }

    ListPtr CreateRecord(Machine& machine, std::vector<ElementPtr>& args)
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

    ListPtr CreateList(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lp = MakeList("__list__");
        for (ElementPtr& e : args)
        {
            lp->items.push_back(e);
        }
        return lp;
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
        if (lst)
        {
            int64_t s = (int64_t)lst->items.size();
            return MakeInteger(s);
        }
        MapPtr map = GetMap(machine, args, 0);
        if (map)
        {
            int64_t s = (int64_t)map->map.size();
            return MakeInteger(s);
        }
        throw std::runtime_error("Len: Unsupported type");
    }

    IntegerPtr Append(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Append: argument must be a list");
        ElementPtr e = GetElement(machine, args, 1);
        if (!e)
            throw std::runtime_error("Append: requires second argument");

        lst->items.push_back(e);
        return MakeInteger(lst->items.size()-1);
    }

    ListPtr SetRecordType(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lp = GetList(machine, args, 0);
        if (!lp)
            throw std::runtime_error("SetRecordType: Arg1 must be a list");
        StringPtr sp = GetString(machine, args, 1);
        if (!sp)
            throw std::runtime_error("SetRecordType: Arg2 must be a string");
        lp->listtype = sp->value;
        return lp;
    }
}


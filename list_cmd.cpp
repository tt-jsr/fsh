#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <limits>
#include "instrusive_ptr.h"
#include "common.h"
#include "element.h"
#include "bytecode.h"
#include "builtins.h"
#include "machine.h"

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

    ListPtr Head(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lp = GetList(machine, args, 0);
        if (!lp)
            throw std::runtime_error("Head requires list as first argument");

        if (lp->items.size() == 0)
            throw std::runtime_error("Head list is empty");

        ListPtr rtn = MakeList();
        rtn->items.push_back(lp->items[0]);
        return rtn;
    }

    ListPtr Tail(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lp = GetList(machine, args, 0);
        if (!lp)
            throw std::runtime_error("Head requires list as first argument");

        if (lp->items.size() == 0)
            throw std::runtime_error("Head list is empty");

        ListPtr rtn = MakeList();
        for(size_t idx = 1; idx < lp->items.size(); ++idx)
        {
            rtn->items.push_back(lp->items[idx]);
        }
        return rtn;
    }

    ListPtr SplitList(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool bySize(false);

        ListPtr lp = GetList(machine, args, 0);
        if (!lp)
            throw std::runtime_error("SplitList expects list as first argument");
        IntegerPtr np = GetInteger(machine, args, 1);
        if (!np)
            throw std::runtime_error("SplitList expects integer as second argument");
        
        machine.get_variable("bySize", bySize);

        size_t sz = 0;
        size_t nlists = 0;
        if (bySize)
        {
            sz = np->value;
            nlists = lp->items.size()/sz;
        }
        else
        {
            sz = lp->items.size()/np->value;
            nlists = np->value;
        }
        ListPtr rtn = MakeList();
        rtn->items.reserve(nlists);
        size_t pos = 0;
        for (int i = 0; i < nlists; ++i)
        {
            ListPtr l = MakeList();
            l->items.insert(l->items.end(), lp->items.begin()+pos, lp->items.begin()+pos+sz);
            rtn->items.push_back(l);
            pos += sz;
        }
        ListPtr ll = rtn->items.back().cast<List>();
        ll->items.insert(ll->items.end(), lp->items.begin()+pos, lp->items.end());
        return rtn;
    }

    ElementPtr Item(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        IntegerPtr n = GetInteger(machine, args, 1);
        if (!n)
        {
            throw std::runtime_error("Item an integer indicies");
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
        throw std::runtime_error("Trying to get item non string or list");
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


    ListPtr Push(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Push: argument must be a list");

        ElementPtr e = GetElement(machine, args, 1);
        if (!e)
            throw std::runtime_error("Push: Requires something to push");

        lst->items.push_back(e);

        return lst;
    }

    ElementPtr Pop(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Pop: argument must be a list");

        if (lst->items.size() == 0)
            throw std::runtime_error("Pop: empty list");

        ElementPtr e = lst->items.back();
        lst->items.pop_back();

        return e;
    }

    ListPtr Set(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Set: argument must be a list");

        IntegerPtr ip = GetInteger(machine, args, 1);
        if (!ip)
            throw std::runtime_error("Set: argument 2 must be an integer");

        if (args.size() < 3)
            throw std::runtime_error("Set: requires argument 3 for the value to set");

        if (ip->value >= args.size())
            throw std::runtime_error("Set: index out of bounds");

        lst->items[ip->value] = args[2];

        return lst;
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


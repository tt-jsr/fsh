#include <unordered_map>
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
    MapPtr CreateMap(Machine& machine, std::vector<ElementPtr>& args)
    {
        return MakeMap();
    }

    MapPtr CreateMapFromList(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool abortOnError(false);

        MapPtr rtn = MakeMap();
        ListPtr lp = GetList(machine, args, 0);
        if (!lp)
            throw std::runtime_error("CreateMapOfList requires a list of lists as the first arg");
        IntegerPtr ip = GetInteger(machine, args, 1);
        if (!ip)
            throw std::runtime_error("CreateMapFromList required list index as second arg");
        machine.get_variable("abortOnError", abortOnError);
        size_t idx = ip->value;
        for (ElementPtr e : lp->items)
        {
            if (!e->IsList())
                throw std::runtime_error("CreateMapFromList, requires a list of lists");
            ListPtr l = e.cast<List>();
            if (idx >= l->items.size())
            {
                if (abortOnError)
                    throw std::runtime_error("CreateMapFromList index out of bounds");
            }
            else
            {
                ElementPtr k = l->items[idx];
                auto it = rtn->map.find(k);
                if (it == rtn->map.end())
                {
                    ListPtr x = MakeList();
                    x->items.push_back(l);
                    rtn->map.emplace(k, x);
                }
                else
                {
                    ListPtr x = it->second.cast<List>();
                    x->items.push_back(l);
                }
            }
        }
        return rtn;
    }

    ElementPtr Lookup(Machine& machine, std::vector<ElementPtr>& args)
    {
        MapPtr mp = GetMap(machine, args, 0);
        if (!mp)
        {
            throw std::runtime_error("Lookup: required map as first arg");
        }
        ElementPtr key = GetElement(machine, args, 1);
        if (!key)
            throw std::runtime_error("Error Lookup[map, key]");

        auto it = mp->map.find(key);
        if (it == mp->map.end())
            return MakeNone();
        return it->second;
    }

    ElementPtr MapInsert(Machine& machine, std::vector<ElementPtr>& args)
    {
        MapPtr mp = GetMap(machine, args, 0);
        if (!mp)
        {
            throw std::runtime_error("MapInsert: required map as first arg");
        }
        ElementPtr key = GetElement(machine, args, 1);
        if (!key)
            throw std::runtime_error("Error MapInsert[map, key, value]");

        ElementPtr value = GetElement(machine, args, 2);
        if (!value)
            throw std::runtime_error("Error MapInsert[map, key, value]");
        mp->map[key] = value;
        return mp;
    }

    ElementPtr ListInsert(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("ListInsert: argument must be a list");

        IntegerPtr ip = GetInteger(machine, args, 1);
        if (!ip)
            throw std::runtime_error("ListInsert: argument 2 must be an integer");
        if (ip->value >= lst->items.size())
            throw std::runtime_error("ListInsert: position out of bounds");

        auto itSrc = args.begin() + 2;
        auto itDst = lst->items.begin() + ip->value;

        lst->items.insert(itDst, itSrc, args.end());

        return lst;
    }

    ElementPtr StringInsert(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr lst = GetString(machine, args, 0);
        if (!lst)
            throw std::runtime_error("StringInsert: argument must be a string");

        IntegerPtr ip = GetInteger(machine, args, 1);
        if (!ip)
            throw std::runtime_error("StringInsert: argument 2 must be an integer");
        if (ip->value >= lst->value.size())
            throw std::runtime_error("StringInsert: position out of bounds");

        StringPtr s = GetString(machine, args, 2);
        if (!s)
            throw std::runtime_error("StringInsert: argument 3 must be a string");

        lst->value.insert(ip->value, s->value);

        return lst;
    }

    ElementPtr Insert(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (e->IsMap())
            return MapInsert(machine, args);
        if (e->IsList())
            return ListInsert(machine, args);
        if (e->IsString())
            return StringInsert(machine, args);
        throw std::runtime_error("Insert: ust be a string, list or map");
    }

    ElementPtr MapDelete(Machine& machine, std::vector<ElementPtr>& args)
    {
        MapPtr mp = GetMap(machine, args, 0);
        if (!mp)
        {
            throw std::runtime_error("MapDelete: required map as first arg");
        }
        ElementPtr key = GetElement(machine, args, 1);
        if (!key)
            throw std::runtime_error("MapDelete[map, key]");

        mp->map.erase(key);
        return MakeNone();
    }

    ListPtr ListDelete(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Delete: required list as first arg");
        IntegerPtr ip  = GetInteger(machine, args, 1);
        if (!ip)
            throw std::runtime_error("ListDelete requires integer position");

        if (ip->value >= lst->items.size())
            throw std::runtime_error("ListDelete: index out of bounds");
        auto it = lst->items.begin() + ip->value;
        lst->items.erase(it);
        return lst;
    }

    ElementPtr Delete(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("Delete: map or list must be specified");
        if(e->IsMap())
            return MapDelete(machine, args);
        if(e->IsList())
            return ListDelete(machine, args);
        throw std::runtime_error("Delete invalid type for delete");
    }
}


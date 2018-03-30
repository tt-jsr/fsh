#include <unordered_map>
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
    MapPtr CreateMap(Machine& machine, std::vector<ElementPtr>& args)
    {
        return MakeMap();
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

    ElementPtr Insert(Machine& machine, std::vector<ElementPtr>& args)
    {
        MapPtr mp = GetMap(machine, args, 0);
        if (!mp)
        {
            throw std::runtime_error("Insert: required map as first arg");
        }
        ElementPtr key = GetElement(machine, args, 1);
        if (!key)
            throw std::runtime_error("Error Insert[map, key, value]");

        ElementPtr value = GetElement(machine, args, 2);
        if (!value)
            throw std::runtime_error("Error Insert[map, key, value]");
        mp->map[key] = value;
        return MakeNone();
    }

    ElementPtr Delete(Machine& machine, std::vector<ElementPtr>& args)
    {
        MapPtr mp = GetMap(machine, args, 0);
        if (!mp)
        {
            throw std::runtime_error("Delete: required map as first arg");
        }
        ElementPtr key = GetElement(machine, args, 1);
        if (!key)
            throw std::runtime_error("Error Delete[map, key]");

        mp->map.erase(key);
        return MakeNone();
    }

}


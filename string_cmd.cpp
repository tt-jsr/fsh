
#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include "common.h"
#include "element.h"
#include "machine.h"
#include "builtins.h"

namespace
{
    // trim from start
    std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
    }
}

namespace fsh
{
    ElementPtr Trim(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            std::string s = sp->value;
            return MakeString(trim(s));
        }
        return MakeNone();
    }

    ElementPtr TrimLeft(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            std::string s = sp->value;
            return MakeString(ltrim(s));
        }
        return MakeNone();
    }

    ElementPtr TrimRight(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            std::string s = sp->value;
            return MakeString(rtrim(s));
        }
        return MakeNone();
    }

    ElementPtr Split(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        StringPtr chars = GetString(machine, args, 1);
        if (!(sp && chars))
        {
            return MakeList("__list__");
        }
        ListPtr lst = MakeList("__list__");
        std::string s;
        for (auto it = sp->value.begin(); it != sp->value.end(); ++it)
        {
            if (chars->value.find_first_of(*it) != std::string::npos)
            {
                StringPtr item = MakeString(s);
                lst->items.push_back(item);
                s = "";
            }
            else
            {
                s.push_back(*it);
            }
        }
        if (s.size())
        {
            StringPtr item = MakeString(s);
            lst->items.push_back(item);
        }
        return lst;
    }

    ElementPtr Strcmp(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr s1 = GetString(machine, args, 0);
        if (!s1)
            throw std::runtime_error("Strcmp: invalid argument");
        StringPtr s2 = GetString(machine, args, 1);
        if (!s2)
            throw std::runtime_error("Strcmp: invalid argument");
        bool nocase(false);
        machine.get_variable("__nocase", nocase);
        int n;
        if(nocase)
            n = strcasecmp(s1->value.c_str(), s2->value.c_str());
        else
            n = strcmp(s1->value.c_str(), s2->value.c_str());
        return MakeInteger(n);
    }

    ElementPtr RegSearch(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr str = GetString(machine, args, 0);
        if (!str)
            throw std::runtime_error("Strcmp: invalid argument");
        StringPtr regex = GetString(machine, args, 1);
        if (!regex)
            throw std::runtime_error("Strcmp: invalid argument");
        return MakeBoolean(false);
    }

    ElementPtr RegMatch(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr str = GetString(machine, args, 0);
        if (!str)
            throw std::runtime_error("Strcmp: invalid argument");
        StringPtr regex = GetString(machine, args, 1);
        if (!regex)
            throw std::runtime_error("Strcmp: invalid argument");
        return MakeBoolean(false);
    }
}



#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include "instrusive_ptr.h"
#include "common.h"
#include "element.h"
#include "bytecode.h"
#include "builtins.h"
#include "machine.h"

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
    StringPtr Trim(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            std::string s = sp->value;
            return MakeString(trim(s));
        }
        throw std::runtime_error("Trim requires a string argument");
    }

    StringPtr TrimLeft(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            std::string s = sp->value;
            return MakeString(ltrim(s));
        }
        throw std::runtime_error("Trim requires a string argument");
    }

    StringPtr TrimRight(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (sp)
        {
            std::string s = sp->value;
            return MakeString(rtrim(s));
        }
        throw std::runtime_error("Trim requires a string argument");
    }

    IntegerPtr Find(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr s = GetString(machine, args, 0);
        if (!s)
            return MakeInteger(-1);
        StringPtr search = GetString(machine, args, 1);
        if (!search)
            return MakeInteger(-1);

        size_t pos = s->value.find(search->value);
        if (pos == std::string::npos)
            return MakeInteger(-1);
        return MakeInteger(pos);
    }

    int collect_argnum(const char *& p)
    {
        ++p; // get pass the '{'
        if (*p == '}')
        {
            ++p;
            return -1;
        }
        std::string arg;
        while (*p && isdigit(*p))
        {
            arg.push_back(*p);
            ++p;
        }
        if (*p != '}')
            return -1;
        return std::stol(arg);
    }

    StringPtr Format(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (!sp)
            throw std::runtime_error("Format requires a string argument");
        std::string rtn;
        const char *p = sp->value.c_str();
        while (*p)
        {
            switch(*p)
            {
            case '\\':
                ++p;
                rtn.push_back(*p);
                ++p;
                break;
            case '{':
                {
                    int idx = collect_argnum(p);
                    ++idx;
                    if (idx < 0 || idx >= args.size())
                        throw std::runtime_error("Format: index out of range");
                    rtn += toString(machine, args[idx]);
                }   
                if (*p)
                    ++p;
                break;
            default:
                rtn.push_back(*p);
                ++p;
            }
        }
        return MakeString(rtn);
    }

    ListPtr Split(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool collapse(false);
        machine.get_variable("collapse", collapse);

        StringPtr sp = GetString(machine, args, 0);
        if (!sp)
            throw std::runtime_error("Split requires a string to be split");
        StringPtr separators = GetString(machine, args, 1);
        if (!separators)
            separators = MakeString(" ");
        ListPtr lst = MakeList("__list__");
        std::string s;
        bool prevWasDelimiter(false);
        for (auto it = sp->value.begin(); it != sp->value.end(); ++it)
        {
            if (separators->value.find_first_of(*it) != std::string::npos)
            {
                StringPtr item = MakeString(s);
                lst->items.push_back(item);
                s = "";
                if (collapse)
                {
                    while (separators->value.find_first_of(*it) != std::string::npos)
                        ++it;
                    --it;
                }
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

    StringPtr Join(Machine& machine, std::vector<ElementPtr>& args)
    {
        ListPtr lst = GetList(machine, args, 0);
        if (!lst)
            throw std::runtime_error("Join expects a list as the first argument");

        StringPtr delim = GetString(machine, args, 1);
        if (!delim)
            delim = MakeString(" ");

        std::stringstream strm;
        auto it = lst->items.begin();
        if (it == lst->items.end())
        {
            return MakeString("");
        }
        strm << toString(machine, *it);
        ++it;
        for (; it != lst->items.end(); ++it)
        {
            strm << delim->value;
            strm << toString(machine, *it);
        }
        return MakeString(strm.str());
    }

    IntegerPtr Strcmp(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr s1 = GetString(machine, args, 0);
        if (!s1)
            throw std::runtime_error("Strcmp: invalid argument");
        StringPtr s2 = GetString(machine, args, 1);
        if (!s2)
            throw std::runtime_error("Strcmp: invalid argument");
        bool nocase(false);
        machine.get_variable("nocase", nocase);
        int n;
        if(nocase)
            n = strcasecmp(s1->value.c_str(), s2->value.c_str());
        else
            n = strcmp(s1->value.c_str(), s2->value.c_str());
        return MakeInteger(n);
    }

    StringPtr SubString(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr sp = GetString(machine, args, 0);
        if (!sp)
            throw std::runtime_error("SubString: Arg 1 requires a string");
        IntegerPtr pos = GetInteger(machine, args, 1);
        if (!pos)
            throw std::runtime_error("SubString: Arg 2 must be a position");
        IntegerPtr count = GetInteger(machine, args, 2);
        if (!count)
            throw std::runtime_error("SubString: Arg 3 must be a count");

        std::string s = sp->value.substr(pos->value, count->value);
        return MakeString(s);
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

